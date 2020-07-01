#include "master.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <Scheduler/JobScheduler.h>
#include <Windows.h>
#include <tchar.h>

using grpc::ServerBuilder;
using grpc::ClientContext;
using MapReduce::Scheduler;

size_t ihash(const std::string& s)
{
  std::hash<std::string> hasher{};
  return hasher(s );
}

void Master::doReduce(const std::string& jobName, int mapTaskN, int nReduce, reduceFunc reducef)
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

void Master::BeginSequential(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF)
{
  auto n_files = input_files.size();
  int nReduce = 5;
  for (int i = 0; i < n_files; i++)
  {
    doMap("master", input_files[i], i, nReduce, mapf);
  }

  for (int i = 0; i < nReduce; i++)
  {
    doReduce("master", n_files, i, reduceF);
  }
}

void Master::BeginDistributed(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF)
{
  auto n_files = input_files.size();
  int nReduce = 5;
  auto* s = MapReduce::GetScheduler();
  std::vector<std::shared_future<void>> waiters;
  for (int i = 0; i < n_files; i++)
  {
    waiters.push_back(s->Schedule(&Master::doMap, this, "master", input_files[i], i, nReduce, mapf));
  }
  for (auto& f : waiters)
  {
    f.wait();
  }
  waiters.clear();
  for (int i = 0; i < nReduce; i++)
  {
    waiters.push_back(s->Schedule(&Master::doReduce, this, "master", n_files, i,reduceF));
  }
  for (auto& f : waiters)
  {
    f.wait();
  }
}

void Master::BeginRPCDistributed(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF)
{
  nMap = 0;
  nReduce = 10;
  auto* s = MapReduce::GetScheduler();
  size_t i = 0;
  for (const auto& files : input_files)
  {
    ClientContext ctx;
    EmptyMessage empty;
    JobMessage jb;
    jb.add_input(files);
    jb.set_nmap(i);
    jb.set_nreduce(nReduce);
    jb.set_phase(JobMessage::Phase::JobMessage_Phase_MAP);
    // Find available worker

    WorkerHandle wrker;
    while (true)
    {
      if (!workers_.try_pop(wrker))
      {
        continue;
      }
      auto status = (*wrker)->Work(&ctx, jb, &empty);
      if (status.ok())
      {
        ++i;
        break;
      }
      else
      {
        workers_.push(wrker);
        std::cout << "Error: " << status.error_code() << " " << status.error_message() << std::endl;
      }
    }
     
    
  }
  // Reference wrapper need due std::atomic is non-copy-non-move
  // Schedule() removes all reference types in the argument tuple.
  auto wait_map_phase = s->Schedule([](std::reference_wrapper<std::atomic_size_t> nmap, size_t expected) {
    while (nmap.get() != expected);
  }, std::ref(nMap), input_files.size());

  wait_map_phase.wait();
  nMap = 0;
  size_t expected = nReduce;
  JobMessage jb;

  jb.set_nmap(input_files.size());
  jb.set_phase(JobMessage::Phase::JobMessage_Phase_REDUCE);
  for (size_t i = 0; i < nReduce; i++)
  {
    ClientContext ctx;
    EmptyMessage empty;
    jb.set_nreduce(i);

    WorkerHandle wrker;
    while (true)
    {
      if (!workers_.try_pop(wrker))
      {
        continue;
      }
      auto status = (*wrker)->Work(&ctx, jb, &empty);
      if (status.ok())
      {
        // TODO
        break;
      }
      else
      {
        workers_.push(wrker);
        std::cout << "Error: " << status.error_code() << " " << status.error_message() << std::endl;
      }
    }

   
  }

  auto wait_reduce_phase = s->Schedule([](std::reference_wrapper<std::atomic_size_t> nmap, size_t expected) {
    while (nmap.get() != expected);
    }, std::ref(nMap), expected);

  wait_reduce_phase.wait();
}

std::shared_future<void> Master::StartRPCs(const std::vector<std::string>& input_files, mapFunc mapf, reduceFunc reduceF)
{
  size_t len = input_files.size();
  WCHAR cmdline[128];
  int port = 5050;
  int worker_port = port + 1;
  LPCWSTR processName = L"B:\\OpenSource\\luciolas\\c++\\MapReduce\\Debug\\MapReduceWorker.exe";
  for (size_t i = 0; i < len; i++)
  {
    ZeroMemory(cmdline, sizeof(cmdline));
    swprintf_s(cmdline, L"%s %d %d %d","MapReduceWorker.exe", port, worker_port++, i);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    if (!CreateProcess(processName,
      cmdline,
      NULL,
      NULL,
      false,
      0,
      NULL,
      NULL,
      &si,
      &pi))
    {
      printf("Failed to open process (%d)", GetLastError());
      continue;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
  auto* s = MapReduce::GetScheduler();
  auto wait_reduce_phase = s->Schedule([](std::reference_wrapper<std::atomic_size_t> nmap, size_t expected) {
    while (nmap.get() != expected);
    }, std::ref(nMap), len);

  return wait_reduce_phase;
}

void Master::StopAllRPCs()
{
  while (!workers_.empty())
  {
    WorkerHandle wrker;
    workers_.try_pop(wrker);
    ClientContext ctx;
    EmptyMessage emp_msg;
    EmptyMessage reply;
    auto status = (*wrker)->Shutdown(&ctx, emp_msg, &reply);

    if (status.ok())
    {
      std::cout << "Worker shut down" << std::endl;
    }
    else
    {
      std::cout << "Error? " << status.error_code() << " " << status.error_message() << std::endl;
    }
  }
}

Status Master::ReportStatus(ServerContext* ctx, const JobStatus* req, EmptyMessage* reply)
{
  auto ok = JobStatus::Status::JobStatus_Status_IDLE | JobStatus::Status::JobStatus_Status_DONE;
  if (req->status() | ok)
  {
    workers_.push(NewHandle("localhost:" + std::to_string(req->id())));
    ++nMap;
    std::cout << "From worker " << "localhost:" + std::to_string(req->id()) << std::endl;
  }
  return Status::OK;
}

void Master::Init()
{

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder sb;
  int port = 0;
  sb.AddListeningPort("localhost:5050", grpc::InsecureServerCredentials(), &port);
  sb.RegisterService(this);

  server_ = sb.BuildAndStart();
  //std::unique_ptr<Server> server(sb.BuildAndStart());

  std::cout << "Listening to: " << port << std::endl;
}

void Master::Start()
{
  if (server_)
  {
    server_->Wait();
  }
}


void Master::doMap(const std::string& jobName,const std::string& file, int mapTaskN, int nReduce, mapFunc mapf)
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
      auto rn = ihash (kv.Key) % nReduce;
      json output_json = kv;
      auto found = output_encodes.find(rn);
      if (found != output_encodes.end())
      {
        output_encodes[rn] << output_json << std::endl;
      }
      else
      {
        std::ostringstream os (std::ostringstream::ate);
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


std::string Master::GenerateMapName(const std::string& jobName, int mapTaskN, int nReduce)
{
  std::stringstream ss;
  ss << jobName << "-" << mapTaskN << "-" << nReduce << "map";
  return std::string();
}

std::string Master::GenerateReduceName(const std::string& jobName, int mapTaskN, int nReduce)
{
  std::stringstream ss;
  ss << jobName << "-"  << "reduce-" << mapTaskN << "-" << nReduce;
  return ss.str();;
}

void to_json(json& j, const KeyValue& p) {
  j = json{ { "Key", p.Key },{ "Value", p.Value }, };
}

void from_json(const json& j, KeyValue& p) {
  j.at("Key").get_to(p.Key);
  j.at("Value").get_to(p.Value);
}
