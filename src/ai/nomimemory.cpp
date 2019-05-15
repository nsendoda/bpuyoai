#include "nomimemory.h"

int NomiMemory::Complement3(Field* f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i) {

	if (c_i == 0) {
		if (shape_i == 0) return Complement3_0_1(f, base, direct, base_conflict);
		if (shape_i == 1) return Complement3_0_2(f, base, direct, base_conflict);
		if (shape_i == 2) return Complement3_0_3(f, base, direct, base_conflict);
		if (shape_i == 3) return Complement3_0_4(f, base, direct, base_conflict);
		if (shape_i == 4) return Complement3_0_5(f, base, direct, base_conflict);
		if (shape_i == 5) return Complement3_0_6(f, base, direct, base_conflict);
	}
	if (c_i == 1) {
		if (shape_i == 0) return Complement3_1_1(f, base, direct, base_conflict);
		if (shape_i == 1) return Complement3_1_2(f, base, direct, base_conflict);
		if (shape_i == 2) return Complement3_1_3(f, base, direct, base_conflict);
		if (shape_i == 3) return Complement3_1_4(f, base, direct, base_conflict);
		if (shape_i == 4) return Complement3_1_5(f, base, direct, base_conflict);
		if (shape_i == 5) return Complement3_1_6(f, base, direct, base_conflict);
	}
	if (c_i == 2) {
		if (shape_i == 0) return Complement3_2_1(f, base, direct, base_conflict);
		if (shape_i == 1) return Complement3_2_2(f, base, direct, base_conflict);
		if (shape_i == 2) return Complement3_2_3(f, base, direct, base_conflict);
		if (shape_i == 3) return Complement3_2_4(f, base, direct, base_conflict);
		if (shape_i == 4) return Complement3_2_5(f, base, direct, base_conflict);
		if (shape_i == 5) return Complement3_2_6(f, base, direct, base_conflict);
	}
	return 0;
}

int NomiMemory::Complement2(Field* f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i) {

	if (c_i == 0) {
		if (shape_i == 0) return Complement2_0_1(f, base, direct, base_conflict);
		if (shape_i == 1) return Complement2_0_2(f, base, direct, base_conflict);
	}
	if (c_i == 1) {
		if (shape_i == 0) return Complement2_1_1(f, base, direct, base_conflict);
		if (shape_i == 1) return Complement2_1_2(f, base, direct, base_conflict);
	}
	if (c_i == 2) {
		if (shape_i == 0) return Complement2_2_1(f, base, direct, base_conflict);
		if (shape_i == 1) return Complement2_2_2(f, base, direct, base_conflict);
	}
	return 0;
}

int NomiMemory::Complement1(Field* f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i) {
	if (c_i == 0) {
		return Complement1_0_1(f, base, direct, base_conflict);
	}
	if (c_i == 1) {
		return Complement1_1_1(f, base, direct, base_conflict);
	}
	if (c_i == 2) {
		return Complement1_2_1(f, base, direct, base_conflict);
	}
	return 0;
}

// o ぷよの位置
// b baseとして渡されるぷよの位置
// x 補完可能なぷよの位置
// . 空白

//----------------------------------- 3連結補完----------------------------------------------------
//-----------------------------------（順タワーからすると）3列目-----------------------------------

// ox.
// o..
// b..
int NomiMemory::Complement3_0_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + 2 * Field::COLUMN + direct;
	if ((*f)[com1] == EMPTY) {
		(*f)[com1] = (*f)[base];
		return com1 / Field::COLUMN - f->GetLowestEmptyRows(com1%Field::COLUMN) + 1;
	}
	return 0;
}

// .x.
// oox
// b..
int NomiMemory::Complement3_0_2(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + 2 * Field::COLUMN + direct;
	const FieldIndex com2 = base + 1 * Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ...
// box
// .ox
int NomiMemory::Complement3_0_3(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + 2 * direct;
	const FieldIndex com2 = base - Field::COLUMN + 2 * direct;

	std::initializer_list<FieldIndex> list;
	if (base_conflict) list = { com1 };
	else list = { com1, com2 };

	for (FieldIndex c : list) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// .x.
// .ox
// box
int NomiMemory::Complement3_0_4(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + 2 * Field::COLUMN + direct;
	const FieldIndex com2 = base +     Field::COLUMN + 2 * direct;
	const FieldIndex com3 = base +                     2 * direct;
	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ...
// o..
// box
int NomiMemory::Complement3_0_5(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + 2 * direct;
	for (FieldIndex c : {com1}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// xxx.
// boox
int NomiMemory::Complement3_0_6(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + Field::COLUMN + direct;
	const FieldIndex com2 = base + Field::COLUMN + 2 * direct;
	const FieldIndex com3 = base + 3 * direct;
	const FieldIndex com4 = base + Field::COLUMN;
	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

//-----------------------------------（順タワーからすると）4列目-----------------------------------
// .o.
// .ox
// xbx
int NomiMemory::Complement3_1_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + direct;
	const FieldIndex com3 = base + Field::COLUMN + direct;

	std::initializer_list<FieldIndex> list;
	if (base_conflict) list = { com3 };
	else list = { com1, com2, com3 };

	for (FieldIndex c : list) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// .x..
// xoox
// xb..
int NomiMemory::Complement3_1_2(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN - direct;
	const FieldIndex com3 = base + 2 * Field::COLUMN;
	const FieldIndex com4 = base + Field::COLUMN + 2 * direct;

	std::initializer_list<FieldIndex> list;
	if (base_conflict) list = { com2, com3, com4 };
	else list = { com1, com2, com3, com4 };

	for (FieldIndex c : list) {

		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// .xx.
// xbox
// ..ox
int NomiMemory::Complement3_1_3(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + Field::COLUMN + direct;
	const FieldIndex com4 = base + 2 * direct;
	const FieldIndex com5 = base - Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2, com3, com4, com5}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// .xox
// xbox
int NomiMemory::Complement3_1_4(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + 2 * direct;
	const FieldIndex com4 = base + Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// .x..
// xox.
// xbox
int NomiMemory::Complement3_1_5(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN - direct;
	const FieldIndex com3 = base + 2 * direct;
	const FieldIndex com4 = base + Field::COLUMN + direct;
	const FieldIndex com5 = base + 2 * Field::COLUMN;
	for (FieldIndex c : {com1, com2, com3, com4, com5}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// .xxx
// xboo
int NomiMemory::Complement3_1_6(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + Field::COLUMN + direct;
	const FieldIndex com4 = base + Field::COLUMN + 2 * direct;
	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

//-----------------------------------（順タワーからすると）5列目-----------------------------------
// .xo
// .xo
// ..b
int NomiMemory::Complement3_2_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct + Field::COLUMN;
	const FieldIndex com2 = base - direct + 2 * Field::COLUMN;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// .xoo
// .xb.
int NomiMemory::Complement3_2_2(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base - direct + Field::COLUMN;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// .xoo
// ...b
int NomiMemory::Complement3_2_3(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - 2 * direct + Field::COLUMN;
	for (FieldIndex c : {com1}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// ...o
// .xbo
int NomiMemory::Complement3_2_4(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	for (FieldIndex c : {com1}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// .xo.
// .xbo
int NomiMemory::Complement3_2_5(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base - direct + Field::COLUMN;
	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// 存在しない。
// ....
// ....
// ..oow
int NomiMemory::Complement3_2_6(Field* f, FieldIndex base, int direct, bool base_conflict) {
	return 0;
}

//----------------------------------- 2連結補完----------------------------------------------------
//-----------------------------------（順タワーからすると）3列目-----------------------------------

// x...
// ox..
// bx..
int NomiMemory::Complement2_0_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + direct + Field::COLUMN;
	const FieldIndex com2 = base + direct;
	const FieldIndex com3 = base + 2 * Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// xx..
// box.
int NomiMemory::Complement2_0_2(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + 2 * direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + Field::COLUMN + direct;
	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// .x..
// xox.
// xbx.
int NomiMemory::Complement2_1_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + direct;
	const FieldIndex com2 = base - direct;
	const FieldIndex com3 = base + direct + Field::COLUMN;
	const FieldIndex com4 = base - direct + Field::COLUMN;
	const FieldIndex com5 = base + 2 * Field::COLUMN;

	std::initializer_list<FieldIndex> list;
	if (base_conflict) list = { com3, com4, com5 };
	else list = { com1, com2, com3, com4, com5 };

	for (FieldIndex c : list) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// .xx.
// xbox
int NomiMemory::Complement2_1_2(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + Field::COLUMN;
	const FieldIndex com2 = base - direct;
	const FieldIndex com3 = base + direct + Field::COLUMN;
	const FieldIndex com4 = base + 2 * direct;

	for (FieldIndex c : {com1, com2, com3, com4}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ..x.
// .xox
// .xbx
// 左に置きたい。
int NomiMemory::Complement2_2_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base - direct + Field::COLUMN;
	const FieldIndex com4 = base + direct;
	const FieldIndex com3 = base + direct + Field::COLUMN;
	const FieldIndex com5 = base + 2 * Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3, com4, com5}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// ....
// ..xx
// .xbo
int NomiMemory::Complement2_2_2(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;
	const FieldIndex com3 = base + direct + Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// x.
// ox
int NomiMemory::Complement1_0_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + direct;
	const FieldIndex com2 = base + Field::COLUMN;

	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}

// .x.
// xox
int NomiMemory::Complement1_1_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base + direct;
	const FieldIndex com2 = base - direct;
	const FieldIndex com3 = base + Field::COLUMN;

	for (FieldIndex c : {com1, com2, com3}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	
	return 0;
}

// ..x
// .xo
int NomiMemory::Complement1_2_1(Field* f, FieldIndex base, int direct, bool base_conflict) {
	const FieldIndex com1 = base - direct;
	const FieldIndex com2 = base + Field::COLUMN;

	for (FieldIndex c : {com1, com2}) {
		if ((*f)[c] == EMPTY) {
			(*f)[c] = (*f)[base];
			return c / Field::COLUMN - f->GetLowestEmptyRows(c%Field::COLUMN) + 1;
		}
	}
	return 0;
}
