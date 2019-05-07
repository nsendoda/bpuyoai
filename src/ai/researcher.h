#ifndef BPUYOAI_RESEARCHER_H_
#define BPUYOAI_RESEARCHER_H_

#include "ai.h"
#include "../../puyo_ai.h"

#include "../state.h"
#include "../padsearch.h"
#include "../bpuyopad.h"
#include "../debug.h"
#include "../mawashistate.h"

class Researcher : public AI {
public:
	Researcher() = default;
	~Researcher() = default;

	void Init() override;

	void PreProcess() override;

	void Main() override;
};

#endif