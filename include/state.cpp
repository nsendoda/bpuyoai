#include "state.h"

State::State(int player)
    : now_kumipuyo(player), next_kumipuyo(player), turn(0), ojama(0),
      field(player), decided(false), player_(player) {}

State::~State() {}

bool State::UpdateWhenDetectTurnChange() {
  bool turnchanged = TurnChanged();
  now_kumipuyo.SetPositionAndRotate();
  // ターン変更時, フィールドと組みぷよを読み込む
  if (turnchanged) {
    field.LoadFromBpuyo();
    now_kumipuyo.SetNowColor();
    next_kumipuyo.SetNextColor();
    decided = false;
    return true;
  }

  return false;
}

bool State::TurnChanged() {
  int next_turn = ai_GetCount2(player_);
  if (next_turn != turn) {
    turn = next_turn;
    return true;
  }
  return false;
}
