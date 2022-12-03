#include "processor.h"

namespace fs = std::filesystem;

// algorithm obtained from this Stack Overflow post:
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    char commBuffer[64];
    float cpu_util = 0.0f;
    
    FILE* pipe = popen(CpuCommand, "r");
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
    /*  pid's are the names of directories in the /proc/ dir so loop through
        and extract dir names that are only digits */
    for (auto const& d : fs::directory_iterator{ProcDir}) {
        std::string filename = d.path().stem();
        if (std::all_of(filename.begin(), filename.end(), isdigit)) {
            long pid = stol(filename);

            // this bit checks to see if this pid is in the Processes vector
            if (std::none_of(processes.begin(), processes.end(), [pid](Process& proc) {
                return proc.Pid() == pid; 
            })) {
                // pid wasn't found so initialize and add Process
                Process p(pid, Hz);

                // lookup uid in hash table we created to cache /etc/passwd in constructor
                int uid = p.FindUid();
                p.User(passwdMap[uid]);
                
                processes.push_back(p);
            } else
                // process has already been recorded, so skip to next iteration
                continue;
        }
    }

    /*  remove processes no longer in the /proc/ dir that may still be in the processes vector 
        if the process is valid then update its utilization */
    for (long unsigned int i = 0; i < processes.size(); i++) {
        if (fs::exists(processes[i].ProcFileName()))
            processes[i].CpuUtilization(jiffies);
        else 
            processes.erase(processes.begin() + i);
    }
        
    std::sort(processes.begin(), processes.end());

    return processes;
};

void Processor::RefreshProcessInfo() { 
    char procBuffer[10];

    FILE* pipe = popen(ProcRefreshCommand, "r");

    if (pipe) {
        total = stoi(std::string(fgets(procBuffer, 10, pipe)));
        running = stoi(std::string(fgets(procBuffer, 10, pipe)));
    }

    pclose(pipe);
}

int Processor::RunningProcesses() { return running; }

int Processor::TotalProcesses() { return total; }

void Processor::UpdateJiffies(long u1, long u2) {
    jiffies = u1 * Hz;
    idleJiffies = u2 * Hz;
}

long Processor::Jiffies() { return jiffies; }

long Processor::ActiveJiffies() { return jiffies - idleJiffies; };

long Processor::IdleJiffies() { return idleJiffies; };
