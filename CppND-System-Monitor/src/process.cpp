#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
Process::Process(int pid):pid_(pid){
  calculateCpuUsage();
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

float Process::calculateCpuUsage() {
  // read values from filesystem
  long uptime = LinuxParser::UpTime();
  vector<float> val = LinuxParser::CpuUtilization(Pid());
  float totaltime =0;
  // only if the values could be read sucessfully
  if (val.size() == 5) {
    // add utime, stime, cutime, cstime (they are in seconds)
    for(int i=0;i<4;i++)
       totaltime += val[i];
    float seconds = uptime - val[4];
    // calculate the processes CPU usage
    return cpuUsage_ = totaltime / seconds;
  } else
    return cpuUsage_ = 0;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return calculateCpuUsage(); }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
   return cpuUsage_ < a.cpuUsage_;
    }