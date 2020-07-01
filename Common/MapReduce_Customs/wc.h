#pragma once
#include "../common.h"
#include <cctype>
#include <functional>
#include <sstream>
#include <string>


std::vector<KeyValue> mapF(const std::string& doc, const std::string& bytes);


std::string reduceF(const std::string& key, const std::vector<std::string>& values);