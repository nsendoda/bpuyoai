#ifndef BPUYOAI_TOWERRATE_H_
#define BPUYOAI_TOWERRATE_H_

#include "../types.h"
#include "../chain.h"

// nomi�ɂ�����^���[�̕]���Ɏg���N���X�Ɣ�r�N���X

class TowerRate {
public:
	const int FRAME_PENALTY = 5;

	// ���Γ_��2�A���ȏ�ŁA���Γ_�����܂��ĂȂ�
	bool instant_delete;

	int score;
	// 2��ڂ܂ł̘A���t���[���Ɛݒu�t���[���̍��Z
	Frame frame;

	// �⊮�������̘A���̓��_
	int potential_score;
	// �⊮�Ɏg�����Ղ搔
	int potential_needs;
	Frame potential_frame;

	int putindex;

	//
	Score fatal_dose;


	TowerRate operator =(const TowerRate& a)  {
		this->instant_delete = a.instant_delete;
		this->score = a.score;
		this->frame = a.frame;
		this->potential_score = a.potential_score;
		this->potential_needs = a.potential_needs;
		this->potential_frame = a.potential_frame;
		this->putindex = a.putindex;
		this->fatal_dose = a.fatal_dose;
		return *this;
	}
	// potential init
	TowerRate(int potential_score, int potential_needs, Frame potential_frame) :
		potential_score(potential_score),
		potential_needs(potential_needs),
		potential_frame(potential_frame),
		score(0),
		frame(0),
		fatal_dose(10000),
		putindex(-1),
		instant_delete(false) {}

	TowerRate(int potential_score, int potential_needs, Frame potential_frame, Score s, Frame f, Score fd, PutIndex pi, bool indel) :
		potential_score(potential_score),
		potential_needs(potential_needs),
		potential_frame(potential_frame),
		score(s),
		frame(f),
		fatal_dose(fd),
		putindex(pi),
		instant_delete(indel) {}

	int GetPutIndex() const {
		return putindex;
	}

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
	int Rate() const {
		return potential_score + score - frame * FRAME_PENALTY;
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
			if (a.potential_frame > b.potential_frame) return a.potential_frame > b.potential_frame;
			return a.frame > b.frame;
		}
		if (b.potential_score > b.fatal_dose) return true;
		if (a.potential_score > a.fatal_dose) return false;

		// �������Ή\��
		if (a.instant_delete != b.instant_delete) return b.instant_delete;

		// 
		if (a.Rate() != b.Rate()) return a.Rate() < b.Rate();
//		if (a.potential_score + a.score != b.potential_score + b.score) return  a.potential_score + a.score < b.potential_score + b.score;
		if (a.frame != b.frame) return a.frame > b.frame;
		return a.putindex > b.putindex;
	}
};



#endif

