#ifndef BPUYOAI_CHAINRATE_H_
#define BPUYOAI_CHAINRATE_H_

#include "../chain.h"

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
	

	static bool Compare(const ChainRate& a, const ChainRate& b) {
		if (a.c.score > a.fatal_dose && b.c.score > b.fatal_dose) {
			if (a.required_puyo != b.required_puyo) return a.required_puyo < b.required_puyo;
			if (a.c.frame != b.c.frame) return a.c.frame < b.c.frame;
			if (a.puyoappear_row != b.puyoappear_row) return a.puyoappear_row > b.puyoappear_row;
			else return a.empty_count > b.empty_count;
		}
		if (a.c.score > a.fatal_dose || b.c.score > b.fatal_dose)
			return a.c.score > b.c.score;

		// ˆÈ‰º’v€–¢–
		// 2˜A½ˆÈã“¯m‚Ì‚İscore”äŠr
		if (a.c.number > 1 && b.c.number > 1) {
			if (a.c.score != b.c.score) return a.c.score > b.c.score;
			if (a.required_puyo != b.required_puyo) return a.required_puyo < b.required_puyo;
		}
		//  ‚Ç‚¿‚ç‚©1˜A½‚È‚çA1˜A½‚Í‘I‚Î‚È‚¢
		if (a.c.number != b.c.number) return a.c.number > b.c.number;
		return a.c.frame < b.c.frame;
	};
};

#endif