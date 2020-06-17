#pragma once
#include <vector>
#include <thread> 
#include <future>
#include <queue>
#include <tuple>
#include <functional>

using WorkerFn = void(*)();

template<typename T>
struct T_FnPackageBuilder
{

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
  using VoidReturn = int ;
  

};

template<typename R, typename ... Args>
struct T_FnArgs : public T_FnReturn<R>
{
  using TReturn = T_FnReturn<R>;
  std::tuple<Args...> args_; 
  std::promise<R> p_result;
  std::future<R> get_future()
  {
    return p_result.get_future();
  }
  template<typename ... CArgs>
  T_FnArgs(CArgs&&... args) : TReturn{}, args_ { std::forward<CArgs>(args)... } 
  {
    printf("TFNArgs called");
  }

  template<typename Callable, typename Ret = R, typename dummy = typename T_FnReturn<Ret>::PReturn, std::size_t... Ns>
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
  }

};

struct T_FnPackageBase
{
  bool isDone;

};



class I_FnPackage
{

public:
  virtual void invoke() = 0;

};

template<typename R, typename ... Args>
struct T_FnPackageBuilder<R(*)(Args...)> : public I_FnPackage , T_FnArgs<R, Args...>, T_FnPackageBase
{
  using Callable = R(*)(Args...);
  using FNArgs = T_FnArgs<R, Args...>;
  using IndexSeq = std::make_index_sequence<sizeof...(Args)>;
  Callable fn_;
  T_FnPackageBuilder() : FNArgs{} {}

  template<typename ... CArgs>
  T_FnPackageBuilder(Callable fn, CArgs&&... args) :
    FNArgs{ std::forward<CArgs>(args)... },
    fn_{ fn }
  {
  }

  template<typename Ret = R,  typename dummy = typename T_FnReturn<Ret>::PReturn>
  Ret operator()()
  {
    return FNArgs::call(fn_, IndexSeq{});
  }

  template<typename Ret =  R, typename dummy = typename T_FnReturn<Ret>::VoidReturn>
  void operator()()
  {
    FNArgs::call(fn_, IndexSeq{});
  }

  void invoke() override
  {
    this->operator()();
  }
  template<typename Ret = R>
  std::future<Ret> get_future()
  {
    return FNArgs::get_future();
  }
};

template<typename R,typename C, typename ... Args>
struct T_FnPackageBuilder<R(C::*)(Args...)> : public I_FnPackage, T_FnArgs<R, Args...>, T_FnPackageBase
{
  using Callable = R(C::*)(Args...);
  using FNArgs = T_FnArgs<R, Args...>;
  C* owner;
  Callable fn_;
  T_FnPackageBuilder() {}

  template<typename ... CArgs>
  T_FnPackageBuilder(Callable fn, C* c, CArgs&&... args) :
    FNArgs{ args... }, owner{ c },
    fn_{ fn }
  {}

  template<typename Ret = R, class dummy = typename T_FnReturn<Ret>::PReturn>
  Ret operator()()
  {
    return FNArgs::call(fn_, owner, std::make_index_sequence<sizeof...(Args)>{});
  }

  template<typename Ret = R, class dummy = typename T_FnReturn<Ret>::VoidReturn>
  void operator()()
  {
    FNArgs::call(fn_, owner, std::make_index_sequence<sizeof...(Args)>{});
  }
  void invoke() override
  {
    this->operator()();
  }

  template<typename Ret = R>
  std::future<Ret> get_future()
  {
    return FNArgs::get_future();
  }

};



template <typename T>
struct Package{};

template<typename R, typename... Args>
struct Package<R(*)(Args...)>
{
  std::future<R> prom;
  I_FnPackage* pack;
  ~Package()
  {
    delete pack;
  }
};

template<typename R, typename C, typename... Args>
struct Package<R(C::*)(Args...)>
{
  std::future<R> prom;
  I_FnPackage* pack;
  ~Package()
  {
    delete pack;
  }
};

template<typename T>
using FnPackage = T_FnPackageBuilder<T>;

template<typename T, typename ... TArgs>
Package<T> * MakeFnPackage(T fn, TArgs&&... capture)
{
  FnPackage<T>  * fp = new FnPackage<T>{fn, std::forward<TArgs>(capture)...};

  return new Package<T>{ fp->get_future(), fp };
}



class Scheduler {

  std::queue<I_FnPackage*> queue_;
  std::queue<int> workers_;
  std::vector<std::thread> worker_threads_;
  std::condition_variable work_signal;
  std::mutex mu_;
  bool isStopped;


  void thread_main_loop();

public:

  Scheduler(int workers = 2);

  void Start();

  bool Schedule(I_FnPackage* fn);

  ~Scheduler() {};
};

static Scheduler* GetScheduler(int threadCount) 
{
  static Scheduler* s = new Scheduler{threadCount};
  return s;
}

