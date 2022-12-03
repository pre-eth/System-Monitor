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
#include <iostream>

class Process {
 public:
   Process(long id, int ticks) : 
   pid{id}, ticks{ticks} {
      /* write StatFileName now that we have the pid and
         Command and User values can be cached */
      char buffer[32];
      sprintf(buffer, StatFile, pid);
      StatFileName = buffer;

      memset(buffer, 0, sizeof(buffer));
      sprintf(buffer, StatusFile, pid);
      StatusFileName = buffer;

      FindCommand();
   }; 
   long Pid(); 
   std::string ProcFileName();
   int FindUid(); 
   void User(std::string user);                         
   std::string User();  
   void FindCommand();                  
   std::string Command();
   void CpuUtilization(long jiffies);                   
   float CpuUtilization();                  
   std::string Ram();                       // TODO: See src/process.cpp
   long UpTime();                       
   bool operator<(Process const& a) const; 
   bool operator==(Process const& a) const;            

 private:
   const char* StatFile{"/proc/%lu/stat"};
   const char* StatusFile{"/proc/%lu/status"};
   const char* CmdlineFile{"/proc/%lu/cmdline"};
   const char* StatusRegex{"\\t\\d+(?=\\s)"}; 
   std::string UserPrefix{"Uid:"};
   std::string RAMPrefix{"VMSize:"};
   std::regex StatRegex{"\\(.+\\) "};
   std::string StatFileName;
   std::string StatusFileName;
   long pid;
   int ticks;
   long startTime{0};
   float utilization;
   std::string userName{};
   std::string command{"Zombie"}; // if /proc/{pid}/cmdline is empty it's a zombie process
};
#endif