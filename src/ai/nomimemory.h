#ifndef BPUYOAI_NOMIMEMORY_H_
#define BPUYOAI_NOMIMEMORY_H_

#include "../field.h"
#include "../types.h"

#include <functional>

class NomiMemory {
public:
	// 3->4連結にする。4連結に出来ないならfalseを返す。
	static const std::function<bool(Field*, FieldIndex, int)> ComplementTower3Connection[3][6];
	// 2->3連結にする。3連結に出来ないならfalseを返す。
	static const std::function<bool(Field*, FieldIndex, int)> ComplementTower2Connection[3][2];

private:
	static bool Complement3_0_1(Field* f, FieldIndex base, int direct);
	static bool Complement3_0_2(Field* f, FieldIndex base, int direct);
	static bool Complement3_0_3(Field* f, FieldIndex base, int direct);
	static bool Complement3_0_4(Field* f, FieldIndex base, int direct);
	static bool Complement3_0_5(Field* f, FieldIndex base, int direct);
	static bool Complement3_0_6(Field* f, FieldIndex base, int direct);

	static bool Complement3_1_1(Field* f, FieldIndex base, int direct);
	static bool Complement3_1_2(Field* f, FieldIndex base, int direct);
	static bool Complement3_1_3(Field* f, FieldIndex base, int direct);
	static bool Complement3_1_4(Field* f, FieldIndex base, int direct);
	static bool Complement3_1_5(Field* f, FieldIndex base, int direct);
	static bool Complement3_1_6(Field* f, FieldIndex base, int direct);

	static bool Complement3_2_1(Field* f, FieldIndex base, int direct);
	static bool Complement3_2_2(Field* f, FieldIndex base, int direct);
	static bool Complement3_2_3(Field* f, FieldIndex base, int direct);
	static bool Complement3_2_4(Field* f, FieldIndex base, int direct);
	static bool Complement3_2_5(Field* f, FieldIndex base, int direct);
	static bool Complement3_2_6(Field* f, FieldIndex base, int direct);


	static bool Complement2_0_1(Field* f, FieldIndex base, int direct);
	static bool Complement2_0_2(Field* f, FieldIndex base, int direct);

	static bool Complement2_1_1(Field* f, FieldIndex base, int direct);
	static bool Complement2_1_2(Field* f, FieldIndex base, int direct);

	static bool Complement2_2_1(Field* f, FieldIndex base, int direct);
	static bool Complement2_2_2(Field* f, FieldIndex base, int direct);
};

#endif