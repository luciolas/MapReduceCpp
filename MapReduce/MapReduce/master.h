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
#include <concurrent_unordered_map.h>
#include "JobPackage.h"

#define WORKER_START_PORT 5051
#define MAX_WORKERS 512
#define DEFAULT_REDUCE 5
#define MAP_PER_WORKER 1
#define REDUCE_PER_WORKER 1

using grpc::Channel;
using mapreduce_master::MapReduceMaster;
using mapreduce_worker::MapReduceWorker;
using grpc::ServerReader;
using grpc::Status;
using grpc::ServerContext;
using mapreduce_common::EmptyMessage;
using mapreduce_common::JobMessage;
using mapreduce_common::UploadRequest;
using mapreduce_common::MetaData;
using mapreduce_common::Chunk;
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

  concurrent_unordered_map<size_t, JobPackage*> job_list_;
  concurrent_unordered_map<size_t, std::stringstream> chunk_cache_;
  std::unique_ptr<Server> server_;
  concurrent_unordered_map<size_t, WorkerHandle> worker_list_;
  concurrent_queue<WorkerHandle> workers_;
  concurrent_queue<JobPackage*> job_queue_;

  Phase current;
  void DownloadFiles();

  // common map

  // common reduce
  bool _try_work(const JobMessage& job, std::atomic_bool& stop_signal);
  void _poll_jobs(std::atomic_bool& stop_signal);
  size_t _validate_chunks(const std::string& filePath);
  void _string_chunks(const std::string& filePath, const std::string& fileName);
public:
  enum class Mode
  {
    SEQ,
    MT,
    DIST,
  };
  // deprecated
  void doMap(const std::string& jobName, const std::string& file, int mapTaskN, int nReduce, mapFunc mapf);
  void doReduce(const std::string& jobName, const std::string& nextJobName, int mapTaskN, int nReduce, reduceFunc reducef);
  void doMerge(const std::string& jobname, int nMapTask, int nReduce);
  void BeginSequential(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF);
  void BeginDistributed(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF);
  void BeginRPCDistributed(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF);
  // ------------------   deprecated
  std::string GenerateMapName(const std::string& jobName, int mapTaskN, int nReduce);
  std::string GenerateReduceName(const std::string& jobName, int mapTaskN, int nReduce);
  // --------------------------------

  void StopAllRPCs();
  void StartRPCs(size_t nworkers);

  HANDLE StartGateway();

  void AddAsMap(const std::string& path);
  void AddAsReduce(const std::string& path);

  void Init();
  void Start(Mode mode);

  concurrent_queue<WorkerHandle>& GetWorkers() { return workers_; }

  const concurrent_queue<WorkerHandle>& GetWorkers() const { return workers_; }


  // ---------------- GRPC Services ------------------------ //
  Status ReportStatus(ServerContext* ctx, const JobStatus* req, EmptyMessage* reply) override;
  Status Job(ServerContext* ctx, const JobMessage* req, JobStatus* reply) override;
  Status GetStatus(ServerContext* ctx, const JobStatus* req, JobStatus* reply) override;
  Status UploadFile(ServerContext* context, const Chunk* request, JobStatus* response) override;
  Status RequestUploadFile(ServerContext* context, const MetaData* request, JobStatus* response) override ;
  Status StreamFile(ServerContext* ctx, ServerReader< UploadRequest>* reader, JobStatus* reply) override;
  // ---------------- GRPC Services ------------------------ //

  /// Not implemented 
  //Status RequestStreamFile(ServerContext* context, const  mapreduce_common::UploadRequest* request, JobStatus* response);
};


static Master& GetMaster() 
{
  static Master m{};
  return m;
}