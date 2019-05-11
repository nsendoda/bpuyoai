#ifndef BPUYOAI_OJAMA_H_
#define BPUYOAI_OJAMA_H_

#include "types.h"

class Ojama {
public:

	static constexpr Score ONE_SCORE = 70;
	static constexpr int MAX_ONCE = 30;

	Ojama() {}
	Ojama(int q, Frame r) : quantity(q), rest_drop(r) {}

	inline void Set(int q, Frame r) { quantity = q; rest_drop = r; }

	int quantity;
	Frame rest_drop;
};

#endif