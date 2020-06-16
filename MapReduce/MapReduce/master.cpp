#include "master.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>


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
      auto rn = ihash(kv.Key) & nReduce;
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


size_t ihash(const std::string& s)
{
  std::hash<std::string> hasher{};
  return hasher(s);
}