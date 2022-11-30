#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <cmath>

#include "process.h"

class Processor { 
 public:
  Processor() {
    char hzBuffer[5];

    FILE* pipe = popen(tickCommand, "r");

    if (pipe) {
      Hz = stoi(std::string(fgets(hzBuffer, 5, pipe)));
    }

    pclose(pipe);
  };
  float Utilization();
  long Jiffies();
  long ActiveJiffies();
  long ActiveJiffies(int pid);
  long IdleJiffies();
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  void RefreshProcessInfo();
  int TotalProcesses();               
  int RunningProcesses();             

 private:
  int Hz;
  const char* tickCommand{"grep -oP '(?<=CONFIG_HZ=)\\d+' /boot/config-$(uname -r)"};
  const char* cpuCommand{"cat /proc/stat | head -n1"};
  const char* procRefreshCommand{"cat /proc/stat | tail -n4 | grep -oP \"\\d+\" | head -n3"};
  int prevIdle{};
  int prevNonIdle{};
  int prevTotal{};
  std::array<int, 3> cpuTimes{};
  int total{};
  int running{};
  std::vector<Process> processes = {};
};
#endif