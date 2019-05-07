#ifndef BPUYOAI_SEARCH_H_
#define BPUYOAI_SEARCH_H_

#include "searchresultqueue.h"
#include "state.h"
#include "types.h"

// super class
class Search {
  virtual SearchResultQueue Excute(const State state) const;
};

#endif
