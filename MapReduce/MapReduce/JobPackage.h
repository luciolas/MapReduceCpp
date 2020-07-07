#pragma once
#include <vector>
#include <string>
#include <atomic>
#include <vector>
struct JobPackage
{
  enum class Status : size_t
  {
    IDLE = 0,
    WORKING,
    DONE,
  };

  enum class Phase : size_t
  {
    MAP,
    REDUCE,
    MERGE,
    DONE,
  };
  std::vector <std::string> files;
  size_t id;
  std::pair<std::atomic_size_t, size_t> nreduce_completed;
  std::pair<std::atomic_size_t, size_t> nmap_completed;
  Status status;
  Phase phase;

  JobPackage() : files{}, id{ 0 }, nreduce_completed{  }, nmap_completed{  },  status{ Status::IDLE }, phase{ Phase::MAP }{}
  bool isDone()
  {

  }
  bool Done()
  {
    switch (phase)
    {
    case Phase::MAP:
      return ++nmap_completed.first >= nmap_completed.second;
    case Phase::REDUCE:
      return ++nreduce_completed.first >= nreduce_completed.second;
    default:
      return true;
    }
  }
  Phase NextPhase() { size_t p = (size_t)phase; phase = (Phase)(++p); return phase; }
};