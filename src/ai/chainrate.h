#ifndef BPUYOAI_CHAINRATE_H_
#define BPUYOAI_CHAINRATE_H_

#include "../chain.h"

static constexpr int REQUIRED_PENALTY = 200;

class ChainRate {
public:
	Chain c;
	
	int required_puyo;
	Row puyoappear_row;
	int empty_count;

	PutIndex first_pi;
	int fatal_dose;

	ChainRate() : c(), required_puyo(100), first_pi(0), fatal_dose(1000000) {}
	ChainRate(Chain c, int required_puyo, Row puyoappear_row, int empty_count, PutIndex first_pi, int fatal_dose)
		: c(c.number, c.score, c.frame),
		required_puyo(required_puyo),
		puyoappear_row(puyoappear_row),
		empty_count(empty_count),
		fatal_dose(fatal_dose),
		first_pi(first_pi)
	{}

	inline int Rate() const {
		return c.score;
	}
	

	static bool Compare(const ChainRate& a, const ChainRate& b) {
		if (a.c.score > a.fatal_dose && b.c.score > b.fatal_dose) {
			if (a.required_puyo != b.required_puyo) return a.required_puyo < b.required_puyo;
			if (a.c.frame != b.c.frame) return a.c.frame < b.c.frame;
			if (a.puyoappear_row != b.puyoappear_row) return a.puyoappear_row > b.puyoappear_row;
			else return a.empty_count > b.empty_count;
		}
		if (a.c.score > a.fatal_dose || b.c.score > b.fatal_dose)
			return a.c.score > b.c.score;

		// �ȉ��v������
		// 2�A���ȏ㓯�m�̂�score��r
		if (a.c.number > 1 && b.c.number > 1) {
			if (a.Rate() != b.Rate()) return a.Rate() > b.Rate();
			if (a.required_puyo != b.required_puyo) return a.required_puyo < b.required_puyo;
		}
		//  �ǂ��炩1�A���Ȃ�A1�A���͑I�΂Ȃ�
		if (a.c.number != b.c.number) return a.c.number > b.c.number;
		return a.c.frame < b.c.frame;
	};
};

#endif