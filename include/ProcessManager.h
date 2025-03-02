#pragma once
#include <vector>
#include <string>

bool fileManager(const std::string& path, const std::string& findOnly, std::vector<std::string>* strings);
bool lineFormatter(const std::string& line, const std::string& findOnly, std::string* outPath);
