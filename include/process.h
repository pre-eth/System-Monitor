#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

class Process {
 public:
    Process(int id): pid{id} {}; 
    int Pid();                           
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
    const int pid{};
};
#endif