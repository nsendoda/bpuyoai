#ifndef BPUYOAI_CHAINRATE_H_
#define BPUYOAI_CHAINRATE_H_

#include "../chain.h"


class ChainRate {
public:

	static constexpr int REQUIRED_PENALTY = 200;
	static constexpr int FRAME_PENALTY = 5;

	static constexpr int CRISIS_PENALTY = 20;
	static constexpr int LINK2_BONUS = 300;
	static constexpr int LINK3_BONUS = 550;

	static constexpr int EDGE_PENALTY = 600;

	Chain c;
	
	int required_puyo;
	Row puyoappear_row;
	int empty_count;
	int link2_count;
	int link3_count;

	PutIndex first_pi;
	int fatal_dose;

	// どの連結を消したかのIndex
	int connection_i;

	ChainRate() : c(), required_puyo(100), puyoappear_row(0), empty_count(0), link2_count(0), link3_count(0), first_pi(0), fatal_dose(1000000) {}
	ChainRate(Chain c, int required_puyo, Row puyoappear_row, int empty_count, PutIndex first_pi, int fatal_dose, int c_i)
		: c(c.number, c.score, c.frame),
		required_puyo(required_puyo),
		puyoappear_row(puyoappear_row),
		empty_count(empty_count),
		fatal_dose(fatal_dose),
		first_pi(first_pi),
		connection_i(c_i),
		link2_count(0),
		link3_count(0)
	{}

	inline int Rate() const {
		return c.score
			- REQUIRED_PENALTY * required_puyo
			- c.frame * FRAME_PENALTY
			- puyoappear_row * CRISIS_PENALTY
			+ link2_count * LINK2_BONUS
			+ link3_count * LINK3_BONUS
			- ((first_pi <= 6 || first_pi >= 19) ? EDGE_PENALTY : 0);
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

		// 以下致死未満
		// 2連鎖以上同士のみscore比較
		if (a.c.number > 1 && b.c.number > 1) {
			if (a.Rate() != b.Rate()) return a.Rate() > b.Rate();
			if (a.required_puyo != b.required_puyo) return a.required_puyo < b.required_puyo;
		}
		//  どちらか1連鎖なら、1連鎖は選ばない
		if (a.c.number != b.c.number) return a.c.number > b.c.number;
		return a.c.frame < b.c.frame;
	};
};

#endif