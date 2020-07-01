#include "wc.h"



std::vector<KeyValue> mapF(const std::string& doc, const std::string& bytes)
{
  std::vector<std::string> string_array{};
  std::vector<KeyValue> result{};
  StringSplit(&string_array, bytes, isLetter);
  for (auto& word : string_array)
  {
    result.emplace_back(KeyValue{word, "1"});
  }
  return result;
}

std::string reduceF(const std::string&, const std::vector<std::string>& values)
{
  int result=0;
  for (const auto& val : values)
  {
    result += atoi(val.c_str());
  }
  return std::to_string(result);
}
