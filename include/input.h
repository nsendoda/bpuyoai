#ifndef BPUYOAI_INPUT_H_
#define BPUYOAI_INPUT_H_

#include "field.h"
#include "kumipuyo.h"
#include "puyo_ai.h"
#include "types.h"

class Input {
public:
  static void PositionAndRotate(Kumipuyo *puyo, Player player);
  static void NowColor(Kumipuyo *puyo, Player player);
  static void NextColor(Kumipuyo *puyo, Player player);
  // Bぷよのフィールドを取得して盤面に反映
  static void NowField(Field *field_, Player player_);

private:
  // Bぷよからフィールドを受け取るポインタ
  static unsigned char *bpuyofield;
};

#endif
