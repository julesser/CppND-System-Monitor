#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

  // DONE: Declare any necessary private members
 private:
  float prevTotal_ = 0.0;
  float prevIdle_ = 0.0;
};

#endif