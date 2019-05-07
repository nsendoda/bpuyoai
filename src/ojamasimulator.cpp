#include "ojamasimulator.h"

const int OjamaSimulator::convinient_drop_table[Field::COLUMN - 1][Field::COLUMN]
= {
{0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 1},
{0, 1, 0, 0, 0, 0, 1},
{0, 1, 0, 0, 0, 1, 1},
{0, 1, 1, 0, 0, 1, 1},
{0, 1, 1, 0, 1, 1, 1}
};

int OjamaSimulator::DropOnceRoughly(Field* field_, int ojama_) {

	// 30å¬à»â∫Ç‹Ç≈å∏ÇÁÇ∑
	int ojama_once = std::min(OjamaSimulator::DROPONCE_OJAMA, ojama_);
	int ojama_rest = ojama_ - ojama_once;
	// 5å¬à»â∫ÇÃÉâÉìÉ_ÉÄÇ…ç~ÇÈÇ®é◊ñÇ
	int fraction = ojama_once % (Field::COLUMN - 1);
	// ämíËÇ≈ç~ÇÈíiêî
	int row = ojama_once / (Field::COLUMN - 1);

	for (int c = 1; c < Field::COLUMN; c++) {
		int added_row = row + convinient_drop_table[fraction][c];
		if (field_->GetLowestEmptyRows(c) + added_row > Field::LOWEST_EMPTY_ROWS_MAX) {
			ojama_rest += field_->GetLowestEmptyRows(c) + added_row - Field::LOWEST_EMPTY_ROWS_MAX;
		}
		for (int i = 0; i < added_row && field_->GetLowestEmptyRows(c) < Field::LOWEST_EMPTY_ROWS_MAX; i++) {
			(*field_)[field_->GetEmptyIndex(c)] = Color::OJAMA;
			field_->AddLowestEmptyRows(c, 1);
		}
	}

	return ojama_rest;

}
