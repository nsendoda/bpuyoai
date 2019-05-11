#ifndef BPUYOAI_NOMITHINK_H_
#define BPUYOAI_NOMITHINK_H_

#include "../state.h"
#include "../field.h"
#include "../simulator.h"
#include "../types.h"

class NomiThink {
	static Score CaluculateFatalDose(const State & state_);
	static bool KillThink(const State & state, Score fatal_dose);
	static PutType Think(const State & state);
	static void LinkCount(const Field & field_);
};

#endif