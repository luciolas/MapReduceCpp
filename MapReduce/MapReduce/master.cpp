#include "master.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>


size_t ihash(const std::string& s)
{
  std::hash<std::string> hasher{};
  return hasher(s );
}

void Master::doReduce(const std::string& jobName, int mapTaskN, int nReduce, reduceFunc reducef)
{
  for (int i = 0; i < mapTaskN; i++)
  {
    std::ifstream f;
    auto filename = GenerateReduceName(jobName, i, nReduce);
    f.open(filename, std::ifstream::in);
    if (f.is_open())
    {
      std::string buffers;
      std::unordered_map < std::string, std::vector<std::string>> key_val;
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
      auto outputname = GenerateReduceName("second", 4, 2);
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
      auto rn = ihash (kv.Key) & nReduce;
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
