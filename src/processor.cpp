#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<int> values = LinuxParser::CpuUtilization();

  float idle = values[3] + values[4];
  float nonIdle =
      values[0] + values[1] + values[2] + values[5] + values[6] + values[7];

  float total = idle + nonIdle;

  float totalDelta = total - prevTotal_;
  float idleDelta = idle - prevIdle_;

  float cpuUsage = (totalDelta - idleDelta) / totalDelta;

  prevTotal_ = total;
  prevIdle_ = idle;

  return cpuUsage;
}