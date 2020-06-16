#include "pch.h"
#include "CppUnitTest.h"
#include "../MapReduce/MapReduce/master.h"
#include "../MapReduce/MapReduce_Customs/wc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MapTests
{
	TEST_CLASS(MapTests)
	{
	public:
		TEST_METHOD(MapResult)
		{
			Master* m = new Master;
			m->doMap("master", "pg-dorian_gray.txt", 4, 4, mapF);
			delete m;
		}
	};
}
