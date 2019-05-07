#include "Field.h"


/// 壁埋め, rows_heightの初期化
Field::Field(Player player):player_(player) {
	std::fill_n(field_, COLUMN, Parameter::WALL);
	for (int i = FIELD_START - 1; i <= FIELD_END + 1; i += COLUMN)
		field_[i] = Parameter::WALL;
	std::fill_n(lowest_empty_rows_index_, COLUMN, 1);
}

Field::Field(const Field& field):player_(field.player_){
	std::copy(field.field_, field.field_ + FIELD_SIZE, field_);
	std::copy(field.lowest_empty_rows_index_, field.lowest_empty_rows_index_ + COLUMN, lowest_empty_rows_index_);
}

Field::~Field() {}

//--------------------------------------------------------------------
/// @todo 浮いてるぷよがある場合のrows_height_
/// @todo test
/// @todo bpuyofieldが不正な値になった時の検知
void Field::LoadFromBpuyo() {
	bpuyofield = ai_GetField(player_);
	std::fill_n(lowest_empty_rows_index_, COLUMN, 1);
	for (int i = FIELD_START, b_i = 1; i <= FIELD_END; i++) {
		if (i % COLUMN != 0) {
			if (bpuyofield[b_i] < Parameter::EMPTY || bpuyofield[b_i] > Parameter::WALL) break; /// value violation
			field_[i] = bpuyofield[b_i++];
			if(field_[i] != Parameter::EMPTY) lowest_empty_rows_index_[i % COLUMN]++;
		}
	}
}

bool Field::VerifyBpuyo() {
	bpuyofield = ai_GetField(player_);
	for (int i = FIELD_START, b_i = 1; i <= FIELD_END; i++) {
		if (i % COLUMN != 0 && field_[i] != bpuyofield[b_i++]) return false;
	}
	return true;
}

void Field::Output()const {
	for (int i = FIELD_END - 5; i >= FIELD_START; i++) {
		if (i%COLUMN == 0) { 
			i -= 2 * COLUMN;
			Debug::Print("\n");
		}
		else Debug::Print(" %d", field_[i]);
	}
	Debug::Print("\nrows_height:");
	for (int i = 1; i < Field::COLUMN; ++i) {
		Debug::Print(" %d", lowest_empty_rows_index_[i]);
	}
	Debug::Print("\n");
}

