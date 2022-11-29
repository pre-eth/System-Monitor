#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  System() {
    char commBuffer[72];

    FILE* pipe = popen(basicCommand, "r");
    if (fgets (commBuffer, 72, pipe) != nullptr) {
      std::string result = commBuffer;
      std::istringstream stream(result);
      stream >> os >> kernel;
    }

    pclose(pipe);

  };
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               
  std::string OperatingSystem();      
  
 private:
  const char* basicCommand{"hostnamectl | grep -oP \"(?<=Operating System: ).+|(?<=Kernel: Linux ).+\""};
  std::string kernel;
  std::string os;
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif