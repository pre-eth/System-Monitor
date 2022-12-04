#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <unistd.h>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <regex>

class Process {
 public:
   Process(long id, int ticks) : 
   pid{id}, ticks{ticks} {
      /* write StatFileName now that we have the pid and
         Command and User values can be cached */
      char b1[32] = {0};
      sprintf(b1, StatFile, pid);
      StatFileName = b1;

      char b2[32] = {0};
      sprintf(b2, StatusFile, pid);
      StatusFileName = b2;

      RAMCommand.append(StatusFileName);
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
   int Ram();                       
   long UpTime();                       
   bool operator<(Process const& a) const; 
   bool operator==(Process const& a) const;            

 private:
   const char* StatFile{"/proc/%lu/stat"};
   const char* StatusFile{"/proc/%lu/status"};
   const char* CmdlineFile{"/proc/%lu/cmdline"};
   std::regex UserRegex{"Uid:\\t\\d+(?=\\s)"};
   std::regex StatRegex{"\\(.+\\) "};
   std::string RAMCommand{"grep -oP \"(?<=VmSize:)\\t*\\s*\\s*\\d+(?=\\s)\" "};
   std::string StatFileName;
   std::string StatusFileName;
   long pid{0};
   int ticks;
   long startTime{0};
   float utilization{0.0f};
   int memory{0};
   std::string userName{};
   std::string command{"Zombie"}; // if /proc/{pid}/cmdline is empty it's a zombie process
};
#endif