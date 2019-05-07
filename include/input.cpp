#include "input.h"

/// @pre 予めプレイヤーがセットされている
void Input::PositionAndRotate(Kumipuyo *puyo, Player player) {
  AI_NOWPOS *nowpos = ai_GetNowPos(player);
  puyo->x = nowpos->x;
  puyo->y = nowpos->y;
  puyo->rotate = static_cast<RotateType>(nowpos->rot);
}

/// @pre 予めプレイヤーがセットされている
void Input::NowColor(Kumipuyo *puyo, Player player) {
  AI_NOW *nowpuyo = ai_GetNow2(player);
  puyo->parent = static_cast<Color>(nowpuyo->a);
  puyo->child = static_cast<Color>(nowpuyo->b);
}

void Input::NextColor(Kumipuyo *puyo, Player player) {
  AI_NOW *nextpuyo = ai_GetNext2(player);
  puyo->parent = static_cast<Color>(nextpuyo->a);
  puyo->child = static_cast<Color>(nextpuyo->b);
}

//--------------------------------------------------------------------
/// @todo 浮いてるぷよがある場合のrows_height_
/// @todo test
/// @todo bpuyofieldが不正な値になった時の検知
void Input::NowField(Field *field, Player player_) {
  bpuyofield = ai_GetField(player_);
  field->InitLowestEmptyRowsIndex();
  for (int i = Field::FIELD_START, b_i = 1; i <= Field::FIELD_END; i++) {
    if (i % Field::COLUMN != 0) {
      if (bpuyofield[b_i] < Color::EMPTY || bpuyofield[b_i] > Color::WALL)
        break; /// value violation
      (*field)[i] = static_cast<Color>(bpuyofield[b_i++]);
      if ((*field)[i] != Color::EMPTY)
        field->AddLowestEmptyRowsIndex(i % Field::COLUMN, 1);
    }
  }
}
