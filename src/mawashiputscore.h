#ifndef BPUYOAI_MAWASHIPUTSCORE_H_
#define BPUYOAI_MAWASHIPUTSCORE_H_

#include "puttype.h"

class MawashiPutScore {
public:
	MawashiPutScore() : row_3(100), row_2(100), row_4(100), rest_ojama(1000), frame(-1), rest_field_empty_count(0) {}
	MawashiPutScore(int r3, int r2, int r4, int rest_ojama, int frame, int rest_field_empty_count, PutType p)
		:
		row_3(r3),
		row_2(r2),
		row_4(r4),
		rest_ojama(rest_ojama),
		frame(frame),
		rest_field_empty_count(rest_field_empty_count),
		p(p) {}

	// 3列目の高さとデスツインが作られる高さで、高い列の高さを返す
	int DeadRow() const {
		return std::max(row_3, MinRow24());
	}
	int Row3() const {
		return row_3;
	}
	int MinRow24() const {
		return std::min(row_2, row_4);
	}
	int RestOjama() const {
		return rest_ojama;
	}
	int Frame() const {
		return frame;
	}
	int RestField() const {
		return rest_field_empty_count;
	}
	PutType Put() const {
		return p;
	}	

private:
	int row_3;
	int row_2;
	int row_4;
	int rest_ojama;
	int frame;
	int rest_field_empty_count;
	PutType p;
};



#endif