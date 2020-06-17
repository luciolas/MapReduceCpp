#include "pch.h"
#include "CppUnitTest.h"
#include "../MapReduce/MapReduce/master.h"
#include "../MapReduce/MapReduce_Customs/wc.h"

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
	};
}
