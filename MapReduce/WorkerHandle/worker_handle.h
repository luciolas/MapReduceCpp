#pragma once
#include <grpcpp/grpcpp.h>
#include <mapreduceworker.grpc.pb.h>


using grpc::Channel;

using mapreduce_worker::MapReduceWorker;
using WorkerStub = MapReduceWorker::Stub;
using WorkerHandle = std::shared_ptr <std::unique_ptr<WorkerStub>>;

static WorkerHandle NewHandle(const std::string& target)
{
  return std::make_shared<std::unique_ptr<WorkerStub>>(MapReduceWorker::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials())));
}
