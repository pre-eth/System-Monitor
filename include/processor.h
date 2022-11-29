#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <cmath>

class Processor { 
 public:
  float Utilization();
  long Jiffies();
  long ActiveJiffies();
  long ActiveJiffies(int pid);
  long IdleJiffies();

 private:
  const char* cpuCommand{"cat /proc/stat | head -n1"};
  int prevIdle{};
  int prevNonIdle{};
  int prevTotal{};
  std::array<int, 8> cpuTimes{};
};
#endif