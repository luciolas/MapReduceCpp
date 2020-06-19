#include "pch.h"
#include "CppUnitTest.h"
#include "../MapReduce/MapReduce/master.h"
#include "../MapReduce/MapReduce_Customs/wc.h"
#include "../MapReduce_Customs/ii.h"
#include "JobScheduler.h"
#include <vector>
#include <future>
#include <filesystem>

namespace fs = std::filesystem;

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
			int nreduce = std::thread::hardware_concurrency();
			Scheduler* s = GetScheduler(nreduce);
			std::vector<std::shared_future<void>> mappackages;
			for (int i = 0; i < mapTask; i++)
			{
				auto mappackage = MakeFnPackage(&Master::doMap, m, "master", "pg-dorian_gray.txt", i, nreduce, mapF);
				s->Schedule(mappackage.pack);
				mappackages.push_back(mappackage.prom);
			}
			for (auto& fut : mappackages)
			{
				fut.wait();
			}
			mappackages.clear();
			for (int i = 0; i < nreduce; i++)
			{
				auto reducepackage = MakeFnPackage(&Master::doReduce, m, "master", mapTask, i, reduceF);
				s->Schedule(reducepackage.pack);
				mappackages.push_back(reducepackage.prom);
			}
			for (auto& fut : mappackages)
			{
				fut.wait();
			}
			mappackages.clear();
		}

		TEST_METHOD(iiMapReduceMT)
		{
			Master* m = new Master;

			int mapTask = 1;
			int nreduce = std::thread::hardware_concurrency();
			Scheduler* s = GetScheduler(nreduce);
			std::vector<std::shared_future<void>> mappackages;
			for (int i = 0; i < mapTask; i++)
			{
				auto result = s->Schedule(&Master::doMap, m, "master", "pg-dorian_gray.txt", i, nreduce, iimapF);
				mappackages.push_back(result);
			}
			for (auto& fut : mappackages)
			{
				fut.wait();
			}
			mappackages.clear();
			for (int i = 0; i < nreduce; i++)
			{
				auto result = s->Schedule(&Master::doReduce, m, "master", mapTask, i, iireduceF);
				mappackages.push_back(result);
			}
			for (auto& fut : mappackages)
			{
				fut.wait();
			}
			mappackages.clear();
		}


		TEST_METHOD(iiMapReduceMT_MultipleFiles)
		{
			Master* m = new Master;
			std::vector<std::string> input_files{};
			auto curr_path = fs::current_path();
			for (auto& entry : fs::directory_iterator(curr_path))
			{
				if (entry.path().extension() == ".txt")
				{
					input_files.push_back(entry.path().filename().string());
				}
			}
			m->BeginDistributed(input_files, iimapF, iireduceF);
		}
	};

}
