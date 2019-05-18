#ifndef BPUYOAI_NOMIMEMORY_H_
#define BPUYOAI_NOMIMEMORY_H_

#include "../puttype.h"
#include "../field.h"
#include "../types.h"

#include <functional>

class NomiMemory {
public:
	// �Ԃ������͋��ʂ��ĕ⊮�ɕK�v�Ȓi���{�⊮���i�P�j�B
	// �Ⴆ�΁A�⊮����ۂ�1�}�X���ɋ�}�X������ꍇ�͂��̕��⊮���K�v�ƍl����2��Ԃ��B
	// 3->4�A���ɂ���B4�A���ɏo���Ȃ��Ȃ�0��Ԃ��B
	static int Complement3(Field * f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i);

	// 2->3�A���ɂ���B3�A���ɏo���Ȃ��Ȃ�0��Ԃ��B
	static int Complement2(Field * f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i);

	// 1->2�A���ɂ���B2�A���ɏo���Ȃ��Ȃ�0��Ԃ��B
	static int Complement1(Field * f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i);

	static const std::map<std::string, std::vector<PutType> > first_const_puts;

private:
	static int Complement3_0_1(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_0_2(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_0_3(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_0_4(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_0_5(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_0_6(Field* f, FieldIndex base, int direct, bool base_conflict);

	static int Complement3_1_1(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_1_2(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_1_3(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_1_4(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_1_5(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_1_6(Field* f, FieldIndex base, int direct, bool base_conflict);

	static int Complement3_2_1(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_2_2(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_2_3(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_2_4(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_2_5(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement3_2_6(Field* f, FieldIndex base, int direct, bool base_conflict);


	static int Complement2_0_1(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement2_0_2(Field* f, FieldIndex base, int direct, bool base_conflict);

	static int Complement2_1_1(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement2_1_2(Field* f, FieldIndex base, int direct, bool base_conflict);

	static int Complement2_2_1(Field* f, FieldIndex base, int direct, bool base_conflict);
	static int Complement2_2_2(Field* f, FieldIndex base, int direct, bool base_conflict);

	static int Complement1_0_1(Field* f, FieldIndex base, int direct, bool base_conflict);

	static int Complement1_1_1(Field* f, FieldIndex base, int direct, bool base_conflict);

	static int Complement1_2_1(Field* f, FieldIndex base, int direct, bool base_conflict);

};

#endif