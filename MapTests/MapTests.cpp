#include "pch.h"
#include "CppUnitTest.h"
#include "../MapReduce/MapReduce/master.h"
#include "../MapReduce/MapReduce_Customs/wc.h"
#include "JobScheduler.h"
#include <vector>
#include <future>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MapReduceTests
{
	TEST_CLASS(MapReduce)
	{
	public:
		TEST_METHOD(MapResult)
		{
			Master* m = new Master;
			int mapTask = 1;
			int nreduce = 4;
			for (int i = 0; i < mapTask; i++)
			{
				m->doMap("master", "pg-dorian_gray.txt", i, nreduce, mapF);
			}
			delete m;
		}

		TEST_METHOD(MapReduceResult)
		{
			Master* m = new Master;
			int mapTask = 1;
			int nreduce = 4;
			for (int i = 0; i < mapTask; i++)
			{
				m->doMap("master", "pg-dorian_gray.txt", i, nreduce, mapF);
			}
			for (int i = 0; i < nreduce; i++)
			{
				m->doReduce("master", mapTask, i, reduceF);
			}
			delete m;
		}

		TEST_METHOD(MapReduceMT)
		{
			Master* m = new Master;
			
			int mapTask = 1;
			int nreduce = 2;
			Scheduler* s = GetScheduler(std::thread::hardware_concurrency());
			std::vector<std::future<void>> mappackages;
			for (int i = 0; i < mapTask; i++)
			{
				auto mappackage = MakeFnPackage(&Master::doMap, m, "master", "pg-dorian_gray.txt", i, nreduce, mapF);
				s->Schedule(mappackage->pack);
				mappackages.push_back(mappackage->prom);
			}
			for (auto& fut : mappackages)
			{
				fut.wait();
			}
			mappackages.clear();
			for (int i = 0; i < nreduce; i++)
			{
				auto reducepackage = MakeFnPackage(&Master::doReduce, m, "master", mapTask, i, reduceF);
				s->Schedule(reducepackage->pack);
				mappackages.push_back(reducepackage->prom);
			}
			for (auto& fut : mappackages)
			{
				fut.wait();
			}
			mappackages.clear();
		}
	};
}
