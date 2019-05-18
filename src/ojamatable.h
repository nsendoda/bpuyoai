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

	// �܂��~��Ȃ����ז����~��܂ł̃t���[�����P���炷�B
	// 0�ɂȂ������A�����ł��邨�ז��ɉ��Z����B
	void Update() {
		if (pre_ojama.rest_drop > 0) {
			pre_ojama.rest_drop--;
			if (pre_ojama.rest_drop == 0) {
				instant_quantity += pre_ojama.quantity;
				pre_ojama.quantity = 0;
			}
		}
	}

	// ���ז����ő�30���炷�B
	// @when �^�[�����؂�ւ�������ɁA���ז����������Ɛ����������킹�邽�߂ɌĂ΂��B
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

	// �Œ�����������ނ��Ԃ�
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