#include "processor.h"

// algorithm obtained from this Stack Overflow post:
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    char commBuffer[64];
    float cpu_util = 0.0f;
    
    FILE* pipe = popen(cpuCommand, "r");
    if (pipe) {
        std::string result = fgets(commBuffer, 64, pipe);
        std::stringstream res_stream(result.substr(4));
        
        // first three non-idle fields
        res_stream >> cpuTimes[0] >> cpuTimes[1] >> cpuTimes[2];
        int non_idle = cpuTimes[0] + cpuTimes[1] + cpuTimes[2];

        // idle fields
        res_stream >> cpuTimes[0] >> cpuTimes[1];
        int idle = cpuTimes[0] + cpuTimes[1];

        // remaining non-idle fields
        res_stream >> cpuTimes[0] >> cpuTimes[1] >> cpuTimes[2];
        non_idle += cpuTimes[0] + cpuTimes[1];

        int total = idle + non_idle;

        int dTotal = total - prevTotal;
        int dIdle = idle - prevIdle;

        cpu_util = (float) (dTotal - dIdle) / (float) dTotal;

        // store the values for next round of calculations
        prevTotal = total;
        prevIdle = idle;
        prevNonIdle = non_idle;
    }

    pclose(pipe);
    return std::ceil(cpu_util * 100) / 100;
}

std::vector<Process>& Processor::Processes() {
    return processes;
};

void Processor::RefreshProcessInfo() { 
    char procBuffer[10];

    FILE* pipe = popen(procRefreshCommand, "r");

    if (pipe) {
        total = stoi(std::string(fgets(procBuffer, 10, pipe)));
        running = stoi(std::string(fgets(procBuffer, 10, pipe)));
    }

    pclose(pipe);
}

int Processor::RunningProcesses() { return running; }

int Processor::TotalProcesses() { return total; }