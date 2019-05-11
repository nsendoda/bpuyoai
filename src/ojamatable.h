#ifndef BPUYOAI_OJAMATABLE_H_
#define BPUYOAI_OJAMATABLE_H_

#include "types.h"
#include "ojama.h"

#include <deque>

class OjamaTable {
public:

	Ojama pre_ojama;
	int instant_quantity;

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
	// �^�[�����؂�ւ�������̂��ז����������Ɛ����������킹��B
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