#include "mawashi.h"

void Mawashi::Execute(const State& state_, BpuyoPad* bpuyopad_) {
	if (AdjustPut(state_)) return;
	mawashistate.Excute(state_, bpuyopad_);
}

bool Mawashi::AdjustPut(const State& state) {

	return false;
}

// true��Ԃ����A�񂵁A�M�u�A�b�v�A��Ԃ�MawashiState�ɕύX����B
// false��Ԃ����A�������Ȃ��B
bool Mawashi::ShouldMawashi(const State & state, const State & enemy)
{
	int my_count = CountMawashi(state);
  int enemy_count = CountMawashi(enemy);
	if (my_count == MAX_MAWASHI_COUNT) {
		if (enemy_count == MAX_MAWASHI_COUNT) return false;
		return true;
	}

	// FATALDOSE�Ɍ����Ēǌ�
	if (enemy_count == MAX_MAWASHI_COUNT || my_count < enemy_count) {
		return false;
	}
	return true;
}




int Mawashi::CountMawashi(const State& state) const {
	// ���i�̂��ז��Ŋm�莀���邩
	Row fatal_row_ojama = std::min(Field::VISIBLE_ROW - (state.field.GetLowestEmptyRows(3) - 1)
		, std::max(Field::VISIBLE_ROW - (state.field.GetLowestEmptyRows(2) - 1), Field::VISIBLE_ROW - (state.field.GetLowestEmptyRows(4) - 1)));
	int ojama_change = state.ojamas.Change(state.field);
	int rest = state.ojamas.instant_quantity;
	if (fatal_row_ojama - Divide(rest, Field::VISIBLE_COLUMN) - Divide(ojama_change, Field::VISIBLE_COLUMN) > 0) return MAX_MAWASHI_COUNT;
	int ans = 0;
	while (fatal_row_ojama > 0) {

		// �ŏ��̑z��̌�ނ�Ƃ��ז��Ղ悪�S�ė��������ꍇ
		if (rest == 0 && ojama_change == 0) return ans + 1;

		if (rest >= Ojama::MAX_ONCE) {
			fatal_row_ojama -= Ojama::MAX_ONCE / Field::VISIBLE_COLUMN;
			rest -= Ojama::MAX_ONCE;
		}
		else {
			fatal_row_ojama -= rest / Field::VISIBLE_COLUMN;
			rest = ojama_change;
			ojama_change = 0;
		}
		ans++;
	}
	return ans;
}

int Mawashi::Divide(int a, int b) const {
	return (a + b - 1) / b;
}
