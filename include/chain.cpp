#include "chain.h"

Chain::Chain() : number(0), score(-1), frame(1000) {}

Chain::Chain(int number, int score, int frame)
    : number(number), score(score), frame(frame) {}

Chain::~Chain() {}

const int KILL_FRAME_PARAMETER = 3;

bool Chain::GreaterKillerThan(const Chain &comp) const {
  if (score >= Parameter::LOWER_FATAL_DOSE) {
    if (comp.score < Parameter::LOWER_FATAL_DOSE) {
      return true;
    }
    int score_diff = score - comp.score;
    int frame_diff = frame - comp.frame;
    return score_diff - frame_diff * KILL_FRAME_PARAMETER > 0;
  } else {
    if (comp.score >= Parameter::LOWER_FATAL_DOSE) {
      return false;
    }
    return score - Chain::FRAME_PARAMETER * frame +
               Chain::NUMBER_PARAMETER * number * number >
           comp.score - Chain::FRAME_PARAMETER * comp.frame +
               Chain::NUMBER_PARAMETER * comp.number * comp.number;
  }
}
