#ifndef TYPES_H
#define TYPES_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <memory>
#include <fstream>

using std::string;
using std::wstring;

#include <filesystem>
namespace fs = std::filesystem;

typedef std::shared_ptr<std::wifstream> IfstreamPtr;
typedef std::shared_ptr<std::shared_mutex> MutexPtr;

typedef std::vector<string> StringList;
typedef std::vector<string>::iterator StringListIterator;
typedef std::vector<string>::const_iterator StringListConstIterator;

#endif // TYPES_H
