#include <unistd.h>
#include <cstddef>
#include <set>
#include <iostream>

#include "system.h"

using std::set, std::size_t, std::string, std::vector, std::array;

string System::Kernel() { return kernel; }

string System::OperatingSystem() { return os; }

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

float System::Utilization() { return cpu.Utilization(); }

// TODO: Return the number of seconds since the system started running
long System::UpTime() { return 0; }

void System::RefreshProcessInfo() { cpu.RefreshProcessInfo(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return cpu.RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return cpu.TotalProcesses(); }

vector<Process>& System::Processes() { return cpu.Processes(); }