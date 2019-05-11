#ifndef BPUYOAI_OJAMATABLE_H_
#define BPUYOAI_OJAMATABLE_H_

#include "types.h"
#include "ojama.h"

#include <deque>

class OjamaTable {
public:

	Ojama pre_ojama;
	int instant_quantity;

	// まだ降らないお邪魔が降るまでのフレームを１減らす。
	// 0になった時、落下できるお邪魔に加算する。
	void Update() {
		if (pre_ojama.rest_drop > 0) {
			pre_ojama.rest_drop--;
			if (pre_ojama.rest_drop == 0) {
				instant_quantity += pre_ojama.quantity;
				pre_ojama.quantity = 0;
			}
		}
	}

	// お邪魔を最大30個減らす。
	// ターンが切り替わった時のお邪魔落下処理と整合性を合わせる。
	int Drop() {
		int ans = 0;
		if (instant_quantity > Ojama::MAX_ONCE) {
			instant_quantity -= Ojama::MAX_ONCE;
			return Ojama::MAX_ONCE;
		}
		return ans;
	}

	int SumOjama() const {
		return instant_quantity + pre_ojama.quantity;
	}
};

#endif