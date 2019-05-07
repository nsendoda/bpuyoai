#ifndef BPUYOAI_SEARCH_H_
#define BPUYOAI_SEARCH_H_

#include "types.h"

// super class
class Search {
  virtual PutIndex excute(const Field field, const State state) const;
  
}

#endif
