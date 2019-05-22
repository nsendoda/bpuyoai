#ifndef BPUYOAI_MAWASHI_H_
#define BPUYOAI_MAWASHI_H_

#include "state.h"
#include "bpuyopad.h"
#include "mawashistate.h"
#include "types.h"

class Mawashi {
public:

	const int MAX_MAWASHI_COUNT = 10;

	Mawashi(Frame drop_speed) : mawashistate(drop_speed) {}

	inline void Init(Frame drop_speed_) {
		mawashistate.Init(drop_speed_);
	}

	void Execute(const State& state_, BpuyoPad* bpuyopad_);

	bool DecideAdjustPut(const State& state);
	
	bool ShouldMawashi(const State& state, const State& enemy);


private:
	MawashiState mawashistate;
	int CountMawashi(const State & state) const;
	int CountMinMawashi(const State & state) const;
	int Ceil(int a, int b) const;

};

#endif