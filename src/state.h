#ifndef BPUYOAI_STATE_H_
#define BPUYOAI_STATE_H_

#include "field.h"
#include "kumipuyo.h"
#include "bpuyo.h"

/// 各プレイヤーの状態を記述するクラス
class State {
public:
  Field field;
  Kumipuyo now_kumipuyo, next_kumipuyo;
  int turn;
  int ojama;
	// 操作可能になってから経過したフレーム, [0, ...
	int hand_frame;

  State(int player);
  State(const Field &field, const Kumipuyo &now, const Kumipuyo &next);
  ~State();

  bool DetectTurnChange();

	// フィールドの情報が完全になる最初の操作可能ターンかどうか
	bool VisibleStateFirstTurn();

	// 手ごとの更新
	// 先行入力フレームでは正しくセットされないので呼ばないこと
	void Update();

	// フレーム毎の更新
	bool UpdateFrame();

private:
  int player_;
  bool TurnChanged();
};

#endif
