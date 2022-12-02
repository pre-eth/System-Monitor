#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <regex>

class Process {
 public:
   Process(long id, int ticks): pid{id}, ticks{ticks} {
      /* write buffers now that we have the pid and
         Command and User values can be cached */
      Command(id);

      sprintf(statBuffer, StatFile, pid);
   }; 
   long Pid();      
   void User(long pid);                     
   std::string User();
   void Command(long pid);                      
   std::string Command();
   void CpuUtilization(long jiffies);                   
   float CpuUtilization();                  
   std::string Ram();                       // TODO: See src/process.cpp
   long UpTime();                       
   bool operator<(Process const& a) const; 
   bool operator==(long p) const;            

 private:
   std::regex StatRegex{"\\(.+\\) "};
   const char* StatFile{"/proc/%lu/stat"};
   const char* CmdlineFilename{"/proc/%lu/cmdline"};
   const char* UserIDCommand{"cat /proc/%lu/status | grep -oP \"(?<=Uid:\\t)\\d+(?=\\s)\""};
   const char* UserCommand{"cat /etc/passwd | grep -oP \".+(?=:x:%d)\""};
   char statBuffer[24];
   long pid;
   int ticks;
   long startTime{0};
   float utilization;
   std::string command{"Zombie"}; // if /proc/{pid}/cmdline is empty it's a zombie process
   std::string userName;
};
#endif