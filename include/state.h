#ifndef BPUYOAI_STATE_H_
#define BPUYOAI_STATE_H_

#include "kumipuyo.h"
#include "field.h"

/// 各プレイヤーの状態を記述するクラス
class State
{
public:
	Kumipuyo now_kumipuyo, next_kumipuyo;
	Field field;
	int turn;
	/// @?
	int ojama;
	bool decided;

	State(int player);
	~State();

  // 一度しか呼んではいけない。
  // これを解決するデザインパターンないかな
	bool UpdateWhenDetectTurnChange();

private:
	int player_;
	bool TurnChanged();
};

#endif
