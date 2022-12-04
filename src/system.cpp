#include "system.h"

using std::string, std::vector, std::array;

string System::Kernel() { return kernel; }

string System::OperatingSystem() { return os; }

float System::MemoryUtilization() { 
    char commBuffer[24] = {0};
    string total, used;
    float util;

    FILE* pipe = popen(memCommand, "r");
    if (pipe && fgets(commBuffer, 24, pipe) != NULL) {
        total = commBuffer;
        used = fgets(commBuffer, 24, pipe);
        pclose(pipe);
        util = std::stof(used) / std::stof(total);
    }

    return (util * 100) / 100.0f;
}

float System::Utilization() { return cpu.Utilization(); }

void System::RefreshUpTime() { 
    char upBuffer[20] = {0};

    FILE* pipe = popen("cat /proc/uptime", "r");
    if (pipe && fgets(upBuffer, 20, pipe) != NULL) {
        pclose(pipe);
        std::istringstream iss(upBuffer);
        float time;

        iss >> time;
        upTime = std::ceil(time);
        iss >> time;
        idleTime = std::ceil(time);

        cpu.UpdateJiffies(upTime, idleTime);
    }
}

long System::UpTime() { return upTime; }

long System::IdleTime() { return idleTime; }

void System::RefreshProcessInfo() { cpu.RefreshProcessInfo(); }

int System::RunningProcesses() { return cpu.RunningProcesses(); }

int System::TotalProcesses() { return cpu.TotalProcesses(); }

vector<Process>& System::Processes() { return cpu.Processes(); }