#ifndef BPUYOAI_CHAINRATE_H_
#define BPUYOAI_CHAINRATE_H_

#include "../chain.h"

class ChainRate {
public:
	Chain c;
	int fatal_dose;
	Row puyoappear_row;
	int empty_count;

	PutIndex first_pi;

	ChainRate() {}
	ChainRate(Chain c, int fatal_dose, Row puyoappear_row, int empty_count, PutIndex first_pi)
		: c(c.number, c.score, c.frame),
		fatal_dose(fatal_dose),
		puyoappear_row(puyoappear_row),
		empty_count(empty_count),
		first_pi(first_pi)
	{}

	bool operator < (const ChainRate& b) const {
//		return this->first_pi < b.first_pi;
		if (this->c.score > this->fatal_dose && b.c.score > b.fatal_dose) {
			if (this->c.frame != b.c.frame) return this->c.frame < b.c.frame;
			if (this->puyoappear_row != b.puyoappear_row) return this->puyoappear_row > b.puyoappear_row;
			else return this->empty_count > b.empty_count;
		}
		if (this->c.score > this->fatal_dose || b.c.score > b.fatal_dose)
			return this->c.score > b.c.score;

		// ˆÈ‰º’vŽ€–¢–ž
		// 2˜A½ˆÈã“¯Žm‚Ì‚Ýscore”äŠr
		if (this->c.number > 1 && b.c.number > 1) {
			if (this->c.score != b.c.score) return this->c.score > b.c.score;
		}
		//  ‚Ç‚¿‚ç‚©1˜A½‚È‚çA1˜A½‚Í‘I‚Î‚È‚¢
		if (this->c.number != b.c.number) return this->c.number > b.c.number;
		 return this->c.frame < b.c.frame;
	}

	static bool Compare(const ChainRate& a, const ChainRate& b) {
		return a < b;
	};
};

#endif