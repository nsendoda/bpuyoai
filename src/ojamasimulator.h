#ifndef BPUYOAI_OJAMASIMULATOR_H_
#define BPUYOAI_OJAMASIMULATOR_H_

#include "field.h"

class OjamaSimulator {
public:

	static const int DROPONCE_OJAMA = 30;

	// ���ז��𗎂Ƃ��镪�����i�ő�30�j������Field�ɗ��Ƃ��B
	// ���̍ہA��6�̗]��ł���1��ɖ����Ȃ����ז��Ղ�͓s���̗ǂ��l�ɗ�����B
	// �܂�A6,1,5,2,4,3�̂悤�ɁB
	// @ret ���ނ���܂񂾎c�肨�ז���
	static int DropOnceRoughly(Field* field_, int ojama_);

private:
	// [fraction][column] = 1 or 0
	static const int convinient_drop_table[Field::COLUMN - 1][Field::COLUMN];

};



#endif