#include "format.h"

#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int min = int((seconds / (60)) % 60);
  int sec = int(seconds % (60));
  int hrs = int(min / (60 * 60));

  string time = std::to_string(hrs) + ":" + std::to_string(min) + ":" +
                std::to_string(sec);
  return time;
}