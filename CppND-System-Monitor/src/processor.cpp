#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::vector<long> cpuValues = LinuxParser::CpuUtilization();
  long idleTime = cpuValues[LinuxParser::kIdle_] + cpuValues[LinuxParser:: kIOwait_];
  long nonIdleTime = cpuValues[LinuxParser::kUser_] + cpuValues[LinuxParser::kNice_] 
                        + cpuValues[LinuxParser::kSystem_] + cpuValues[LinuxParser::kIRQ_]
                         +cpuValues[LinuxParser::kSoftIRQ_] + cpuValues[LinuxParser::kSteal_];
  long long int totalTime = idleTime + nonIdleTime;
  
  return (totalTime - idleTime)/totalTime;
     }