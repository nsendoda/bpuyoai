#ifndef BPUYOAI_STATE_H_
#define BPUYOAI_STATE_H_

#include "field.h"
#include "kumipuyo.h"
#include "ojama.h"
#include "ojamatable.h"
#include "simulator.h"
#include "bpuyo.h"

/// 各プレイヤーの状態を記述するクラス
class State {
public:
  Field field;
  Kumipuyo now_kumipuyo, next_kumipuyo;
  int turn;
	OjamaTable ojamas;
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
	void UpdateUnitHand();

	// フレーム毎の更新
	bool UpdateUnitFrame();

	void DetectOjama(State * enemy_);

	void UpdateOjama(const State & enemy_);

	Mode GetMode() const;

	Player GetPlayer() const;

private:
  Player player_;
	bool chain_checked;
  bool TurnChanged();
};

#endif
