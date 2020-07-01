#include "common.h"

size_t ihash(const std::string& s)
{
  std::hash<std::string> hasher{};
  return hasher(s);
}


void to_json(json& j, const KeyValue& p) {
  j = json{ { "Key", p.Key },{ "Value", p.Value }, };
}

void from_json(const json& j, KeyValue& p) {
  j.at("Key").get_to(p.Key);
  j.at("Value").get_to(p.Value);
}
