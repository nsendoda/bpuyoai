#include "chain.h"

Chain::Chain() : number(0), score(-1), frame(10000) {}

Chain::Chain(int number, int score, int frame)
    : number(number), score(score), frame(frame) {}

Chain::~Chain() {}
