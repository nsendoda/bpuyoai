#ifndef BPUYOAI_SEARCH_UNTIL_NEXT_H_
#define BPUYOAI_SEARCH_UNTIL_NEXT_H_

#include "field.h"
#include "search.h"
#include "searchresult.h"
#include "searchresultqueue.h"
#include "simulator.h"
#include "state.h"
#include "types.h"

#include <iostream>
class SearchUntilNext : Search {
public:
  static SearchResultQueue Excute(const State state, const Score GOAL_SCORE);
};

#endif
