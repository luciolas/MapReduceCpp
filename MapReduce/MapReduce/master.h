#pragma once
#include <string>
#include <queue>
#include <mutex>
#include <json.hpp>
#include "../common.h"

using json = nlohmann::json;

struct Config
{
  int reduceTasks;
  int mapTasks;
  std::vector<std::string> files;
};


void to_json(json& j, const KeyValue& p) {
  j = json{ {"Key", p.Key}, {"Value", p.Value}, };
}

void from_json(const json& j, KeyValue& p) {
  j.at("Key").get_to(p.Key);
  j.at("Value").get_to(p.Value);
}


class Master
{
  enum class Phase
  {
    MAPPHASE,
    REDUCEPHASE,
  };
  std::mutex mu_;
  std::condition_variable cond_var_;
  int nReduce;
  int nMap;
  mapFunc mapFunction;
  reduceFunc reduceFunction;
  std::queue<std::string> map_files;
  std::queue<std::string> reduce_files;

  Phase current;

  // common map
  void doMap(const std::string& jobName, const std::string& file,int mapTaskN, int nReduce, mapFunc mapf);

  // common reduce
  void doReduce();
public:
  
  std::string GenerateMapName();
  std::string GenerateReduceName(const std::string& jobName, int mapTaskN, int nReduce);

  void BeginSequential();
  void BeginDistributed();

  void AddAsMap(const std::string& path);
  void AddAsReduce(const std::string& path);

  
};