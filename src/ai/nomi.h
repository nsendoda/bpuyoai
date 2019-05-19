#ifndef BPUYOAI_NOMI_H_
#define BPUYOAI_NOMI_H_

// データベース使用（凍結）
// #define USE_DATABASE

// Debug::Print有効化
// #define DEBUG_PRINT

#define TYPE_CHAIN

#include "ai.h"
#include "../../puyo_ai.h"
#include "bitdatabase.h"
#include "nomithink.h"
#include "../debug.h"

#include "../ojama.h"
#include "../state.h"
#include "../pad.h"
#include "../padsearch.h"
#include "../bpuyopad.h"
#include "../mawashi.h"
#include "../types.h"

#ifdef USE_DATABASE
#include "data/puyocount_bit_raw.h"
#endif

#include <string>
#include <queue>
#include <iostream>
#include <Windows.h>

class Nomi : public AI {
public:

	static const int DATABASE_SIZE = 13;

	Nomi();
	~Nomi() {}


	void Init() override;

	void PreProcess() override;

	void Main() override;

	void Decide();
	void PadDecide();
	void Operate();

	void MakeDatabase(int puyocount);

	int ColorPuyoCount();

private:
	State state;
	State enemy;

	BpuyoPad my_pad;

	Mawashi mawashi;

  std::vector<Pad> pad_orders;
	std::queue<Pad> cancel_que;

	BitDatabase database[DATABASE_SIZE];

	Frame frame_ct;
};

#endif