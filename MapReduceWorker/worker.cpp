#include "worker.h"
#include <fstream>
#include <MapReduce_Customs/ii.h>
#include <JSON/single_include/nlohmann/json.hpp>
#include <Scheduler/JobScheduler.h>
#include <Scheduler/DeferScheduleObject.h>

using grpc::ServerBuilder;
using grpc::ClientContext;
using grpc::ServerInterface;
using grpc::StatusCode;
using json = nlohmann::json;

Worker::Worker(std::shared_ptr<Channel> master_chnl, const std::string& addr) :
  report_{ MapReduceMaster::NewStub(master_chnl) }, master_addr_{ addr } ,
  idle_{ true }, p_shutdown_{}
{
}

Worker::Worker(std::shared_ptr<Channel> chnl, const std::string& addr, int id) :
  report_{ MapReduceMaster::NewStub(chnl) }, master_addr_{ addr },
  id_{ id }, idle_{ true }, p_shutdown_{}
{
}

Status Worker::Work(ServerContext* serverCtx, const JobMessage* msg, EmptyMessage* reply)
{
  if (!idle_)
  {
    return Status{ StatusCode::INTERNAL, "working" };
  }
  auto nmap = msg->nmap();
  auto nReduce = msg->nreduce();
  auto phase = msg->phase();
  auto* s =::MapReduce::GetScheduler();
  
  std::vector<std::shared_future<void>> res;
  if (phase == JobMessage::Phase::JobMessage_Phase_MAP)
  {
    for (const auto& input : msg->input())
    {
      res.push_back(s->Schedule(&Worker::doMap, this, "master", input, nmap, nReduce, iimapF));
    }
  }
  else if (phase == JobMessage::Phase::JobMessage_Phase_REDUCE)
  {
    res.push_back(s->Schedule(&Worker::doReduce, this, "master", nmap, nReduce, iireduceF));
  }

  s->Schedule([](Worker* worker, std::vector<std::shared_future<void>> r) {
    for (auto& sp : r)
    {
      sp.wait();
    }
    JobStatus j;
    j.set_status(JobStatus::Status::JobStatus_Status_DONE);
    j.set_id(worker->GetPort());
    worker->SetIdle(true);
    
    worker->Report(j);
    }, this, res);

  SetIdle(false);
  return Status::OK;
}

void Worker::doReduce(const std::string& jobName, int mapTaskN, int nReduce, reduceFunc reducef)
{
  std::unordered_map < std::string, std::vector<std::string>> key_val;
  for (int i = 0; i < mapTaskN; i++)
  {
    std::ifstream f;
    auto filename = GenerateReduceName(jobName, i, nReduce);
    f.open(filename, std::ifstream::in);
    if (f.is_open())
    {
      std::string buffers;
      while (std::getline(f, buffers))
      {
        json j;
        try
        {
          j = json::parse(buffers);
        }
        catch (const std::exception& e)
        {
          continue;
        }
        auto kv = j.get<KeyValue>();
        auto found = key_val.find(kv.Key);
        if (found != key_val.end())
        {
          key_val[kv.Key].emplace_back(kv.Value);
        }
        else
        {
          key_val[kv.Key] = std::vector <std::string>{ kv.Value };
        }
      }
      f.close();
      auto outputname = GenerateReduceName("second", mapTaskN, nReduce);
      std::ofstream of{ outputname, std::ofstream::out };
      for (const auto& kv : key_val)
      {
        auto newkv = KeyValue{ kv.first,  reducef(kv.first, kv.second) };
        json mj = newkv;
        of << mj << std::endl;
      }
      of.close();
    }
  }

}

void Worker::doMap(const std::string& jobName, const std::string& file, int mapTaskN, int nReduce, mapFunc mapf)
{
  // Read files
  std::ifstream f;
  f.open(file, std::ifstream::in);
  if (f.is_open())
  {
    f.seekg(0, f.end);
    int length = f.tellg();
    f.seekg(0, f.beg);

    char* buffer = new char[length];

    f.read(buffer, length);

    // Put in a buffer
    std::string buffers{ buffer };

    // Call mapf
    auto kv_result = mapf(file, buffers);
    std::unordered_map < size_t, std::ostringstream> output_encodes;
    // loop through the result array is usually an array of keyval pairs
    for (auto& kv : kv_result)
    {
      // Encode each pairs
      auto rn = ihash(kv.Key) % nReduce;
      json output_json = kv;
      auto found = output_encodes.find(rn);
      if (found != output_encodes.end())
      {
        output_encodes[rn] << output_json << std::endl;
      }
      else
      {
        std::ostringstream os(std::ostringstream::ate);
        os << output_json << std::endl;
        output_encodes[rn] = std::move(os);
      }

    }

    // Write files here
    for (auto& kv : output_encodes)
    {
      auto output_name = GenerateReduceName(jobName, mapTaskN, kv.first);
      std::ofstream of{ output_name, std::ofstream::out };
      of << kv.second.str() << std::endl;
      of.close();
    }
  }
}

std::string Worker::GenerateReduceName(const std::string& jobName, int mapTaskN, int nReduce)
{
  std::stringstream ss;
  ss << jobName << "-" << "reduce-" << mapTaskN << "-" << nReduce;
  return ss.str();;
}

Status Worker::Shutdown(ServerContext* serverCtx, const EmptyMessage* req, EmptyMessage* reply)
{
    //std::cout << "GRPC Server shutting down..." << std::endl;
  p_shutdown_.set_value();
  return Status::OK;
}

void Worker::Report(JobStatus js)
{
  ClientContext cliCtx;
  EmptyMessage reply;

  Status s = report_->ReportStatus(&cliCtx, js, &reply);
  if (!s.ok())
  {
    // log rpc error
    // master died or sth...
    printf("Not ok, worker\n");
  } 
}

void Worker::Start(int startPort)
{
  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder sb;
  std::string port = std::to_string(startPort);
  sb.AddListeningPort("localhost:" + port, grpc::InsecureServerCredentials());
  sb.RegisterService(this);
  
  grpc_server_ = sb.BuildAndStart();
  JobStatus j;
  port_ = startPort;
  j.set_id(startPort);
  j.set_status(JobStatus::Status::JobStatus_Status_IDLE);
  Report(j);
  auto* s = MapReduce::GetScheduler();
  
  s->Schedule([&]() {
    grpc_server_->Wait();
    });

  p_shutdown_.get_future().wait();
  grpc_server_->Shutdown();
}
