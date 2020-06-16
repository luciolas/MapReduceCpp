#include "wc.h"
#include <cctype>
#include <functional>
#include <sstream>

bool isLetter(char c)
{
  return isalpha(c);
}


template< typename Pred>
void StringSplit(std::vector<std::string>* output, const std::string& input, Pred pred)
{
  std::stringstream out;
  for (auto& c : input)
  {
    if pred(c)
    {
      out << c;
    }
    else
    {
      out.seekg(0, out.end);
      int length = out.tellg;
      out.seekg(0, out.beg);
      if (length > 0) 
      {
        output->emplace_back(out.str());
        out = std::stringstream{""};
      }
    }
  }
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
