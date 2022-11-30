#include <unistd.h>
#include <sstream>
#include <vector>
#include <filesystem>

#include "linux_parser.h"

using std::stoi, std::string, std::vector;
namespace fs = std::filesystem;

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  const fs::path proc_dir{kProcDirectory};

  for (auto const& d : fs::directory_iterator{proc_dir}) {
    string filename = d.path().stem();
    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
      int pid = stoi(filename);
      pids.push_back(pid);
    }
  }
  return pids;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
