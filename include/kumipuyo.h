#ifndef BPUYOAI_KUMIPUYO_AI_
#define BPUYOAI_KUMIPUYO_AI_

#include <cmath>

#include "field.h"
#include "puttype.h"
#include "rotatetype.h"
#include "types.h"

/// コンストラクタがプレイヤー初期化を伴うことに注意
class Kumipuyo {
public:
  Color parent, child;
  // x=[0, 5], y=[0, 11.5]
  float x, y;
  RotateType rotate;
  PutType desirable_put;
  Kumipuyo();
  ~Kumipuyo();

  /// xとyの情報から座標を返す
  inline int GetIndex() const {
    return static_cast<int>(x + 1 + Field::COLUMN * (std::floor(y) + 1));
  }
};

#endif
