#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <unistd.h>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <cmath>
#include <unordered_map>

#include "process.h"

class Processor { 
 public:
  Processor() {
    /*  store the values in a map after reading one time, and then look 
        up the process Uid's after for O(1) access. This allows us to avoid 
        either reading /etc/passwd over and over, or making too many popen 
        calls, which was causing memory and performance issues */
    std::ifstream file{UserFile};
    std::stringstream stream, lineStream;
    std::string name, pass, uid;

    stream << file.rdbuf();
    for (std::string line; std::getline(stream, line);) {
      lineStream << line;

      std::getline(lineStream, name, ':');
      std::getline(lineStream, pass, ':');
      std::getline(lineStream, uid, ':');
      
      passwdMap[stoi(uid)] = name;

      // only need first 3 (technically 2 but password is unfortunately in the middle)
      // fields so reset stream and skip to next line iteration
      lineStream.clear();
      lineStream.str(std::string());
      continue;
    }
  };
  float Utilization();
  void UpdateJiffies(long u1, long u2);
  long Jiffies();
  long ActiveJiffies();
  long IdleJiffies();
  std::vector<Process>& Processes();   
  std::string ProcessUser(long pid);
  std::string ProcessCommand(long pid);
  void RefreshProcessInfo();
  int TotalProcesses();               
  int RunningProcesses();             

 private:
  const long Hz{sysconf(_SC_CLK_TCK)}; 
  const char* CpuCommand{"cat /proc/stat | head -n1"};
  const char* ProcRefreshCommand{"cat /proc/stat | tail -n4 | grep -oP \"\\d+\" | head -n3"};
  const char* UserFile{"/etc/passwd"};
  const std::filesystem::path ProcDir{"/proc/"};
  int prevIdle;
  int prevNonIdle;
  int prevTotal;
  std::array<int, 3> cpuTimes{};
  int total;
  int running;
  long jiffies;
  long idleJiffies;
  std::unordered_map<int, std::string> passwdMap;
  std::vector<Process> processes{};
};
#endif