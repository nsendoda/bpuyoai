#ifndef BPUYOAI_BPUYO_H_
#define BPUYOAI_BPUYO_H_

#include "field.h"
#include "kumipuyo.h"
#include "mode.h"
#include "../puyo_ai.h"
#include "types.h"

class Bpuyo {
public:
  static void PositionAndRotate(Kumipuyo *puyo, Player player);
  static void NowColor(Kumipuyo *puyo, Player player);
  static void NextColor(Kumipuyo *puyo, Player player);
  // Bぷよのフィールドを取得して盤面に反映
  static void NowField(Field *field_, Player player_);
	static int Ojama(Player player_);
	static Mode GetMode(Player player_);
	static int Turn(Player player_);
	// 1マス落下するのにかかるフレーム
	static Frame GetDropSpeed(Player player_);
};

#endif
