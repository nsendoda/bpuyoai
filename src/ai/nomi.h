#ifndef BPUYOAI_NOMI_H_
#define BPUYOAI_NOMI_H_

#include "ai.h"
#include "../../puyo_ai.h"
#include "bitdatabase.h"
#include "../toml.h"
#include "../debug.h"

#include "../state.h"
#include "../pad.h"
#include "../padsearch.h"
#include "../bpuyopad.h"
#include "../mawashistate.h"
#include "../types.h"

#include "data/puyocount_bit_raw.h"


#include <string>
#include <iostream>
#include <Windows.h>

class Nomi : public AI {
public:

	static const int DATABASE_SIZE = 17;

	Nomi();
	~Nomi() {}


	void Init() override;

	void PreProcess() override;

	void Main() override;

	void Decide();
	bool KillThink(Score fatal_dose);
	void Think();
	void PadDecide();
	void Operate();

	void MakeDatabase(int puyocount);

	int ColorPuyoCount();

private:
	State state;
	State enemy;

	BpuyoPad my_pad;

	MawashiState mawashi_state;

  std::vector<Pad> pad_orders;

	BitDatabase database[DATABASE_SIZE];

	Frame frame_ct;
};

#endif