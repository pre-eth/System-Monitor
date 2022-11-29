#include <unistd.h>
#include <cstddef>
#include <set>
#include <iostream>

#include "system.h"

using std::set, std::size_t, std::string, std::vector, std::array;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
string System::Kernel() { return kernel; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    char commBuffer[24];
    string total, used;
    float util;

    FILE* pipe = popen(memCommand, "r");
    if (pipe) {
        string total = fgets(commBuffer, 24, pipe);
        string used = fgets(commBuffer, 24, pipe);
        util = std::stof(used) / std::stof(total);
    }

    pclose(pipe);
    return (util * 100) / 100;
}

// TODO: Return the operating system name
string System::OperatingSystem() { return os; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long System::UpTime() { return 0; }
