#include "system.h"

using std::string, std::vector, std::array;

string System::Kernel() { return kernel; }

string System::OperatingSystem() { return os; }

float System::MemoryUtilization() { 
    char commBuffer[24];
    string total, used;
    float util;

    FILE* pipe = popen(memCommand, "r");
    if (pipe) {
        total = fgets(commBuffer, 24, pipe);
        used = fgets(commBuffer, 24, pipe);
        util = std::stof(used) / std::stof(total);
    }

    pclose(pipe);
    return (util * 100) / 100;
}

float System::Utilization() { return cpu.Utilization(); }

void System::RefreshUpTime() { 
    char upBuffer[20];

    FILE* pipe = popen("cat /proc/uptime", "r");
    if (pipe) {
        std::istringstream iss(fgets(upBuffer, 20, pipe));
        float time;

        iss >> time;
        upTime = std::ceil(time);
        iss >> time;
        idleTime = std::ceil(time);

        cpu.UpdateJiffies(upTime, idleTime);
    }

    pclose(pipe);
}

long System::UpTime() { return upTime; }

long System::IdleTime() { return idleTime; }

void System::RefreshProcessInfo() { cpu.RefreshProcessInfo(); }

int System::RunningProcesses() { return cpu.RunningProcesses(); }

int System::TotalProcesses() { return cpu.TotalProcesses(); }

vector<Process>& System::Processes() { return cpu.Processes(); }