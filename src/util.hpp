#ifndef MY_UTIL
#define MY_UTIL

#include <string>
#include "task.hpp"

task** load(int threshold, std::string filename, int streamlen);

void write(task **results, int streamlen);

#endif