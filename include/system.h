#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  std::string OperatingSystem();
  std::string Kernel();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  Processor& Cpu();
  std::vector<Process>& Processes();

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif