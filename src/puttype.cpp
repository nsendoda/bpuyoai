#include "puttype.h"

PutType::PutType() {}

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

PutType::~PutType() {}
