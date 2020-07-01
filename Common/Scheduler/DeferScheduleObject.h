#pragma once
#include "JobScheduler.h"
namespace MapReduce
{
  struct DeferObject
  {

    DeferObject(DeferObject&& rhs) noexcept : package_{ rhs.package_ }
    {
    }
    DeferObject operator=(DeferObject&& rhs) noexcept
    {
      std::swap(package_, rhs.package_);
    }

    template<typename Callable, typename ... CArgs>
    DeferObject(Callable&& fn, CArgs&&... cargs) : package_{ MakeFnPackage(std::forward<Callable>(fn), std::forward<CArgs>(cargs)...).pack } {}

    ~DeferObject()
    {
      package_->invoke();

      delete package_;
    }


  private:
    DeferObject(const DeferObject&) = delete;
    DeferObject operator=(const DeferObject&) = delete;
    I_FnPackage* package_;

  };



}
#ifdef _MSC_VER
#define def(LINE, Callable, ...) ::MapReduce::DeferObject Defer##c##LINE{Callable, __VA_ARGS__};
#define DEFER(Callable, ...) def(__LINE__, Callable, __VA_ARGS__)
#endif