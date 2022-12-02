#include "process.h"

using std::string;
namespace fs = std::filesystem;

#include <iostream>

long Process::Pid() { return pid; }

void Process::CpuUtilization(long jiffies) {
    std::ifstream file{statBuffer};
    std::stringstream statContents;
    statContents << file.rdbuf();

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
    float elapsedTime = ((float) (jiffies - startTime) / (float) ticks ); 
    utilization = 100 * ((float) (total_time / ticks) / elapsedTime);
}

float Process::CpuUtilization() { return utilization; }

void Process::Command(long pid) {
    char cmdBuffer[25];
    sprintf(cmdBuffer, CmdlineFilename, pid); 
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

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

void Process::User(long pid) {

}

string Process::User() { return userName; }

long Process::UpTime() { return (long) (startTime / ticks); }

bool Process::operator<(Process const& a) const { 
    if (a.utilization != utilization)
        return a.utilization < utilization;
    return a.startTime < startTime;
}

bool Process::operator==(long p) const {return pid == p;}