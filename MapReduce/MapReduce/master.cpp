#include "master.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <functional>
#include <Scheduler/JobScheduler.h>
#include <Windows.h>
#include <tchar.h>
#include <curl/curl.h>
#include <random>
#include <string>
#include <filesystem>

using grpc::ServerBuilder;
using grpc::ClientContext;
using MapReduce::Scheduler;

namespace fs = std::filesystem;

// See Stackoverflow
// https://stackoverflow.com/questions/47977829/generate-a-random-string-in-c11/47979867
std::string random_string()
{
  std::string ALPHA_NUMERIC("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(ALPHA_NUMERIC.begin(), ALPHA_NUMERIC.end(), generator);

  return ALPHA_NUMERIC.substr(0, 16);    // assumes 32 < number of characters in str         
}

size_t random_int()
{
  std::random_device rd;
  std::mt19937 generator(rd());
  return generator();
}

size_t ihash(const std::string& s)
{
  std::hash<std::string> hasher{};
  return hasher(s );
}

void Master::doReduce(const std::string& jobName, const std::string& nextJobName, int mapTaskN, int nReduce, reduceFunc reducef)
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
      auto outputname = GenerateReduceName(nextJobName, mapTaskN, nReduce);
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

void Master::doMerge(const std::string& jobname, int nMapTask, int nReduce)
{
  std::map<std::string, std::string, std::less<std::string>> key_val;
  
  for (int i = 0; i < nReduce; i++)
  {
    auto file_name = GenerateReduceName(jobname, nMapTask, i);
    std::ifstream ifs;
    ifs.open(file_name, std::ifstream::in);
    if (ifs.is_open())
    {
      std::string buf{};
      DEFER([&]() { ifs.close(); });
      while (std::getline(ifs, buf))
      {
        json j;
        try
        {
          j = json::parse(buf);
        }
        catch (const std::exception& e)
        {
          continue;
        }
        auto kv = j.get<KeyValue>();
        key_val.emplace(kv.Key, kv.Value);
      }

      ifs.close();
    }
    else
    {
      // log file unabvle to open
    }
  }
  auto outputname = GenerateReduceName("mergedfile",nMapTask, nReduce);
  std::ofstream of{ outputname, std::ofstream::out };
  DEFER([&]() { of.close(); });
  for (const auto& kv : key_val)
  {
    auto newkv = KeyValue{ kv.first,  kv.second };
    json mj = newkv;
    of << mj << std::endl;
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
    doReduce("master","merge", n_files, i, reduceF);
  }

  doMerge("merge", n_files, nReduce);
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
    waiters.push_back(s->Schedule(&Master::doReduce, this, "master","merge", n_files, i,reduceF));
  }
  for (auto& f : waiters)
  {
    f.wait();
  }

  doMerge("merge", n_files, nReduce);
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
    jb.set_jobname("master");
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
  jb.set_jobname("master");
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
    // TODO: timeout
    while (nmap.get() != expected);
    }, std::ref(nMap), expected);

  wait_reduce_phase.wait();

  doMerge("merge", input_files.size(), nReduce);
  
}

void Master::StartRPCs(size_t nworkers)
{
  size_t len = nworkers;
  WCHAR cmdline[128];
  int port = 5050;
  int worker_port = port + 1;
  LPCWSTR processName = L"MapReduceWorker.exe";
  for (size_t i = 0; i < len; i++)
  {
    ZeroMemory(cmdline, sizeof(cmdline));
    swprintf_s(cmdline, L"%s %d %d %d",L"MapReduceWorker.exe", port, worker_port++, i);
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
      printf("Failed to start worker process (%d)", GetLastError());
      continue;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
}

HANDLE Master::StartGateway()
{
  LPCWSTR processName = L"../../grpc_json_gateway/cmd/cmd.exe";
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  ZeroMemory(&pi, sizeof(pi));
  si.cb = sizeof(si);

  if (!CreateProcess(processName,
    NULL,
    NULL,
    NULL,
    false,
    0,
    NULL,
    NULL,
    &si,
    &pi))
  {
    printf("Failed to start gateway process (%d)", GetLastError());
  }
  CloseHandle(pi.hThread);

  return pi.hProcess;
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
  auto [handle, inserted] = worker_list_.insert({ req->id(), NewHandle("localhost:" + std::to_string(req->id())) });
  auto wrk_handle = handle->second;
  if (req->status() == JobStatus::Status::JobStatus_Status_IDLE)
  {
    workers_.push(handle->second);
    // TODO: remove nMap, not needed.
    ++nMap;
    std::cout << "From worker " << "localhost:" + std::to_string(req->id()) << std::endl;
  }
  else if (req->status() ==  JobStatus::Status::JobStatus_Status_DONE)
  {
    // update jobs
    auto jp = job_list_[req->job_id()];
    if (jp->Done() && jp->NextPhase() != JobPackage::Phase::DONE)
    {
      job_queue_.push(jp);
    }
    workers_.push(wrk_handle);
  } 

  return Status::OK;
}


size_t write_file_bytes(char* buffer, size_t size, size_t nmemb, std::stringstream* userp)
{
  std::stringstream* my_buffer = userp;
  //std::cout << "Buffer: " << *buffer << std::endl;
  my_buffer->write(buffer, size * nmemb);
  return size * nmemb;
}


Status Master::Job(ServerContext* ctx, const JobMessage* req, JobStatus* reply)
{
  auto files = req->input();
  if (files.size() == 0)
  {
    return Status{ grpc::StatusCode::INVALID_ARGUMENT, "no input files" };
  }
  const auto job_id = random_int();

  std::stringstream dir;
  dir << job_id;

  fs::create_directories(dir.str());

  auto easyhandle = curl_easy_init();
  std::vector < std::string > local_files{};
  std::string file_prefix{ "file_" };
  std::string file_ext{ ".mp" };
  int i = 0;
  curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_file_bytes);
  curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, 1L);

  for (const auto& file_location : files)
  {
    std::cout << "Job called: " << file_location << std::endl;
    std::stringstream buffer;
    
    curl_easy_setopt(easyhandle, CURLOPT_URL, file_location.c_str());
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &buffer);

    auto ok = curl_easy_perform(easyhandle);
    if (ok == CURLcode::CURLE_OK)
    {
      std::stringstream filename;
      filename << dir.str() << "/" <<  file_prefix << i << file_ext;
      std::ofstream ofs{ filename.str(), std::ofstream::out };
      ofs << buffer.rdbuf();
      ofs.close();
      ++i;
      local_files.push_back(filename.str());
    }
    else
    {
      std::cout << "Curl error: " << ok << std::endl;
    }
  }

  curl_easy_cleanup(easyhandle);

  auto* jp = new JobPackage{};
  job_list_[job_id] = jp;
  jp->files = std::move(local_files);
  jp->id = job_id;
  jp->nmap_completed.second = jp->files.size();
  jp->nreduce_completed.second = DEFAULT_REDUCE;

  job_queue_.push(jp);
  return Status::OK;
}

Status Master::GetStatus(ServerContext* ctx, const JobStatus* req, JobStatus* reply)
{
  return Status::OK;
}

Status Master::UploadFile(ServerContext* context, const Chunk* request, JobStatus* response)
{
  auto client_md = context->client_metadata();
  auto client_it = client_md.find("x-client-id");
  if (client_it == client_md.end())
  {
    return Status{ grpc::StatusCode::UNAUTHENTICATED, "missing client auth" };
  }
  // Read upload_id
  auto found = chunk_cache_.find((size_t)request->upload_id());
  if (found == chunk_cache_.end())
  {
    return Status{ grpc::StatusCode::NOT_FOUND, std::string{"invalid upload id"} };
  }
  std::stringstream path;
  path << client_it->second << "/" << request->upload_id() << "/";
  auto size = request->chunk().size();
    //Get chunk cache
  if (size == 0)
  {
    std::stringstream metaPath;
    metaPath << path.rdbuf() << "meta";
    std::ifstream ifs{metaPath.str() , std::ifstream::in };
    if (ifs.is_open())
    {
      std::string line[4];
      auto i = 0;
      while (std::getline(ifs, line[i++]));
      auto sch = MapReduce::GetScheduler();
      sch->Schedule(&Master::_string_chunks, this, path.str(), line[2]);
    }
    ifs.close();
  }
  else if (size <= (1 << 16))
  {    
    // TODO: Parts ID/positional chunking
    std::stringstream chunkID;
    chunkID << path.rdbuf();
    chunkID << request->position();
    chunkID << ".chunk";
    std::ofstream ofs{ chunkID.str(), std::ofstream::binary };
    if (ofs.is_open())
    {
      ofs << request->chunk();
    }
    ofs.close();
  }
  else
  {
    return Status{ grpc::StatusCode::RESOURCE_EXHAUSTED, std::string{"chunk over 1MB"} };
  }
  return Status::OK;
}
//-------------------------------------------------------------------------------------//
Status Master::RequestUploadFile(ServerContext* context, const MetaData* request, JobStatus* response)
{
  const auto upload_id = random_int();
  auto client_md = context->client_metadata();
  auto client_it = client_md.find("x-client-id");
  if (client_it == client_md.end())
  {
    return Status{ grpc::StatusCode::UNAUTHENTICATED, "missing client auth" };
  }

  // Create upload session folders
  std::stringstream path;
  path << client_it->second << "/" << upload_id;
  fs::create_directories(path.str());

  std::stringstream meta;
  meta << client_it->second << std::endl << upload_id << std::endl << request->name() << std::endl << request->size();
  std::ofstream ofs{ path.str()  + "/" + "meta", std::ofstream::out };
  ofs << meta.rdbuf();
  ofs.close();

  // chunk cache
  chunk_cache_[upload_id] = std::stringstream{};
  response->set_id(upload_id);
  response->set_job_id(upload_id);
  response->set_status(mapreduce_master::JobStatus_Status::JobStatus_Status_IDLE);
  
  return Status::OK;
}
//-------------------------------------------------------------------------------------//
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
//-------------------------------------------------------------------------------------//
void Master::Start(Mode mode)
{
  auto* s = ::MapReduce::GetScheduler();
  DEFER([&]() {
    this->StopAllRPCs();
    server_->Shutdown();
    });
  std::atomic_bool poller_stop_signal{ false };
  auto grpc_server_wait = s->Schedule([&]()
    {
      if (server_)
      {
        printf("Listening...\n");
        server_->Wait();
        printf("Stop listening...\n");
      }
    });
  // TODO: Config mode: MT, Seq, Dist
  
  auto gw_handle = StartGateway();


  DEFER([&]() { TerminateProcess(gw_handle, 0); });
  auto poller_wait = s->Schedule(&Master::_poll_jobs, this, std::ref(poller_stop_signal));
  if (mode == Mode::DIST)
  {
    StartRPCs(10);
  }
  grpc_server_wait.wait();
  printf("End\n");
}

//-------------------------------------------------------------------------------------//
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
//-------------------------------------------------------------------------------------//
bool Master::_try_work(const JobMessage& job, std::atomic_bool& stop_signal)
{
  ClientContext ctx;
  EmptyMessage empty;
  while (true)
  {
    if (stop_signal.load())
    {
      return true;
    }
    WorkerHandle wrker;
    if (!workers_.try_pop(wrker))
    {
      continue;
    }
    auto status = (*wrker)->Work(&ctx, job, &empty);
    if (status.ok())
    {
      return true;
    }
    else
    {
      workers_.push(wrker);
      std::cout << "Error: " << status.error_code() << " " << status.error_message() << std::endl;
    }
  }
  return false;
}
//-------------------------------------------------------------------------------------//
void Master::_poll_jobs(std::atomic_bool& stop_signal)
{

  while (true)
  {
    //poll jobs
    if (stop_signal.load())
    {
      return;
    }
    JobPackage* jp;
    if (!job_queue_.try_pop(jp))
    {
      continue;
    }

    JobMessage jb;
    std::stringstream id;
    jb.set_nmap(jp->files.size());
    id << jp->id;
    jb.set_jobname(id.str());
    jb.set_phase((JobMessage::Phase)jp->phase);
    jb.set_nreduce(DEFAULT_REDUCE);

    if (jp->phase == JobPackage::Phase::MAP)
    {
      for (size_t i = 0; i < jp->files.size(); ++i)
      {
        jb.add_input(jp->files[i]);
        if (jb.input_size() % MAP_PER_WORKER == 0)
        {
          jb.set_nmap(i + 1 - MAP_PER_WORKER);
          _try_work(jb, stop_signal);
          jb.clear_input();
        }
      }
      if (jb.input_size() > 0)
      {
        jb.set_nmap(jp->files.size() - jb.input_size());
        _try_work(jb, stop_signal);
      }
    }
    else if (jp->phase == JobPackage::Phase::REDUCE)
    {
      for (size_t i = 0; i < DEFAULT_REDUCE; i++)
      {
        jb.set_nreduce(i);
        _try_work(jb, stop_signal);
      }
    }
    else
    {
      _try_work(jb, stop_signal);
    }
  }


}
//-------------------------------------------------------------------------------------//
size_t Master::_validate_chunks(const std::string& filePath)
{

  return size_t();
}
//-------------------------------------------------------------------------------------//
void Master::_string_chunks(const std::string& filesDir, const std::string& outputFileName)
{
  std::map<size_t, std::string> files;
  // Read all .chunk files.
  for (auto& p : fs::directory_iterator(filesDir))
  {
    if (p.path().extension().compare(".chunk") == 0)
    {
      auto fileName = p.path().filename();
      size_t fileNumber = atoi(fileName.string().c_str());
      files.insert(std::make_pair(fileNumber, p.path().string()));
    }
  }

  // Create a file with outputFileName (file name contains the ext)
  std::stringstream outputDir;
  outputDir << filesDir  << outputFileName;
  std::ofstream output{ outputDir.str(), std::ofstream::binary };
  DEFER([&]() { output.close(); });
  // Append them from 0...N
  for (const auto& fit : files)
  {
    const auto& filepath = fit.second;
    std::ifstream chunkFile{ filepath, std::ifstream::binary };
    DEFER([&]() { chunkFile.close(); });
    if (chunkFile.is_open())
    {
      output << chunkFile.rdbuf();
    }
  }
}
//-------------------------------------------------------------------------------------//
std::string Master::GenerateMapName(const std::string& jobName, int mapTaskN, int nReduce)
{
  std::stringstream ss;
  ss << jobName << "-" << mapTaskN << "-" << nReduce << "map";
  return std::string();
}
//-------------------------------------------------------------------------------------//
std::string Master::GenerateReduceName(const std::string& jobName, int mapTaskN, int nReduce)
{
  std::stringstream ss;
  ss << jobName << "-" << "reduce-" << mapTaskN << "-" << nReduce;
  return ss.str();;
}
//-------------------------------------------------------------------------------------//
void to_json(json& j, const KeyValue& p) {
  j = json{ { "Key", p.Key },{ "Value", p.Value }, };
}
//-------------------------------------------------------------------------------------//
void from_json(const json& j, KeyValue& p) {
  j.at("Key").get_to(p.Key);
  j.at("Value").get_to(p.Value);
}

//-------------------------------------------------------------------------------------//

Status Master::StreamFile(ServerContext* ctx, ServerReader< UploadRequest>* reader, JobStatus* reply)
{
  mapreduce_common::UploadRequest upload_request;
  while (reader->Read(&upload_request))
  {
    // Read upload_id
    std::stringstream& ss = chunk_cache_[(size_t)upload_request.upload_id()];
    auto size = upload_request.chunk().chunk().size();
    //Get chunk cache
    if (size == 0)
    {
      // Write file to filesystem
      std::stringstream path;
      path << upload_request.upload_id();
      fs::create_directories(path.str());
      path << "/" << "file_0";
      std::ofstream ofs{ path.str(), std::ofstream::out };
      ofs << ss.rdbuf();
      ofs.close();

      // TODO: Prevent further writing with the same upload_id
      chunk_cache_[upload_request.upload_id()] = std::stringstream{};
    }
    else if (size < (1 << 16))
    {
      // TODO: Parts ID/positional chunking
      ss << upload_request.chunk().chunk();
    }
    else
    {
      return Status{ grpc::StatusCode::RESOURCE_EXHAUSTED, std::string{"chunk over 1MB"} };
    }
  }

  return Status::OK;
}



/////////////////////////// NOT IMPLEMENTED YET//////////////////////////////


void Master::DownloadFiles()
{
  /*auto files = req->input();
  auto easyhandle = curl_easy_init();
  std::stringstream filename;
  filename << "file_";
  int i = 0;
  curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_file_bytes);
  curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, 1L);
  for (const auto& file : files)
  {
    std::cout << "Job called: " << file << std::endl;
    std::stringstream buffer;

    curl_easy_setopt(easyhandle, CURLOPT_URL, file.c_str());
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &buffer);

    auto ok = curl_easy_perform(easyhandle);
    if (ok == CURLcode::CURLE_OK)
    {
      filename << i;
      std::ofstream ofs;
      ofs.open(filename.str(), std::ofstream::out);
      ofs << buffer.rdbuf();
      ofs.close();
      ++i;
    }
    else
    {
      std::cout << "Curl error: " << ok << std::endl;
    }
  }

  curl_easy_cleanup(easyhandle);*/

}
