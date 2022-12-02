#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <unistd.h>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <regex>
#include <algorithm>
#include <filesystem>
#include <cmath>

#include "process.h"

class Processor { 
 public:
  float Utilization();
  void UpdateJiffies(long u1, long u2);
  long Jiffies();
  long ActiveJiffies();
  long IdleJiffies();
  std::vector<Process>& Processes();   
  void RefreshProcessInfo();
  int TotalProcesses();               
  int RunningProcesses();             

 private:
  const long Hz{sysconf(_SC_CLK_TCK)}; 
  const char* CpuCommand{"cat /proc/stat | head -n1"};
  const char* ProcRefreshCommand{"cat /proc/stat | tail -n4 | grep -oP \"\\d+\" | head -n3"};
  const std::filesystem::path ProcDir{"/proc/"};
  int prevIdle;
  int prevNonIdle;
  int prevTotal;
  std::array<int, 3> cpuTimes{};
  int total;
  int running;
  long jiffies;
  long idleJiffies;
  std::vector<Process> processes{};
  void removeDead();
};
#endif