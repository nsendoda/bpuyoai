#ifndef BPUYOAI_OJAMATABLE_H_
#define BPUYOAI_OJAMATABLE_H_

#include "types.h"
#include "ojama.h"

#include <deque>

class OjamaTable {
public:

	Ojama pre_ojama;
	int instant_quantity;

	OjamaTable() : instant_quantity(0) {}

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
	// @when ターンが切り替わった時に、お邪魔落下処理と整合性を合わせるために呼ばれる。
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

	// 最低限発生する御釣りを返す
	int Change(const Field& field) const {
		int change = 0;
		Row min_ojama_drop = SumOjama() / Field::VISIBLE_COLUMN;
		for (Column c = 1; c <= Field::COLUMN; c++) {
			int rest_capa = Field::AVAILABLE_ROW - (field.GetLowestEmptyRows(c) - 1) - min_ojama_drop;
			if (rest_capa < 0) change += std::abs(rest_capa);
		}
		return change;
	}
};

#endif