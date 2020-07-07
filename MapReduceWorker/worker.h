#pragma once
#include <Windows.h>
#include <common.h>
#include <future>
#include <grpcpp/grpcpp.h>
#include "mapreduceworker.grpc.pb.h"
#include "mapreducemaster.grpc.pb.h"
#include "mapreduce_common.grpc.pb.h"
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

using grpc::Channel;
using mapreduce_master::MapReduceMaster;
using mapreduce_worker::MapReduceWorker;
using mapreduce_common::EmptyMessage;
using grpc::Status;
using grpc::ServerContext;
using grpc::Server;
using mapreduce_common::JobMessage;
using mapreduce_master::JobStatus;

class Worker final : public MapReduceWorker::Service
{
  std::shared_ptr<Channel> channel_;
  std::unique_ptr<MapReduceMaster::Stub> report_;
  std::unique_ptr<Server> grpc_server_;
  std::string addr_;
  std::string master_addr_;
  std::promise<void> p_shutdown_;
  bool idle_;
  int id_;
  int port_;
  void doMap(const std::string& jobName, const std::string& file, int mapTaskN, int nReduce, mapFunc mapf);
  void doReduce(const std::string& jobName, int mapTaskN, int nReduce, reduceFunc reducef);
  void doMerge(const std::string& jobname, int nMapTask, int nReduce);
public:
  Worker(std::shared_ptr<Channel> chnl, const std::string& addr);
  Worker(std::shared_ptr<Channel> chnl, const std::string& addr, int id);

  Status Work(ServerContext* serverCtx, const JobMessage * msg, EmptyMessage* reply) override;
  std::string GenerateReduceName(const std::string& jobName, int mapTaskN, int nReduce);
  std::string GenerateMergeName(const std::string& jobName, int mapTaskN, int nReduce);
  Status Shutdown(ServerContext* serverCtx, const EmptyMessage* req, EmptyMessage* reply) override;
  void Report(JobStatus js);
  void Start(int startPort);

  int GetPort() const { return port_; }
  void SetIdle(bool idle) { idle_ = idle; }
};