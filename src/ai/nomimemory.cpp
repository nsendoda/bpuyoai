#include "nomimemory.h"

const std::map<std::string, std::vector<PutType> > NomiMemory::first_const_puts =
{
{"AAAAAA", {PutType(4, ROTATE_180), PutType(1, ROTATE_180), PutType(1, ROTATE_180)}},
{"AAAAAB", {PutType(4, ROTATE_180), PutType(1, ROTATE_180), PutType(3, ROTATE_180)}},
{"AAAABB", {PutType(4, ROTATE_180), PutType(1, ROTATE_180), PutType(5, ROTATE_180)}},
{"AAAABC", {PutType(4, ROTATE_180), PutType(1, ROTATE_180), PutType(5, ROTATE_180)}},

{"AAABAA", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(1, ROTATE_180)}},
{"AAABAB", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(3, ROTATE_180)}},
{"AAABAC", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(3, ROTATE_180)}},
{"AAABBB", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(4, ROTATE_90)}},
{"AAABBC", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(4, ROTATE_0)}},
{"AAABCC", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(5, ROTATE_180)}},
{"AAABCD", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(5, ROTATE_180)}},

{"AABBAA", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(4, ROTATE_180)}},
{"AABBAB", {PutType(4, ROTATE_180), PutType(4, ROTATE_270), PutType(3, ROTATE_180)}},
{"AABBAC", {PutType(4, ROTATE_180), PutType(3, ROTATE_270), PutType(3, ROTATE_180)}},
{"AABBBB", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_180)}},
{"AABBBC", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(4, ROTATE_90)}},
{"AABBCC", {PutType(4, ROTATE_180), PutType(3, ROTATE_270), PutType(5, ROTATE_180)}},
{"AABBCD", {PutType(4, ROTATE_180), PutType(3, ROTATE_270), PutType(5, ROTATE_180)}},

{"AABCAA", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(5, ROTATE_180)}},
{"AABCAB", {PutType(4, ROTATE_180), PutType(4, ROTATE_0), PutType(3, ROTATE_180)}},
{"AABCAC", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_180)}},
{"AABCAD", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(3, ROTATE_180)}},
{"AABCBB", {PutType(4, ROTATE_180), PutType(4, ROTATE_0), PutType(3, ROTATE_270)}},
{"AABCBC", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(5, ROTATE_270)}},
{"AABCBD", {PutType(4, ROTATE_180), PutType(5, ROTATE_0), PutType(4, ROTATE_90)}},
{"AABCCC", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_270)}},
{"AABCCD", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(4, ROTATE_90)}},
{"AABCDD", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(5, ROTATE_180)}},

{"ABAAAA", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(1, ROTATE_180)}},
{"ABAAAB", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(3, ROTATE_180)}},
{"ABAAAC", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(3, ROTATE_180)}},
{"ABAABB", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(4, ROTATE_180)}},
{"ABAABC", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(4, ROTATE_0)}},
{"ABAACC", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(5, ROTATE_180)}},
{"ABAACD", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(5, ROTATE_180)}},

{"ABABAA", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(4, ROTATE_180)}},
{"ABABAB", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(3, ROTATE_180)}},
{"ABABAC", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(3, ROTATE_180)}},
{"ABABBB", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(3, ROTATE_270)}},
{"ABABBC", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(5, ROTATE_0)}},
{"ABABCC", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(5, ROTATE_180)}},
{"ABABCD", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(5, ROTATE_180)}},

{"ABACAA", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(5, ROTATE_180)}},
{"ABACAB", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
{"ABACAC", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
{"ABACAD", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
{"ABACBB", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(6, ROTATE_180)}},
{"ABACBC", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(4, ROTATE_90)}},
{"ABACBD", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(4, ROTATE_90)}},
{"ABACCC", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(6, ROTATE_180)}},
{"ABACCD", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(5, ROTATE_0)}},
{"ABACDD", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(5, ROTATE_0)}},

{"ABBBAA", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(4, ROTATE_180)}},
{"ABBBAB", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(3, ROTATE_0)}},
{"ABBBAC", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(4, ROTATE_0)}},
{"ABBBBB", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(1, ROTATE_180)}},
{"ABBBBC", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(3, ROTATE_180)}},
{"ABBBCC", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(5, ROTATE_180)}},
{"ABBBCD", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(5, ROTATE_180)}},

{"ABBCAA", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(4, ROTATE_180)}},
{"ABBCAB", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(3, ROTATE_0)}},
{"ABBCAC", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(4, ROTATE_90)}},
{"ABBCAD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(4, ROTATE_90)}},
{"ABBCBB", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(5, ROTATE_180)}},
{"ABBCBC", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
{"ABBCBD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
{"ABBCCC", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(6, ROTATE_180)}},
{"ABBCCD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(5, ROTATE_0)}},
{"ABBCDD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(5, ROTATE_180)}},

// Bが下
{"ABCCAA", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(6, ROTATE_180)}},
{"ABCCAB", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_270)}},
{"ABCCAC", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_0)}},
{"ABCCAD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_0)}},
{"ABCCBB", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_90)}},
{"ABCCBC", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(4, ROTATE_90)}},
{"ABCCBD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(4, ROTATE_90)}},
{"ABCCCC", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_180)}},
{"ABCCCD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_180)}},
{"ABCCDD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_270)}},

{"ABCDAA", {PutType(3, ROTATE_90), PutType(2, ROTATE_180), PutType(3, ROTATE_90)}},
{"ABCDAB", {PutType(3, ROTATE_90), PutType(5, ROTATE_180), PutType(4, ROTATE_180)}},
{"ABCDAC", {PutType(3, ROTATE_90), PutType(2, ROTATE_0), PutType(3, ROTATE_0)}},
{"ABCDAD", {PutType(3, ROTATE_90), PutType(2, ROTATE_180), PutType(3, ROTATE_0)}},
{"ABCDBB", {PutType(3, ROTATE_90), PutType(5, ROTATE_180), PutType(3, ROTATE_90)}},
{"ABCDBC", {PutType(3, ROTATE_90), PutType(5, ROTATE_0), PutType(4, ROTATE_0)}},
{"ABCDBD", {PutType(3, ROTATE_90), PutType(5, ROTATE_180), PutType(4, ROTATE_0)}},
{"ABCDCC", {PutType(3, ROTATE_90), PutType(4, ROTATE_270), PutType(3, ROTATE_90)}},
{"ABCDCD", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(4, ROTATE_180)}},
{"ABCDDD", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(3, ROTATE_90)}},
};

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
