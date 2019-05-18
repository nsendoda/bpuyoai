#include "state.h"

State::State(int player) : turn(0), hand_frame(0), player_(player) {}

State::State(const Field &field, const Kumipuyo &now, const Kumipuyo &next)
    : field(field), now_kumipuyo(now), next_kumipuyo(next) {}

State::~State() {}

bool State::DetectTurnChange() {
  bool turnchanged = TurnChanged();
	if (turnchanged) {
		hand_frame = 0;
		return true;
	}
	hand_frame++;
  return false;
}


bool State::VisibleStateFirstTurn() {
	return hand_frame == 1;
}

// 手ごとの更新すべき情報を更新
// フィールド以外にも、お邪魔量を手動更新
void State::UpdateUnitHand() {
	Bpuyo::PositionAndRotate(&now_kumipuyo, player_);
	Bpuyo::NowColor(&now_kumipuyo, player_);
	Bpuyo::NextColor(&next_kumipuyo, player_);
	Bpuyo::NowField(&field, player_);
}

// 1フレームごとに更新すべき情報を更新
// 組ぷよの位置と回転を取得
// preお邪魔の残りフレームを調整
bool State::UpdateUnitFrame() {
	Bpuyo::PositionAndRotate(&now_kumipuyo, player_);
	ojamas.Update();
	return true;
}

// お邪魔を検出する。
// @note 連鎖チェック時にフィールドはこの関数が終わった後も消した状態にされるが、
// 相手のMODEがCHAIN_CHECKの時に必要なフィールドはこの関数の処理以外では連鎖後のフィールドなので、
// 消してしまって良い。
void State::DetectOjama(State * enemy_) {
	if ( ! chain_checked && enemy_->GetMode() == Mode::CHAIN_CHECK) {
		chain_checked = true;
		PutType enemy_put = PutType(enemy_->now_kumipuyo.GetColumn(), enemy_->now_kumipuyo.now_rotate);
		Simulator::Put(enemy_->now_kumipuyo, &enemy_->field, enemy_put);
		Chain res = Simulator::Simulate(&enemy_->field);
		int ojama = res.score / Ojama::ONE_SCORE;
		if (ojama > 0) {
			ojamas.pre_ojama.Set(ojama, res.frame);
		}
	}
	// 操作可能状態になったら再度検出をスタート。
	if (chain_checked && enemy_->GetMode() == Mode::OPERATION) {
		chain_checked = false;
	}

}

// 相手の連鎖が終了している時は、手動で更新
// 自分にお邪魔ぷよが降っている時はお邪魔ぷよ量は取得しない
// @because お邪魔落下中はFieldにはお邪魔は落下していないが、Ojamaは落下する分減らされた状態になり、不整合が生じる
void State::UpdateOjama(const State& enemy_) {
	if ((enemy_.GetMode() == Mode::OPERATION)
		&& GetMode() != Mode::OJAMA_DROP) {
		ojamas.pre_ojama.Set(0, 0);
		ojamas.instant_quantity = Bpuyo::Ojama(player_);
	}
}

Mode State::GetMode() const{
	return Bpuyo::GetMode(player_);
}

Player State::GetPlayer() const
{
	return player_;
}

bool State::TurnChanged() {
  int next_turn = Bpuyo::Turn(player_);
  if (next_turn != turn) {
    turn = next_turn;
    return true;
  }
  return false;
}
