#pragma once

#include "Kumipuyo.h"
#include "Field.h"

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

	bool UpdateWhenDetectTurnChange();

private:
	int player_;
	bool TurnChanged();
};

