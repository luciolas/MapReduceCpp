#pragma once
#include <string>
#include <vector>
#include "../common.h"


std::vector<KeyValue> iimapF(const std::string& doc, const std::string& bytes);


std::string iireduceF(const std::string& key, const std::vector<std::string>& values);