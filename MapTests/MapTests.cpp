#include "pch.h"
#include "CppUnitTest.h"
#include "../MapReduce/MapReduce/master.h"
#include <MapReduce_Customs/wc.h>
#include <common.h>
#include <MapReduce_Customs/ii.h>
#include <Scheduler/JobScheduler.h>
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

		TEST_METHOD(MapReduceResultWC)
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
				m->doReduce("master","merge" ,mapTask, i, reduceF);
			}

			m->doMerge("merge", mapTask, nreduce);
			delete m;
		}

		TEST_METHOD(MapReduceResultii)
		{
			Master* m = new Master;
			int mapTask = 1;
			int nreduce = 5;
			for (int i = 0; i < mapTask; i++)
			{
				m->doMap("master", "pg-dorian_gray.txt", i, nreduce, iimapF);
			}
			for (int i = 0; i < nreduce; i++)
			{
				m->doReduce("master", "merge", mapTask, i, iireduceF);
			}

			m->doMerge("merge", mapTask, nreduce);
			delete m;
		}

		TEST_METHOD(MapReduceMT)
		{
			Master* m = new Master;
			
			int mapTask = 1;
			int nreduce = std::thread::hardware_concurrency();
			::MapReduce::Scheduler* s = ::MapReduce::GetScheduler(nreduce);
			std::vector<std::shared_future<void>> mappackages;
			for (int i = 0; i < mapTask; i++)
			{
				auto mappackage = ::MapReduce::MakeFnPackage(&Master::doMap, m, "master", "pg-dorian_gray.txt", i, nreduce, mapF);
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
				auto reducepackage = ::MapReduce::MakeFnPackage(&Master::doReduce, m, "master", "merge", mapTask, i, reduceF);
				s->Schedule(reducepackage.pack);
				mappackages.push_back(reducepackage.prom);
			}
			for (auto& fut : mappackages)
			{
				fut.wait();
			}
			mappackages.clear();
		}

		TEST_METHOD(iiMapReduceSeq)
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
			m->BeginSequential(input_files, iimapF, iireduceF);
		}
		TEST_METHOD(iiMapReduceMT)
		{
			Master* m = new Master;

			int mapTask = 1;
			int nreduce = std::thread::hardware_concurrency();
			::MapReduce::Scheduler* s = ::MapReduce::GetScheduler(nreduce);
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
				auto result = s->Schedule(&Master::doReduce, m, "master","merge", mapTask, i, iireduceF);
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

		TEST_METHOD(MapReduce_GRPCMasterWork)
		{
			auto* s = ::MapReduce::GetScheduler();
			int max_files = 10;
			Master m;
			std::vector<std::string> input_files{};
			auto curr_path = fs::current_path();
			int i = 0;
			for (auto& entry : fs::directory_iterator(curr_path))
			{
				if (i >= max_files)
				{
					break;
				}
				if (entry.path().extension() == ".txt")
				{
					input_files.push_back(entry.path().filename().string());
					++i;
				}
			}
			std::cout << "Files: " << i << std::endl;
			m.Init();
			s->Schedule(&Master::Start, &m, Master::Mode::DIST);
			auto start_wait = s->Schedule(&Master::StartRPCs, &m, input_files.size());
			// Wait workers to start up
			start_wait.wait();
			m.BeginRPCDistributed(input_files, iimapF, iireduceF);
			m.StopAllRPCs();
		}

		TEST_METHOD(MapReduce_GRPCMasterShutdown)
		{
			auto* s = ::MapReduce::GetScheduler();
			int max_files = 4;
			Master m;
			std::vector<std::string> input_files{};
			auto curr_path = fs::current_path();
			int i = 0;
			for (auto& entry : fs::directory_iterator(curr_path))
			{
				if (i >= max_files)
				{
					break;
				}
				if (entry.path().extension() == ".txt")
				{
					input_files.push_back(entry.path().filename().string());
					++i;
				}
			}
			std::cout << "Files: " << i << std::endl;
			m.Init();
			s->Schedule(&Master::Start, &m, Master::Mode::DIST);
			auto start_wait = s->Schedule(&Master::StartRPCs, &m, input_files.size());
			start_wait.wait();
			m.StopAllRPCs();
		}
	};

}
