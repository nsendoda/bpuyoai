#ifndef BPUYOAI_SEARCHRESULT_H_
#define BPUYOAI_SEARCHRESULT_H_

#include <vector>

#include "chain.h"
#include "types.h"

class SearchResult {
public:
  std::vector<PutIndex> indexes;
  Chain chain;

  SearchResult(std::vector<PutIndex> indexes, Chain chain)
      : indexes(indexes), chain(chain) {}
};

#endif
