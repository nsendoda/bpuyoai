#ifndef BPUYOAI_PUTTYPE_H_
#define BPUYOAI_PUTTYPE_H_

#include "field.h"
#include "rotatetype.h"
#include "types.h"

/// ぷよの置き方を表すクラス.
class PutType {
public:
  // [1, 6] 何列目か
  Column column;
  RotateType rotate;

	PutType();
	PutType(PutIndex i);
	PutType(int column, RotateType rotate);
  ~PutType();

  void SetPutType(int index);

  static PutType GetPutType(int index);

	static PutIndex Convert(const PutType& p);

	static const PutIndex REVERSE_PUTS[PUTTYPE_PATTERN];
};



#endif
