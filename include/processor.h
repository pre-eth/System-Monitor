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
  Processor() {
    char hzBuffer[5];

    FILE* pipe = popen(tickCommand, "r");

    if (pipe)
      Hz = stoi(std::string(fgets(hzBuffer, 5, pipe)));

    pclose(pipe);
  };
  float Utilization();
  void UpdateJiffies(int u1, int u2);
  long Jiffies();
  long ActiveJiffies();
  long ActiveJiffies(int pid);
  long IdleJiffies();
  std::vector<Process>& Processes();  
  void RefreshProcessInfo();
  int TotalProcesses();               
  int RunningProcesses();             

 private:
  int Hz;
  const char* tickCommand{"grep -oP '(?<=CONFIG_HZ=)\\d+' /boot/config-$(uname -r)"};
  const char* cpuCommand{"cat /proc/stat | head -n1"};
  const char* procRefreshCommand{"cat /proc/stat | tail -n4 | grep -oP \"\\d+\" | head -n3"};
  const std::filesystem::path proc_dir{"/proc/"};
  int prevIdle;
  int prevNonIdle;
  int prevTotal;
  std::array<int, 3> cpuTimes{};
  int total;
  int running;
  long jiffies;
  long activeJiffies;
  long idleJiffies;
  std::vector<int> pids;
  std::vector<Process> processes{};
  void removeDead();
};
#endif