#include "processor.h"

namespace fs = std::filesystem;

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

void Processor::RefreshProcesses() {
    pids.clear();

    /*  pid's are the names of directories in the /proc/ dir so loop through
        and extract dir names that are only digits */
    for (auto const& d : fs::directory_iterator{proc_dir}) {
        std::string filename = d.path().stem();
        if (std::all_of(filename.begin(), filename.end(), isdigit)) {
            int pid = stoi(filename);
            pids.push_back(pid);

            // this bit checks to see if this pid is in the Processes vector
            if (std::none_of(processes.begin(), processes.end(), [pid](Process& proc) {
                return proc == pid; 
            }))
                // pid wasn't found so initialize and add Process
                processes.push_back(Process(pid));
            else
                // process has already been recorded, so skip to next iteration
                continue;
        }
    }

    /*  remove processes no longer in the /proc/ dir that may still be in the processes vector */
    processes.erase(std::remove_if(processes.begin(), processes.end(), [&](Process& p) {
        return std::none_of(pids.begin(), pids.end(), [=](int pid){
            return p == pid;
        });
    }));
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

std::vector<Process>& Processor::Processes() {
    RefreshProcesses();

    return processes;
};

int Processor::RunningProcesses() { return running; }

int Processor::TotalProcesses() { return total; }

void Processor::UpdateJiffies(int u1, int u2) {
    jiffies = u1 * Hz;
    idleJiffies = u2 * Hz;
    activeJiffies = jiffies - idleJiffies;
}

long Processor::Jiffies() { return jiffies;}

long Processor::ActiveJiffies() { return activeJiffies; };

long Processor::IdleJiffies() { return idleJiffies; };
