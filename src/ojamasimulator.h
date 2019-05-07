#ifndef BPUYOAI_OJAMASIMULATOR_H_
#define BPUYOAI_OJAMASIMULATOR_H_

#include "field.h"

class OjamaSimulator {
public:

	static const int DROPONCE_OJAMA = 30;

	// お邪魔を落とせる分だけ（最大30個）引数のFieldに落とす。
	// この際、列数6の余剰である1列に満たないお邪魔ぷよは都合の良い様に落ちる。
	// つまり、6,1,5,2,4,3のように。
	// @ret お釣りを含んだ残りお邪魔量
	static int DropOnceRoughly(Field* field_, int ojama_);

private:
	// [fraction][column] = 1 or 0
	static const int convinient_drop_table[Field::COLUMN - 1][Field::COLUMN];

};



#endif