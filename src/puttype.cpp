#include "puttype.h"

const PutIndex PutType::REVERSE_PUTS[PUTTYPE_PATTERN] =
{ 2, 6, 0,
	5, 10, 3, 1,
	9, 14, 7, 4,
	13, 18, 11, 8,
	17, 21, 15, 12,
	20, 19, 16 };


PutType::PutType() {}

PutType::PutType(PutIndex i) {
	SetPutType(i);
}

PutType::PutType(int column, RotateType rotate)
    : column(column), rotate(rotate) {}


void PutType::SetPutType(int index) {
  PutType p = GetPutType(index);
  this->column = p.column;
  rotate = p.rotate;
}

PutType PutType::GetPutType(int index) {
  return PutType(1 + (index + 1) / ROTATE_NUMBER,
                 ALL_INDEXED_ROTATE_TYPE[index]);
}

PutIndex PutType::Convert(const PutType & p)
{
	if (p.column == 1) {
		if (p.rotate == ROTATE_0) return 0;
		if (p.rotate == ROTATE_90) return 1;
		if (p.rotate == ROTATE_180) return 2;
	}
	if (p.column == 6) {
		if (p.rotate == ROTATE_0) return 19;
		if (p.rotate == ROTATE_180) return 20;
		if (p.rotate == ROTATE_270) return 21;
	}
	return (p.column - 1) * 4 - 1 + p.rotate;
}

PutType::~PutType() {}
