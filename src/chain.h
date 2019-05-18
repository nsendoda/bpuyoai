#ifndef BPUYOAI_CHAIN_H_
#define BPUYOAI_CHAIN_H_

#include "types.h"

class Chain {
public:
  int number;
  Score score;
  Frame frame;

	Chain() : number(0), score(-1), frame(10000) {}

	Chain(const Chain& c) : number(c.number), score(c.score), frame(c.frame) {}
  Chain(int number, Score score, Frame frame) : number(number), score(score), frame(frame) {}
	~Chain() = default;

  Chain &Sum(const Chain &c) {
    number += c.number;
    score += c.score;
    frame += c.frame;
    return *this;
  }
};

#endif
