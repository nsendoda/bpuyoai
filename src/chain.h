#ifndef BPUYOAI_CHAIN_H_
#define BPUYOAI_CHAIN_H_

#include "types.h"

class Chain {
public:
  int number;
  Score score;
  Frame frame;

  Chain();
  Chain(int number, int score, int frame);
  ~Chain();

  Chain &Sum(const Chain &c) {
    number += c.number;
    score += c.score;
    frame += c.frame;
    return *this;
  }
};

#endif
