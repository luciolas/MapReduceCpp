#pragma once
#include "../common.h"
#include <cctype>
#include <functional>
#include <sstream>
#include <string>

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

std::vector<KeyValue> mapF(const std::string& doc, const std::string& bytes);


std::string reduceF(const std::string& key, const std::vector<std::string>& values);