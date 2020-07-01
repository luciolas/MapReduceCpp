
#include "pch.h"
#include "CppUnitTest.h"
#include <Scheduler/JobScheduler.h>

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
      auto * sch_module = ::MapReduce::GetScheduler(4);
      std::string arg1{ "hello" };
      auto doThisPackage = ::MapReduce::MakeFnPackage(&doThis, "hello");
      sch_module->Schedule(doThisPackage.pack);
      doThisPackage.prom.wait();

      Logger::WriteMessage("Passed!\n");

      auto doThisPackage2 = ::MapReduce::MakeFnPackage(&printFloat, 2.0f);
      sch_module->Schedule(doThisPackage2.pack);
      doThisPackage2.prom.wait();

      Assert::AreEqual(1, doThisPackage2.prom.get());


      auto lambdaTest = ::MapReduce::MakeFnPackage([]() { printFloat(2.0); });

      lambdaTest.pack->invoke();
      Logger::WriteMessage("Passed2!\n");
		}

    TEST_METHOD(TestMulti2)
    {
      auto* sch_module = ::MapReduce::GetScheduler(std::thread::hardware_concurrency());
      std::string arg1{ "hello" };
      auto ret_result = sch_module->Schedule(&doThis, arg1);
      ret_result.wait();
      Logger::WriteMessage("Passed!\n");

      auto ret_result2 = sch_module->Schedule(&printFloat, 2.0f);
      Assert::AreEqual(1, ret_result2.get());
      Logger::WriteMessage("Passed2!\n");
    }
	};
}
