#pragma once
#include <vector>
#include <string>

struct KeyValue
{
  std::string Key;
  std::string Value;
};

using mapFunc = std::vector<KeyValue>(*)(const std::string& doc, const std::string& bytes);
using reduceFunc = std::string(*)(const std::string&, const std::vector<std::string>&);
