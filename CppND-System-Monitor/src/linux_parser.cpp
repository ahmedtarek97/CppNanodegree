#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//  read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os,version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os>> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string key , memTotal ,memFree ,kb ,line;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> memTotal >> kb;
    std::getline(filestream, line);
    //to clear the line stream
    linestream.str(line);
    linestream.clear();
    linestream >> key >> memFree >> kb;
    }
    float memUsed = (stof(memTotal) - stof(memFree))/stof(memTotal);
    return memUsed;
  
   }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line,uptime;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >>uptime;
    
  }
  return std::stol(uptime);
     }

/*// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }*/

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  
 vector<long> cpuValues{};
  string line;
  string key;
  string vuser, vnice, vsystem, vidle, viowait, virq, vsoftirq, vsteal, vguest,vguest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> vuser >> vnice >> vsystem >> vidle >>
             viowait >> virq >> vsoftirq >> vsteal >> vguest >> vguest_nice) {
        if (key == "cpu") {
          cpuValues.push_back(stol(vuser));
          cpuValues.push_back(stol(vnice));
          cpuValues.push_back(stol(vsystem));
          cpuValues.push_back(stol(vidle));
          cpuValues.push_back(stol(viowait));
          cpuValues.push_back(stol(virq));
          cpuValues.push_back(stol(vsoftirq));
          cpuValues.push_back(stol(vsteal));
          cpuValues.push_back(stol(vguest));
          cpuValues.push_back(stol(vguest_nice));
          return cpuValues;
        }
      }
    }
  }
  return {};
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return -1;
   }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return -1;
   }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return "-1";
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line,key,value;
  std::ifstream filestream(kProcDirectory+to_string(pid) +kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return to_string(stoi(value) / 1024);
        }
      }
    }
  }
  return "-5";
  
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {  
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return "-1";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string user, x, uid_read; // uid_read is the uid read from the file
  string uid{LinuxParser::Uid(pid)};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid_read) {
        if (uid_read == uid) {
          return user;
        }
      }
    }
  }
  return "-1";
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 1; i < 23; i++) {
        linestream >> value; // get the the value number 22
      }
      return stol(value) / sysconf(_SC_CLK_TCK); // Return uptime in seconds
    }
  }
return -1;
}


vector<float> LinuxParser::CpuUtilization(int pid) {
  vector<float> cpuValues{};
  string line;
  float time = 0;
  string value;
  std::ifstream filestream(kProcDirectory  + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 1; i <= kStarttime_; i++) {
        linestream >> value;
        // read CPU time spent in user mode - utime #14
        // read CPU time spent in kernel mode - stime #15
        // read Waited for children's CPU time spent in user mode - cutime #16
        // read Waited for children's CPU time spent in kernel mode- cstime #17
        // read the time when process started - starttime #22
        if (i == kUtime_ || i == kStime_ || i == kCutime_ || i == kCstime_ ||
            i == kStarttime_) {
          // read in clock ticks and convert to seconds
          // devide by clock ticks per second
          time = stof(value) / sysconf(_SC_CLK_TCK);
          cpuValues.push_back(time);
        }
      }
    }
  }
  return cpuValues;
}