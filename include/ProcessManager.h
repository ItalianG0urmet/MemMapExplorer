#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <string>
#include <unordered_map>

bool fileManager(const std::string& path, const std::string& findOnly, std::unordered_map<int, std::string> *strings);

#endif //PROCESSMANAGER_H
