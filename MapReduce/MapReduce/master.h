#pragma once
#include <string>
#include <ppl.h>
#include <concurrent_queue.h>
#include <mutex>
#include <queue>
#include <JSON/single_include/nlohmann/json.hpp>
#include <Windows.h>
#include <common.h>
#include <grpcpp/grpcpp.h>
#include "mapreducemaster.grpc.pb.h"
#include "mapreduceworker.grpc.pb.h"
#include "mapreduce_common.grpc.pb.h"
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include "../WorkerHandle/worker_handle.h"
#include <bitset>
#include <atomic>
#include <future>

#define WORKER_START_PORT 5051
#define MAX_WORKERS 512

using grpc::Channel;
using mapreduce_master::MapReduceMaster;
using mapreduce_worker::MapReduceWorker;
using grpc::Status;
using grpc::ServerContext;
using mapreduce_common::EmptyMessage;
using mapreduce_worker::JobMessage;
using mapreduce_master::JobStatus;
using json = nlohmann::json;
using grpc::Server;

using namespace concurrency;

class Master final : public MapReduceMaster::Service
{
  enum class Phase
  {
    MAPPHASE,
    REDUCEPHASE,
  };
  std::mutex mu_;
  std::condition_variable cond_var_;
  int nReduce;
  std::atomic_size_t nMap;
  mapFunc mapFunction;
  reduceFunc reduceFunction;

  std::unique_ptr<Server> server_;
  concurrent_queue<WorkerHandle> workers_;

  Phase current;
  

  // common map

  // common reduce
public:
  
  
  std::string GenerateMapName(const std::string& jobName, int mapTaskN, int nReduce);
  void doMap(const std::string& jobName, const std::string& file,int mapTaskN, int nReduce, mapFunc mapf);
  std::string GenerateReduceName(const std::string& jobName, int mapTaskN, int nReduce);
  void doReduce(const std::string& jobName, int mapTaskN, int nReduce, reduceFunc reducef);

  void BeginSequential(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF);
  void BeginDistributed(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF);
  void BeginRPCDistributed(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF);
  std::shared_future<void> StartRPCs(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF);
  void StopAllRPCs();

  Status ReportStatus(ServerContext* ctx, const JobStatus* reply, EmptyMessage* req );

  void AddAsMap(const std::string& path);
  void AddAsReduce(const std::string& path);

  void Init();
  void Start();

  concurrent_queue<WorkerHandle>& GetWorkers() { return workers_; }

  const concurrent_queue<WorkerHandle>& GetWorkers() const { return workers_; }

  
};