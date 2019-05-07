#pragma once

#include <queue>

#include "Kumipuyo.h"
#include "Field.h"
#include "Kumipuyo.h"
#include "Chain.h"
#include "RotateType.h"
#include "Assert.h"


class Simulator
{
public:
	static Chain Simulate(Field* field_, int parent_index, int child_index, int chain_num = 0);
	static int GetDropRowAndFallAll(Field* field_);
	/// 指定した位置にぷよを置く
	inline static int PutOnePuyoAndGetIndex(Field* field, int i, Color c) {
		ASSERT_MESSAGE(i != 0, "Simulator put error : i = 0")
		int index = field->GetLowestEmptyRowsIndex(i) * Field::COLUMN + i;
		(*field)[index] = c;
		field->AddLowestEmptyRowsIndex(i, 1);
		return index;
	}
	inline static void EliminateOnePuyo(Field* field, int i) {
		ASSERT_MESSAGE(i != 0, "Simulator eliminate error : i = 0")
		field->AddLowestEmptyRowsIndex(i, -1);
		(*field)[field->GetLowestEmptyRowsIndex(i) * Field::COLUMN + i] = Parameter::EMPTY;
	}
private:
	Simulator();
	static void DeleteLink(Field* field_, int i, int chain_num, bool linked[]);
	static int CaluculateLinkCount(const Field& field, int index, bool* linked);
};

