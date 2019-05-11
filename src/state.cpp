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
	ojamas.Drop();
}

// 1フレームごとに更新すべき情報を更新
// 組ぷよの位置と回転を取得
// preお邪魔の残りフレームを調整
bool State::UpdateUnitFrame() {
	Bpuyo::PositionAndRotate(&now_kumipuyo, player_);
	ojamas.Update();
	return true;
}

void State::DetectOjama(const State& enemy_) {
	static bool checked = false;
	if (enemy_.GetMode() == Mode::CHAIN_CHECK) {
		checked = true;
		Field tmp(enemy_.field);
		Chain res = Simulator::Simulate(&tmp);
		int ojama = res.score / Ojama::ONE_SCORE;
		if (ojama > 0) {
			ojamas.pre_ojama.Set(ojama, res.frame);
		}
	}
	if (enemy_.GetMode() == Mode::OPERATION) {
		checked = false;
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
