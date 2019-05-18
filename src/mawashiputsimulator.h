#ifndef BPUYOAI_MAWASHIPUTSIMULATOR_H_
#define BPUYOAI_MAWASHIPUTSIMULATOR_H_

#include "puttype.h"
#include "mawashiputscore.h"
#include "field.h"
#include "state.h"
#include "ojamasimulator.h"
#include "simulator.h"

class MawashiSimulator {
public:
	static bool CanKarumeruMawashi(const Field & field_);
	static bool CanNormalMawashi(const Field& field_);

	static PutType BestMawashi(const State& state, const int drop_frame);
	
};

#endif