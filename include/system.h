#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <stdio.h>
#include <cmath>

#include "processor.h"

class System {
 public:
  System() {
    char commBuffer[64];

    FILE* pipe = popen(basicCommand, "r");
    if (pipe) {
      os = fgets(commBuffer, 64, pipe);
      kernel = fgets(commBuffer, 64, pipe);
    }

    pclose(pipe);
  };
  std::string Kernel();               
  std::string OperatingSystem(); 
  float MemoryUtilization();          
  float Utilization();                   
  long UpTime();                      // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  void RefreshProcessInfo();
  int TotalProcesses();               
  int RunningProcesses();             
     
 private:
  const double kiloToGig{9.5367431640625E-7};
  const char* basicCommand{"hostnamectl | grep -oP \"(?<=Operating System: ).+|(?<=Kernel: Linux ).+\""};
  const char* memCommand{"free -k | grep -oP \"\\d+\" | head -n2"};
  const std::string UptimePath{"/uptime"};
  std::string kernel;
  std::string os;
  Processor cpu;
};

#endif