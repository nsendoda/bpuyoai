#ifndef BPUYOAI_TOWERBASE_H_
#define BPUYOAI_TOWERBASE_H_

#include "../types.h"
#include "../chain.h"

// nomi�ɂ�����^���[�̕]���Ɏg���N���X�Ɣ�r�N���X

struct TowerRate {
	// ���Γ_��2�A���ȏ�ŁA���Γ_�����܂��ĂȂ�
	bool instant_delete;

	int score;
	// �A���t���[���Ɛݒu�t���[���̍��Z
	Frame frame;

	// �⊮�������̘A���̓��_
	int potential_score;
	// �⊮�Ɏg�����Ղ搔
	int potential_needs;
	Frame potential_frame;

	//
	Score fatal_dose;

	PutIndex putindex;

	// potential init
	TowerRate(int potential_score, int potential_needs, Frame potential_frame) :
		potential_score(potential_score),
		potential_needs(potential_needs),
		potential_frame(potential_frame),
		score(0),
		frame(0),
		fatal_dose(10000),
		putindex(-1),
		instant_delete(true) {}

	void SetActual(const Chain& c) {
		score = c.score;
		frame = c.frame;
	}
	void SetPutIndex(PutIndex pi) {
		putindex = pi;
	}
	void SetFatalDose(Score f) {
		fatal_dose = f;
	}
	void SetInstantDelete(bool d) {
		instant_delete = d;
	}
};

class CompareTowerRate {
public:
	// �E�ɋ����̂������悤��
	bool operator()(const TowerRate &a, const TowerRate &b) const {

		// ���ɔ��΂Œv���ʂ𒴂���ꍇ
		if (a.score > a.fatal_dose && b.score > b.fatal_dose) {
			if (a.instant_delete != b.instant_delete) return b.instant_delete;
			return a.frame > b.frame;
		}
		if (b.score > b.fatal_dose) return true;
		if (a.score > a.fatal_dose) return false;

		// ���ݓI�ɒv���ʂ𒴂���ꍇ
		if (a.potential_score > a.fatal_dose && b.potential_score > b.fatal_dose) {
			if (a.instant_delete != b.instant_delete) return b.instant_delete;
			if (a.potential_needs != b.potential_needs) return a.potential_needs > b.potential_needs;
			return a.potential_frame > b.potential_frame;
		}
		if (b.potential_score > b.fatal_dose) return true;
		if (a.potential_score > a.fatal_dose) return false;

		// �������Ή\��
		if (a.instant_delete != b.instant_delete) return b.instant_delete;

		// 
		if (a.potential_score + a.score != b.potential_score + b.score) return a.potential_score + a.score < b.potential_score + b.score;
		return a.frame > b.frame;
	}
};

#endif