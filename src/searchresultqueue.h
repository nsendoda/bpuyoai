#ifndef BPUYOAI_SEARCHRESULTQUEUE_H_
#define BPUYOAI_SEARCHRESULTQUEUE_H_

#include "searchresult.h"
#include <algorithm>
#include <functional>
#include <queue>

class SearchResultQueue {
public:
  std::vector<PutIndex> TopPutIndexes() { return searchresults[0].indexes; }
  Chain TopChain() { return searchresults[0].chain; }
  void Push(const SearchResult &a) { 
    searchresults.push_back(a);
    SortFrame();
  }
  bool Empty() { return searchresults.empty(); }
  void SortFrame() {
    std::sort(searchresults.begin(), searchresults.end(),
              [](const SearchResult &a, const SearchResult &b) {
                return a.chain.frame < b.chain.frame;
              });
  }

private:
  std::vector<SearchResult> searchresults;
};
#endif
