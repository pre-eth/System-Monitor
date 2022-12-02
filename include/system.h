#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <sstream>
#include <unistd.h>
#include <cstddef>

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
  void RefreshUpTime();                  
  long UpTime();
  long IdleTime();                      
  std::vector<Process>& Processes();  
  void RefreshProcessInfo();
  int TotalProcesses();               
  int RunningProcesses();             
     
 private:
  const char* basicCommand{"hostnamectl | grep -oP \"(?<=Operating System: ).+|(?<=Kernel: Linux ).+\""};
  const char* memCommand{"free -k | grep -oP \"\\d+\" | head -n2"};
  std::string kernel;
  std::string os;
  long upTime;
  long idleTime;
  Processor cpu;
};

#endif