#include "bpuyo.h"

/// @pre 予めプレイヤーがセットされている
void Bpuyo::PositionAndRotate(Kumipuyo *puyo, Player player) {
  AI_NOWPOS *nowpos = ai_GetNowPos(player);
	puyo->SetX(nowpos->x);
	puyo->SetY(nowpos->y);
	puyo->now_rotate = static_cast<RotateType>(nowpos->rot);
}

/// @pre 予めプレイヤーがセットされている
void Bpuyo::NowColor(Kumipuyo *puyo_, Player player_) {
  AI_NOW *nowpuyo = ai_GetNow2(player_);
  puyo_->parent = static_cast<Color>(nowpuyo->a);
  puyo_->child = static_cast<Color>(nowpuyo->b);
}

void Bpuyo::NextColor(Kumipuyo *puyo_, Player player_) {
  AI_NOW *nextpuyo = ai_GetNext2(player_);
  puyo_->parent = static_cast<Color>(nextpuyo->a);
  puyo_->child = static_cast<Color>(nextpuyo->b);
}

//--------------------------------------------------------------------
/// @todo 浮いてるぷよがある場合のrows_height_
/// @todo test
/// @todo bpuyofieldが不正な値になった時の検知
void Bpuyo::NowField(Field *field_, Player player_) {
	unsigned char *bpuyofield;
  bpuyofield = ai_GetField(player_);
  for (int i = Field::FIELD_START, b_i = 1; i <= Field::FIELD_END; i++) {
    if (i % Field::COLUMN != 0) {
      if (bpuyofield[b_i] < Color::EMPTY || bpuyofield[b_i] > Color::WALL)
        break; /// value violation
      (*field_)[i] = static_cast<Color>(bpuyofield[b_i++]);
    }
  }
	field_->ModifyLowestEmptyRows();
}

int Bpuyo::Ojama(Player player_) {
	return ai_GetYokokuOjama(player_);
}

Mode Bpuyo::GetMode(Player player_) {
	return static_cast<Mode>(ai_GetPlayerMode(player_));
}

int Bpuyo::Turn(Player player) {
	return ai_GetCount2(player);
}