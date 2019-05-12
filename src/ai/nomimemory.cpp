#include "nomimemory.h"

const std::function<bool(Field*, FieldIndex, int)> NomiMemory::ComplementTower3Connection[3][6] = {
	{NomiMemory::Complement3_0_1,
	NomiMemory::Complement3_0_2,
	NomiMemory::Complement3_0_3,
	NomiMemory::Complement3_0_4,
	NomiMemory::Complement3_0_5,
	NomiMemory::Complement3_0_6},
		{NomiMemory::Complement3_1_1,
	NomiMemory::Complement3_1_2,
	NomiMemory::Complement3_1_3,
	NomiMemory::Complement3_1_4,
	NomiMemory::Complement3_1_5,
	NomiMemory::Complement3_1_6},
		{NomiMemory::Complement3_2_1,
	NomiMemory::Complement3_2_2,
	NomiMemory::Complement3_2_3,
	NomiMemory::Complement3_2_4,
	NomiMemory::Complement3_2_5,
	NomiMemory::Complement3_2_6},
};

const std::function<bool(Field*, FieldIndex, int)> NomiMemory::ComplementTower2Connection[3][2] = {
	{
		NomiMemory::Complement2_0_1,
		NomiMemory::Complement2_0_2
	},
		{
		NomiMemory::Complement2_1_1,
		NomiMemory::Complement2_1_2
	},
		{
		NomiMemory::Complement2_2_1,
		NomiMemory::Complement2_2_2
	}
};

// o ぷよの位置
// b baseとして渡されるぷよの位置
// x 補完可能なぷよの位置
// . 空白

//----------------------------------- 3連結補完----------------------------------------------------
//-----------------------------------（順タワーからすると）3列目-----------------------------------

// ox.
// o..
// b..
bool NomiMemory::Complement3_0_1(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + 2 * Field::COLUMN + direct;
	if ((*f)[com1] == EMPTY) {
		(*f)[com1] = (*f)[base];
		return true;
	}
	return false;
}

// .x.
// oox
// b..
bool NomiMemory::Complement3_0_2(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + 2 * Field::COLUMN + direct;
	const FieldIndex com2 = base + 1 * Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ...
// box
// .ox
bool NomiMemory::Complement3_0_3(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + 2 * direct;
	const FieldIndex com2 = base - Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// .x.
// .ox
// box
bool NomiMemory::Complement3_0_4(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + 2 * Field::COLUMN + direct;
	const FieldIndex com2 = base +     Field::COLUMN + 2 * direct;
	const FieldIndex com3 = base +                     2 * direct;
	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ...
// o..
// box
bool NomiMemory::Complement3_0_5(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + 2 * direct;
	for (FieldIndex c : {com1}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// xxx.
// boox
bool NomiMemory::Complement3_0_6(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + Field::COLUMN + direct;
	const FieldIndex com2 = base + Field::COLUMN + 2 * direct;
	const FieldIndex com3 = base + 3 * direct;
	const FieldIndex com4 = base + Field::COLUMN;
	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

//-----------------------------------（順タワーからすると）4列目-----------------------------------
// .o.
// .ox
// xbx
bool NomiMemory::Complement3_1_1(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + direct;
	const FieldIndex com3 = base + Field::COLUMN + direct;
	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// .x..
// xoox
// xb..
bool NomiMemory::Complement3_1_2(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN - direct;
	const FieldIndex com3 = base + 2 * Field::COLUMN;
	const FieldIndex com4 = base + Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// .xx.
// xbox
// ..ox
bool NomiMemory::Complement3_1_3(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + Field::COLUMN + direct;
	const FieldIndex com4 = base + 2 * direct;
	const FieldIndex com5 = base - Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2, com3, com4, com5}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// .xox
// xbox
bool NomiMemory::Complement3_1_4(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + 2 * direct;
	const FieldIndex com4 = base + Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// .x..
// xox.
// xbox
bool NomiMemory::Complement3_1_5(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN - direct;
	const FieldIndex com3 = base + 2 * direct;
	const FieldIndex com4 = base + Field::COLUMN + direct;
	const FieldIndex com5 = base + 2 * Field::COLUMN;
	for (FieldIndex c : {com1, com2, com3, com4, com5}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// .xxx
// xboo
bool NomiMemory::Complement3_1_6(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + Field::COLUMN + direct;
	const FieldIndex com4 = base + Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

//-----------------------------------（順タワーからすると）5列目-----------------------------------
// .xo
// .xo
// ..b
bool NomiMemory::Complement3_2_1(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct + Field::COLUMN;
	const FieldIndex com2 = base - direct + 2 * Field::COLUMN;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// .xoo
// .xb.
bool NomiMemory::Complement3_2_2(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base - direct + Field::COLUMN;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// .xoo
// ...b
bool NomiMemory::Complement3_2_3(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - 2 * direct + Field::COLUMN;
	for (FieldIndex c : {com1}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// ...o
// .xbo
bool NomiMemory::Complement3_2_4(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	for (FieldIndex c : {com1}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// .xo.
// .xbo
bool NomiMemory::Complement3_2_5(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base - direct + Field::COLUMN;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// 存在しない。
// ....
// ....
// ..oow
bool NomiMemory::Complement3_2_6(Field* f, FieldIndex base, int direct) {
	return false;
}

//----------------------------------- 2連結補完----------------------------------------------------
//-----------------------------------（順タワーからすると）3列目-----------------------------------

// x...
// ox..
// bx..
bool NomiMemory::Complement2_0_1(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + direct + Field::COLUMN;
	const FieldIndex com2 = base + direct;
	const FieldIndex com3 = base + 2 * Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// xx..
// box.
bool NomiMemory::Complement2_0_2(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + 2 * direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + Field::COLUMN + direct;
	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// .x..
// xox.
// xbx.
bool NomiMemory::Complement2_1_1(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + direct;
	const FieldIndex com2 = base - direct;
	const FieldIndex com3 = base + direct + Field::COLUMN;
	const FieldIndex com4 = base - direct + Field::COLUMN;
	const FieldIndex com5 = base + 2 * Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3, com4, com5}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// .xx.
// xbox
bool NomiMemory::Complement2_1_2(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base + Field::COLUMN;
	const FieldIndex com2 = base - direct;
	const FieldIndex com3 = base + direct + Field::COLUMN;
	const FieldIndex com4 = base + 2 * direct;

	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ..x.
// .xox
// .xbx
// 左に置きたい。
bool NomiMemory::Complement2_2_1(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base - direct + Field::COLUMN;
	const FieldIndex com4 = base + direct;
	const FieldIndex com3 = base + direct + Field::COLUMN;
	const FieldIndex com5 = base + 2 * Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3, com4, com5}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}

// ....
// ..xx
// .xbo
bool NomiMemory::Complement2_2_2(Field* f, FieldIndex base, int direct) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + direct + Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return true;
		}
	}
	return false;
}