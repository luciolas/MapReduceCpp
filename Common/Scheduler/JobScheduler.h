#pragma once
#include <vector>
#include <thread> 
#include <future>
#include <queue>
#include <tuple>
#include <functional>
#include <type_traits>

using WorkerFn = void(*)();

namespace MapReduce
{

  class I_FnPackage
  {

  public:
    virtual void invoke() = 0;

  };



  template<typename R>
  struct T_FnReturn
  {
    using PReturn = std::promise<R>;
    R result;

  };

  template<>
  struct T_FnReturn<void>
  {
    using VoidReturn = void;


  };

  template<typename R, typename ... Args>
  struct T_FnArgs : public T_FnReturn<R>
  {
    using TReturn = T_FnReturn<R>;
    using Index_Seq = std::make_index_sequence<sizeof ... (Args)>;
    std::tuple<std::decay_t<Args>...> args_;
    std::promise<R> p_result;
    std::shared_future<R> get_future()
    {
      return p_result.get_future();
    }
    template<typename ... CArgs>
    T_FnArgs(CArgs&&... args) : TReturn{}, args_{ std::forward<CArgs>(args)... }
    {
      printf("TFNArgs called");
    }

    /*template<typename Callable, typename Ret = R, typename dummy = typename T_FnReturn<Ret>::PReturn, std::size_t... Ns>
    Ret call(Callable fn, std::index_sequence<Ns...>)
    {
      TReturn::result = fn(std::get<Ns>(args_)...);
      p_result.set_value(TReturn::result);
      return TReturn::result;
    }

    template<typename Callable, typename Ret = R, typename dummy = typename T_FnReturn<Ret>::VoidReturn, std::size_t... Ns>
    void call(Callable fn, std::index_sequence<Ns...>)
    {
      fn(std::get<Ns>(args_)...);
      p_result.set_value();
    }

    template<typename Callable, typename Owner, typename Ret = R, typename dummy = typename T_FnReturn<Ret>::PReturn, std::size_t... Ns>
    Ret call(Callable fn, Owner* o, std::index_sequence<Ns...>)
    {
      TReturn::result = (o->*fn)(std::get<Ns>(args_)...);
      p_result.set_value(TReturn::result);
      return TReturn::result;
    }

    template<typename Callable, typename Owner, typename Ret = R, typename dummy = typename T_FnReturn<Ret>::VoidReturn, std::size_t... Ns>
    void call(Callable fn, Owner* o, std::index_sequence<Ns...>)
    {
      (o->*fn)(std::get<Ns>(args_)...);
      p_result.set_value();
    }*/

    template<typename Callable>
    void call(Callable&& fn)
    {
      call_impl(fn, Index_Seq{});
    }

  private:
    template<typename Callable, typename Ret = R, typename dummy = typename T_FnReturn<Ret>::VoidReturn, std::size_t ... Ns>
    void call_impl(Callable&& fn, std::index_sequence<Ns...>)
    {
      std::invoke(std::forward<Callable>(fn), std::get<Ns>(args_)...);
      p_result.set_value();
    }

    template<typename Callable, typename Ret = R, typename dummy = typename T_FnReturn<Ret>::PReturn, std::size_t ... Ns>
    Ret call_impl(Callable&& fn, std::index_sequence<Ns...>)
    {
      Ret result = std::invoke(std::forward<Callable>(fn), std::get<Ns>(args_)...);
      p_result.set_value(result);
      return result;
    }


  };
  //
  //struct T_FnPackageBase
  //{
  //  bool isDone;
  //
  //};
  //
  //
  //
  //
  //
  //template<typename T>
  //struct T_FnPackageBuilder
  //{
  //
  //};
  //
  //template<typename R, typename ... Args>
  //struct T_FnPackageBuilder<R(Args...)> : public I_FnPackage , T_FnArgs<R, Args...>, T_FnPackageBase
  //{
  //  using Callable = R(*)(Args...);
  //  using FNArgs = T_FnArgs<R, Args...>;
  //  using IndexSeq = std::make_index_sequence<sizeof...(Args)>;
  //  using ReturnType = R;
  //  Callable fn_;
  //  T_FnPackageBuilder() : FNArgs{} {}
  //
  //  template<typename CCallable, typename ... CArgs>
  //  T_FnPackageBuilder(CCallable&& fn, CArgs&&... args) :
  //    FNArgs{ std::forward<CArgs>(args)... },
  //    fn_{ std::forward<CCallable>(fn) }
  //  {
  //  }
  //
  //  template<typename Ret = R,  typename dummy = typename T_FnReturn<Ret>::PReturn>
  //  Ret operator()()
  //  {
  //    return FNArgs::call(fn_, IndexSeq{});
  //  }
  //
  //  template<typename Ret =  R, typename dummy = typename T_FnReturn<Ret>::VoidReturn>
  //  void operator()()
  //  {
  //    FNArgs::call(fn_, IndexSeq{});
  //  }
  //
  //  void invoke() override
  //  {
  //    this->operator()();
  //  }
  //  template<typename Ret = R>
  //  std::shared_future<Ret> get_future()
  //  {
  //    return FNArgs::get_future();
  //  }
  //};
  //
  //template<typename R,typename C, typename ... Args>
  //struct T_FnPackageBuilder<R(C::*)(Args...)> : public I_FnPackage, T_FnArgs<R, Args...>, T_FnPackageBase
  //{
  //  using Callable = R(C::*)(Args...);
  //  using FNArgs = T_FnArgs<R, Args...>;
  //  using ReturnType = R;
  //  C* owner;
  //  Callable fn_;
  //  T_FnPackageBuilder() {}
  //
  //  template<typename ... CArgs>
  //  T_FnPackageBuilder(Callable fn, C* c, CArgs&&... args) :
  //    FNArgs{ std::forward<CArgs>(args)... }, owner{ c },
  //    fn_{ fn }
  //  {}
  //
  //  template<typename Ret = R, class dummy = typename T_FnReturn<Ret>::PReturn>
  //  Ret operator()()
  //  {
  //    return FNArgs::call(fn_, owner, std::make_index_sequence<sizeof...(Args)>{});
  //  }
  //
  //  template<typename Ret = R, class dummy = typename T_FnReturn<Ret>::VoidReturn>
  //  void operator()()
  //  {
  //    FNArgs::call(fn_, owner, std::make_index_sequence<sizeof...(Args)>{});
  //  }
  //  void invoke() override
  //  {
  //    this->operator()();
  //  }
  //
  //  template<typename Ret = R>
  //  std::shared_future<Ret> get_future()
  //  {
  //    return FNArgs::get_future();
  //  }
  //
  //};
  //



  template<typename T, typename R, typename ... Args>
  class T_Fn : public I_FnPackage, public T_FnArgs<R, Args...>
  {
    using FNArgs = T_FnArgs <R, Args...>;
    T fn_;

  public:
    template<typename Fty, typename ... CArgs>
    T_Fn(Fty&& fn, CArgs&&... args) :
      T_FnArgs<R, CArgs...>{ std::forward<CArgs>(args)... },
      fn_{ std::forward<Fty>(fn) } {}

    void invoke() override
    {
      FNArgs::call(fn_);
    }

    std::shared_future< R> get_future()
    {
      return FNArgs::get_future();
    }

  };



  template <typename T>
  struct Package
  {
    std::shared_future<T> prom;
    I_FnPackage* pack;
  };


  template<typename T, typename R, typename ... Args>
  using FnPackage = T_Fn<T, R, Args...>;

  template<typename T, typename ... TArgs>
  Package<std::invoke_result_t<std::decay_t<T>, std::decay_t<TArgs>...>>
    MakeFnPackage(T&& fn, TArgs&&... capture)
  {
    using RetType = std::invoke_result_t<std::decay_t<T>, std::decay_t<TArgs>...>;
    FnPackage<T, RetType, TArgs...>* fp = new FnPackage<T, RetType, TArgs...>{ std::forward<T>(fn), std::forward<TArgs>(capture)... };

    return Package<RetType>{ fp->get_future(), fp };
  }



  class Scheduler {

    std::queue<I_FnPackage*> queue_;
    std::queue<size_t> workers_;
    std::vector<std::thread> worker_threads_;
    std::condition_variable work_signal;
    std::mutex mu_;
    bool isStopped;


    void thread_main_loop();

  public:

    Scheduler(size_t workers = 2);

    void Start();

    bool Schedule(I_FnPackage* fn);

    template<typename Callable, typename ... CArgs>
    std::shared_future<std::invoke_result_t<std::decay_t<Callable>, std::decay_t<CArgs>...>>
      Schedule(Callable&& fn, CArgs&& ... args)
    {
      auto package = MakeFnPackage(std::forward<Callable>(fn), std::forward<CArgs>(args)...);
      Schedule(package.pack);
      return package.prom;
    }

 /*   template<typename Callable, typename Owner, typename ... CArgs, typename RetType = typename FnPackage<Callable>::ReturnType>
    std::shared_future<RetType> Schedule(Callable fn, Owner* o, CArgs&& ... args)
    {
      auto package = MakeFnPackage(fn, o, std::forward<CArgs>(args)...);
      Schedule(package.pack);
      return package.prom;
    }*/

    ~Scheduler() {};
  };

  static Scheduler* GetScheduler(size_t threadCount = 40)
  {
    static Scheduler* s = new Scheduler{ threadCount };
    return s;
  }


}
