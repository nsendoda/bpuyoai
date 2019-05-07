#ifndef BPUYOAI_PUTTYPE_H_
#define BPUYOAI_PUTTYPE_H_

#include <utility>

#include "field.h"
#include "rotatetype.h"

/// ぷよの置き方を表すクラス.
class PutType {
public:
  int index;
  RotateType rotate;
  PutType();
  PutType(int index, RotateType rotate);
  ~PutType();
  void SetPattern(int index);
  static PutType GetPattern(int index);
  /// STLをやめれば解決する可能性？でもそれはfuckだよね
  std::pair<int, int> CaluculatePutIndex(const Field &field) const;
  /// フィールドにこの置き方で置けるかどうか
  bool CanPut(const Field &field) const;
};

#endif
