#include "state.h"

State::State(int player) : turn(0), ojama(0), hand_frame(0), player_(player) {}

State::State(const Field &field, const Kumipuyo &now, const Kumipuyo &next)
    : field(field), now_kumipuyo(now), next_kumipuyo(next) {}

State::~State() {}

bool State::DetectTurnChange() {
  bool turnchanged = TurnChanged();
	ojama = Bpuyo::Ojama(player_);
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

void State::Update() {
	Bpuyo::PositionAndRotate(&now_kumipuyo, player_);
	Bpuyo::NowColor(&now_kumipuyo, player_);
	Bpuyo::NextColor(&next_kumipuyo, player_);
	Bpuyo::NowField(&field, player_);
}

// 組ぷよの位置と回転を取得
bool State::UpdateFrame() {
	Bpuyo::PositionAndRotate(&now_kumipuyo, player_);
	return true;
}

bool State::TurnChanged() {
  int next_turn = Bpuyo::Turn(player_);
  if (next_turn != turn) {
    turn = next_turn;
    return true;
  }
  return false;
}
