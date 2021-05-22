#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// ######################
// SYSTEM IMPLEMENTATIONS
// ######################

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  float value, memTotal, memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = value;
        }
        if (key == "MemFree:") {
          memFree = value;
        }
      }
    }
  }
  return (memTotal - memFree) / memTotal;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long upTime, idleTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
    return upTime;
  }
  return 0.0;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return 0;
}

// ###################
// CPU IMPLEMENTATIONS
// ###################

// DONE: Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() {
  // declare variables
  string line, key;
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice;
  vector<int> cpuValues;
  // read numbers from file
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >>
             irq >> softirq >> steal >> guest >> guestNice) {
        if (key == "cpu") {
          cpuValues = {user, nice,    system, idle,  iowait,
                       irq,  softirq, steal,  guest, guestNice};
          return cpuValues;
        }
      }
    }
  }
  return cpuValues;
}

// Additional constructor for process specific CPU utilization
float LinuxParser::CpuUtilization(int pid) {
  string line, value;
  int uptime = LinuxParser::UpTime();
  int utime, stime, start_time;
  float cpuUsage = 0.0;

  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + "/" +
                       kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i = 1; i <= 22; i++) {
        linestream >> value;
        if (i == 14) {
          utime = std::stol(value) / sysconf(_SC_CLK_TCK);
        } else if (i == 15) {
          stime = std::stol(value) / sysconf(_SC_CLK_TCK);
        } else if (i == 22) {
          start_time = std::stol(value) / sysconf(_SC_CLK_TCK);
        }
      }

      float total_time = utime + stime;
      float seconds = uptime - (start_time);

      cpuUsage = (total_time / seconds);
    }
  }

  return cpuUsage;
}

// #######################
// PROCESS IMPLEMENTATIONS
// #######################

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      return line;
    }
  }
  return string();
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return std::to_string(value / 1000);
        }
      }
    }
  }
  return string();
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "uID") {
          return value;
        }
      }
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uId = LinuxParser::Uid(pid);
  string line, key, value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (key == uId) {
          return value;
        }
      }
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, key, value;
  int count = 0;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        count++;
        if (count == 22) {
          return (LinuxParser::UpTime() -
                  (std::stol(value) / sysconf(_SC_CLK_TCK)));
        }
      }
    }
  }
  return 0;
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
