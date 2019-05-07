#ifndef BPUYOAI_CHAIN_H_
#define BPUYOAI_CHAIN_H_

#include "types.h"

class Chain {
public:
  int number, score, frame;

  Chain();
  Chain(int number, int score, int frame);
  ~Chain();
  static const int FRAME_PARAMETER = 1;
  static const int NUMBER_PARAMETER = -50;

  bool GreaterKillerThan(const Chain &comp) const;
};

#endif
