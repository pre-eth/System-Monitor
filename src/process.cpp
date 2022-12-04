#include "process.h"

using std::string;
namespace fs = std::filesystem;

#include <iostream>

long Process::Pid() { return pid; }

string Process::ProcFileName() { return StatFileName; }

// based on an algorithm obtained from 
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
void Process::CpuUtilization(long jiffies) {
    std::ifstream file{StatFileName};
    std::stringstream statContents;
    statContents << file.rdbuf();
    file.close();

    // each file has a string of form "(<symbols here>)" so remove & proceed
    statContents.str(std::regex_replace(statContents.str(), StatRegex, ""));

    /*  we need properties 14-17 and 22, so to swallow the numbers we 
        don't need, we will loop and store them in a dummy token */
    string token;
    for (int i = 0; i<12; i++) 
        statContents >> token;
        
    long utime, stime, cutime, cstime;
    statContents >> utime >> stime >> cutime >> cstime;
    
    // only need to calc this once so if it's 0 then set it otherwise continue
    if (!startTime) {
        // swallow next 4 numbers
        for (int i = 16; i<20; i++)
            statContents >> token;
        
        statContents >> startTime;
    }

    // including util of children processes
    long total_time = utime + stime + cutime + cstime;
    
    /*  startTime and jiffies are both in jiffies so to convert to seconds we need
        to subtract then divide by the Hz, which was assigned to ticks in the 
        initializer list of the constructor */
    float elapsedTime = ((float) (jiffies - startTime) / (float) ticks); 
    utilization = 100 * ((float) (total_time / ticks) / elapsedTime);
}

float Process::CpuUtilization() { return utilization; }

void Process::FindCommand() {
    char cmdBuffer[25];
    sprintf(cmdBuffer, CmdlineFile, pid); 
    fs::path cmdPath{cmdBuffer};

    if (fs::exists(cmdPath)) {
        std::ifstream file{cmdPath, std::ios::in};
        std::ostringstream ss;
        if (ss << file.rdbuf())
            command = ss.str();
        file.close(); 
    }
}

string Process::Command() { return command; }

int Process::Ram() { 
    char memBuffer[16] = {0};

    FILE* pipe = popen(RAMCommand.c_str(), "r");

    // after some testing, looks like some /status files don't have 
    // VmSize info so make sure result of popen is not NULL
    if (pipe && fgets(memBuffer, 16, pipe) != NULL) {
        pclose(pipe);
        int memKb = stoi(std::string(memBuffer).substr(3));
        memory = std::ceil(memKb / 1000);   // convert to Mb
    }
    
    return memory; 
}

// get user ID for process then lookup in Processor's passwdMap  
// hashtable so we can cache the user value
int Process::FindUid() {
    std::ifstream file{StatusFileName};
    std::stringstream statusContents;
    statusContents << file.rdbuf();
    file.close();
    std::string contents{statusContents.str()};

    std::smatch m;
    if (std::regex_search(contents, m, UserRegex))
        return stoi(m.str().substr(5));
    
    return 0;     
}

void Process::User(std::string user) { userName = user; }

string Process::User() { return userName; }

long Process::UpTime() { return (long) (startTime / ticks); }

bool Process::operator<(Process const& a) const { 
    if (a.utilization != utilization)
        return a.utilization < utilization;
    else if (a.memory != memory)
        return a.memory < memory;
    return a.startTime < startTime;
}

bool Process::operator==(Process const& a) const {return a.pid == pid;}