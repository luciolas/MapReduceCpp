#include "wc.h"

bool isLetter(char c)
{
  return isalpha(c);
}



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

std::string reduceF(const std::string& key, const std::vector<std::string>& values)
{
  return std::string();
}
