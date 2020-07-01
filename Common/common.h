#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "JSON/single_include/nlohmann/json.hpp"

using json = nlohmann::json;


struct Config
{
  int reduceTasks;
  int mapTasks;
  std::vector<std::string> files;
};

struct KeyValue
{
  std::string Key;
  std::string Value;
};

using mapFunc = std::vector<KeyValue>(*)(const std::string& doc, const std::string& bytes);
using reduceFunc = std::string(*)(const std::string&, const std::vector<std::string>&);

void to_json(json& j, const KeyValue& p);
void from_json(const json& j, KeyValue& p);

inline bool isLetter(char c)
{
  return isalpha(c);
}

template< typename Pred>
void StringSplit(std::vector<std::string>* output, const std::string& input, Pred pred)
{
  std::stringstream out;
  for (auto& c : input)
  {
    if (c < 0 || c > 254)
      continue;
    if (pred(c))
    {
      out << c;
    }
    else
    {
      out.seekg(0, out.end);
      int length = out.tellg();
      out.seekg(0, out.beg);
      if (length > 0)
      {
        output->emplace_back(out.str());
        out = std::stringstream{ "" };
      }
    }
  }
}

size_t ihash(const std::string& s);