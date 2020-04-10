#ifndef __MOVE_H__
#define __MOVE_H__
#include <utility>

struct Move {
  std::pair<int,int> start;
  std::pair<int,int> dest;
  bool capture;
  bool causesCheck;
};

#endif
