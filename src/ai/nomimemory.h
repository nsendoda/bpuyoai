#ifndef BPUYOAI_NOMIMEMORY_H_
#define BPUYOAI_NOMIMEMORY_H_

#include "../puttype.h"
#include "../field.h"
#include "../types.h"

#include <functional>

class NomiMemory {
public:
	// 返す数字は共通して補完に必要な段数＋補完個数（１）。
	// 例えば、補完する際に1マス下に空マスがある場合はその分補完が必要と考えて2を返す。
	// 3->4連結にする。4連結に出来ないなら0を返す。
	static int Complement3(Field * f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i);

	// 2->3連結にする。3連結に出来ないなら0を返す。
	static int Complement2(Field * f, FieldIndex base, int direct, bool base_conflict, int c_i, int shape_i);

	// 1->2連結にする。2連結に出来ないなら0を返す。
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