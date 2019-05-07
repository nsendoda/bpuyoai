#ifndef BPUYOAI_SEARCH_UNTIL_NEXT_H_
#define BPUYOAI_SEARCH_UNTIL_NEXT_H_

#include "types.h"
#include "search.h"
#include "field.h"

class SearchUntilNext : Search {
  PutIndex excute(const Field field, const State state) const;
}

#ifndef
