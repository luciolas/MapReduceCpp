
#include "pch.h"
#include "CppUnitTest.h"
#include "JobScheduler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


void doThis(const std::string& ll) {
  Assert::AreEqual("hello", ll.c_str(), "doThis");
}

int printFloat(float f) {
  return 1;
}

namespace SchedulerTests
{
	TEST_CLASS(SchedulerTests)
	{
	public:		
    TEST_METHOD(TestMulti)
    {
      auto sch_module = GetScheduler(4);
      std::string arg1{ "hello" };
      auto doThisPackage = MakeFnPackage(&doThis, arg1);
      sch_module->Schedule(doThisPackage->pack);
      doThisPackage->prom.wait();

      Logger::WriteMessage("Passed!\n");

      auto doThisPackage2 = MakeFnPackage(&printFloat, 2.0f);
      sch_module->Schedule(doThisPackage2->pack);
      doThisPackage2->prom.wait();

      Assert::AreEqual(1, doThisPackage2->prom.get());

      Logger::WriteMessage("Passed2!\n");
		}
	};
}
