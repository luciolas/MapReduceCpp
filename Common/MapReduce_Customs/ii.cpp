#include "ii.h"
#include <algorithm>
#include <unordered_map>


std::vector<KeyValue> iimapF(const std::string& doc, const std::string& bytes)
{
  std::vector<std::string> words{};
  std::vector<KeyValue> result{};
  words.reserve(1 << 16);
  StringSplit(&words, bytes, isLetter);
  for (auto& word : words)
  {
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });
    result.emplace_back(KeyValue{word, doc});
  }
  return result;
}

std::string iireduceF(const std::string& key, const std::vector<std::string>& values)
{
  if (values.size()== 0)
  {
    return std::string{};
  }
  std::unordered_map<std::string, int> word_doc{};
  std::string result{};
  
  result.reserve(values.capacity() + 2 * values.size());
  if (word_doc[values[0]] != 1)
  {
    word_doc[values[0]] = 1;
    result += values[0];
  }
  for (int i = 0; i < values.size() - 1; i++)
  {
    if (word_doc[values[i]] != 1)
    {
      word_doc[values[i]] = 1;
      result += ", " + values[i];
    }
  
  }

  
  return std::to_string(values.size()) + ": " + result;
}
