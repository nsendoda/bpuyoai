#include "mawashistate.h"

bool MawashiState::CanKarumeruMawashi(const Field& field_) {
	for (int c = 1; c < Field::COLUMN; c++) {

		int height = field_.GetLowestEmptyRows(c);
		if (height >= 13) {
			// 2列目で12段なら確実に1列目では回せないのでリセット
			if (c < 3) {
				continue;
			}
			// 4列目以降が12段だと確実に右で回せないのでbreak
			if (c > 3) {
				break;
			}
		}
		int base_index = field_.GetLowestEmptyRows(c) * Field::COLUMN + c;
		// 左側か右側のカルメル回しが出来れば良い
			if ((field_[base_index - Field::COLUMN - 1] == EMPTY
				&& field_[base_index - Field::COLUMN - 2] == EMPTY)
				|| (field_[base_index - Field::COLUMN + 1] == EMPTY
				&& field_[base_index - Field::COLUMN + 2] == EMPTY)) {
				return true;
			}
		}
	return false;
}

bool MawashiState::CanNormalMawashi(const Field& field_) {
	return field_[Field::FIELD_DEATH - 1] == EMPTY || field_[Field::FIELD_DEATH + 1] == EMPTY;
}

MawashiState::MawashiStateType MawashiState::Decide() {

	return [&](const State& state_, BpuyoPad* bpuyopad_) {
		for (int c = 1; c < Field::COLUMN; c++) {

			int height = state_.field.GetLowestEmptyRows(c);
			if (height >= 13) {
				// 2列目で12段なら確実に1列目では回せないのでリセット
				if (c < 3) {
					_max_height = 0;
					continue;
				}
				// 4列目以降が12段だと確実に右で回せないのでbreak
				if (c > 3) {
					break;
				}
			}

			bool now_can_karumeru = false;
			bool now_start_right_side = false;
			int base_index = state_.field.GetLowestEmptyRows(c) * Field::COLUMN + c;
			// left empty check
			if (c >= 3) {
				if (state_.field[base_index - Field::COLUMN - 1] == EMPTY
					&& state_.field[base_index - Field::COLUMN - 2] == EMPTY) {
					now_can_karumeru = true;
				}
			}
			// right empty check
			if (c <= 4) {
				if (state_.field[base_index - Field::COLUMN + 1] == EMPTY
					&& state_.field[base_index - Field::COLUMN + 2] == EMPTY) {
					now_can_karumeru = true;
					now_start_right_side = true;
				}
			}

			// if not karumeru, decide whether right side or left side
			if (!now_can_karumeru) {
				if (state_.field[base_index + 1] == EMPTY) now_start_right_side = true;
				else if (state_.field[base_index - 1] == EMPTY) now_start_right_side = false;
			}


			if (height == _max_height
				&& !_can_karumeru_mawashi && now_can_karumeru) {
				_can_karumeru_mawashi = true;
				_start_right_side = now_start_right_side;
				_best_column = c;
			}
			else if (height > _max_height) {
				_max_height = height;
				_can_karumeru_mawashi = now_can_karumeru;
				_start_right_side = now_start_right_side;
				_best_column = c;
			}
		}

		_mawashistate = Move();
		_mawashistate(state_, bpuyopad_);
	};
}

MawashiState::MawashiStateType MawashiState::Move() {
	return [&](const State& state_, BpuyoPad* bpuyopad_) {
		float wait_x = 0;
		_wait_y = state_.field.GetLowestEmptyRows(_best_column) - 1;

 		if (_can_karumeru_mawashi)
			if (_start_right_side) wait_x = _best_column;
			else wait_x = _best_column - 2;
		else wait_x = _best_column - 1;

		Pad pad;

		// set move
		if (state_.now_kumipuyo.X() < wait_x
			&& !bpuyopad_->IsRight()) {
			pad.SetRight(true);
		}
		else if (state_.now_kumipuyo.X() > wait_x
			&& !bpuyopad_->IsLeft()) {
			pad.SetLeft(true);
		}

		// set rotate
		if (_start_right_side) {
			if (state_.now_kumipuyo.now_rotate != RotateType::ROTATE_90
				&& !bpuyopad_->IsRotateRight()) {
				pad.SetRotateRight(true);
			}
		}
		else {
			if (state_.now_kumipuyo.now_rotate != RotateType::ROTATE_270
				&& !bpuyopad_->IsRotateLeft()) {
				pad.SetRotateLeft(true);
			}
		}

		// state transition
		// @note: このフレームで実行
		if (state_.now_kumipuyo.Y() == _wait_y) {
			if (_can_karumeru_mawashi) _mawashistate = KarumeruMawashiWait();
			else _mawashistate = MawashiWait();
			_mawashistate(state_, bpuyopad_);
			return;
		}
		// PRESS
		bpuyopad_->Press(pad);
	};
}

MawashiState::MawashiStateType MawashiState::KarumeruMawashiWait() {

	return [&](const State& state_, BpuyoPad* bpuyopad_) {
		const int KARUMERU_PRE_DELAY_FRAMELIMIT = _drop_frame;
		const int KARUMERU_DELAY_FRAMELIMIT = _now_put_delay_frame + _drop_frame;

		_karumeru_delay_frame++;

		Pad pad;
		if (_karumeru_delay_frame == KARUMERU_PRE_DELAY_FRAMELIMIT - 1) {
			if (_start_right_side) {
				pad.SetLeft(true);
			}
			else {
				pad.SetRight(true);
			}
		}
		// 最初の浮かし
		if (_karumeru_delay_frame == KARUMERU_DELAY_FRAMELIMIT - 1) {
			if (_start_right_side) {
				pad.SetRotateRight(true);
			}
			else {
				pad.SetRotateLeft(true);
			}
		}
		// 初期位置へ移動をしておく
		if (_karumeru_delay_frame == KARUMERU_DELAY_FRAMELIMIT) {
			if (_start_right_side) {
				pad.SetRotateLeft(true);
			}
			else {
				pad.SetRotateRight(true);
			}
		}
		if (_karumeru_delay_frame == KARUMERU_DELAY_FRAMELIMIT + 1) {
			if (_start_right_side) {
				pad.SetRight(true);
			}
			else {
				pad.SetLeft(true);
			}

			// state transition
			// @note: 次フレームから
			_mawashistate = KarumeruMawashi();
		}
		bpuyopad_->Press(pad);
	};
}

MawashiState::MawashiStateType MawashiState::MawashiWait() {
	_mawashi_delay_frame = 0;
	return [&](const State& state_, BpuyoPad* bpuyopad_) {

		const int DELAY_FRAMELIMIT = _now_put_delay_frame;
		_mawashi_delay_frame++;

		Pad pad;
		// 回し開始
		if (_mawashi_delay_frame == DELAY_FRAMELIMIT - 2) {
			if (_start_right_side) {
				pad.SetRotateRight(true);
			}
			else {
				pad.SetRotateLeft(true);
			}
		}
		if (_mawashi_delay_frame == DELAY_FRAMELIMIT - 1){
			if (_start_right_side) {
				pad.SetRotateLeft(true);
			}
			else {
				pad.SetRotateRight(true);
			}

			// state transition
      // @note: 次フレームから
			_mawashi_delay_frame = 0;
			_mawashistate = Mawashi();
		}
		bpuyopad_->Press(pad);
	};
}

// 初期状態:
// .rr
// ...
// x..
// ↑の様に、設置部分の右上or左下
// カルメル式回しを行う．
// @note: 既に1回浮かされているので、回し回数は8

MawashiState::MawashiStateType MawashiState::KarumeruMawashi() {
	_karumeru_delay_frame = 333;
	return [&](const State& state_, BpuyoPad* bpuyopad_) {
		
		const int KARUMERU_WAIT_FRAME = _drop_frame;
		const int MAX_KAERUMERU_MAWASHI = 8;

		Pad pad;

		if (state_.now_kumipuyo.Y() == _wait_y
			&&_karumeru_delay_frame > KARUMERU_WAIT_FRAME)
			_karumeru_delay_frame = 0;
		_karumeru_delay_frame++;

		// 浮き上がり戻し
		if (_karumeru_delay_frame == KARUMERU_WAIT_FRAME + _now_delay_rerotate) {
			if (_start_right_side) pad.SetRotateLeft(true);
			else pad.SetRotateRight(true);

			_mawashi_count++;

			if (_mawashi_count == MAX_KAERUMERU_MAWASHI) {
				// state transition
				// @note: 次フレームから
				_mawashistate = PutDecide();
			}
		}

		if (state_.now_kumipuyo.Y() == _wait_y) {
			if (_karumeru_delay_frame == KARUMERU_WAIT_FRAME - 1) {
				if (_start_right_side) {
					pad.SetLeft(true);
					pad.SetRotateRight(true);
				}
				else {
					pad.SetRight(true);
					pad.SetRotateLeft(true);
				}
			}
		}

		else if (_karumeru_delay_frame == KARUMERU_WAIT_FRAME + _now_delay_rerotate + 2) {
			if (_start_right_side) pad.SetRight(true);
			else pad.SetLeft(true);
		}

		bpuyopad_->Press(pad);
	};
}


// 初期状態:
// rr
// ..
// x.
// ↑の様に、設置部分の右上or左下
// 通常の回しを行う．
// @note: 既に1回浮かされているので、回し回数は7

MawashiState::MawashiStateType MawashiState::Mawashi() {
	_mawashi_delay_frame = 333;
	return [&](const State& state_, BpuyoPad* bpuyopad_) {

		const int MAWASHI_WAIT_FRAME = _drop_frame;
		const int MAX_MAWASHI = 7;

		Pad pad;

		if (state_.now_kumipuyo.Y() == _wait_y
			&& _mawashi_delay_frame > MAWASHI_WAIT_FRAME) {
			_mawashi_delay_frame = 0;
		}
		_mawashi_delay_frame++;



		if (state_.now_kumipuyo.Y() == _wait_y) {
			if (_mawashi_delay_frame == MAWASHI_WAIT_FRAME - 1) {
				if (_start_right_side) {
					pad.SetRotateRight(true);
				}
				else {
					pad.SetRotateLeft(true);
				}
			}
		}
		if (_mawashi_delay_frame == MAWASHI_WAIT_FRAME + _now_delay_rerotate) {
			if (_start_right_side) pad.SetRotateLeft(true);
			else pad.SetRotateRight(true);

			_mawashi_count++;

			if (_mawashi_count == MAX_MAWASHI) {
				// state transition
				// @note: 次フレームから
				_mawashistate = PutDecide();
			}
		}

		bpuyopad_->Press(pad);
	};
}

bool MawashiPutScoreCompare(const MawashiPutScore& a, const MawashiPutScore& b) {
	Column CENTER = 3;
	// デスツインor即死が確定しているなら、生存時間を最優先
	if (a.DeadRow() >= 12 && b.DeadRow() >= 12) 
		// デスツインがあるならそちらを優先
		if (a.Row3() != b.Row3() && (a.Row3() < 12 || b.Row3() < 12)) return a.Row3() < b.Row3();
		else return a.Frame() > b.Frame();
	if (a.DeadRow() != b.DeadRow()) return a.DeadRow() < b.DeadRow();
	if (a.Row3() != b.Row3()) return a.Row3() < b.Row3();
	if (a.RestOjama() != b.RestOjama()) return a.RestOjama() < b.RestOjama();
	if (a.RestField() != b.RestField()) return a.RestField() > b.RestField(); // Fieldの空とフレームは多い方が良いので注意
	if (a.Frame() != b.Frame()) return a.Frame() > b.Frame();
	if (std::abs(CENTER - a.Put().column) != std::abs(CENTER - b.Put().column)) return std::abs(CENTER - a.Put().column) > std::abs(CENTER - b.Put().column);
	return a.Put().column > b.Put().column;
};

MawashiState::MawashiStateType MawashiState::PutDecide() {
	return [&](const State& state_, BpuyoPad* bpuyopad_) {



		MawashiPutScore mawashi_puts[PUTTYPE_PATTERN];

		for (int first_i = 0; first_i < PUTTYPE_PATTERN; first_i++) {
			PutType first_p(PutType::GetPutType(first_i));
			if (!Simulator::CanPut(first_p, state_.field)) continue;

			Field first_field(state_.field);
			// 置くまでにかかるフレーム数
			int pre_frame = _drop_frame * ((12 - first_field.GetLowestEmptyRows(first_p.column)) * 2 + 1);

			Simulator::Put(state_.now_kumipuyo, &first_field, first_p);

			Chain c(Simulator::Simulate(&first_field));
			int rest_ojama = OjamaSimulator::DropOnceRoughly(&first_field, state_.ojamas.SumOjama());
			pre_frame += c.frame;


			// 2手目の整理

			std::vector<MawashiPutScore> second_mawashi_puts;

			for (int second_i = 0; second_i < PUTTYPE_PATTERN; second_i++) {
				PutType second_p(PutType::GetPutType(second_i));

				if (!Simulator::CanPut(second_p, first_field)) continue;

				Field second_field(first_field);

				int sum_frame = pre_frame;

				// 置くまでにかかるフレーム数
				sum_frame += _drop_frame * ((12 - second_field.GetLowestEmptyRows(second_p.column)) * 2 + 1);

				// 回しが出来るならその分フレームを加算
				if (CanKarumeruMawashi(second_field)) sum_frame += _drop_frame * 18;
				else if (CanNormalMawashi(second_field)) sum_frame += _drop_frame * 16;

				Simulator::Put(state_.next_kumipuyo, &second_field, second_p);

				Chain second_c(Simulator::Simulate(&second_field));
				int second_rest_ojama = OjamaSimulator::DropOnceRoughly(&second_field, rest_ojama);
				sum_frame += c.frame;

				second_mawashi_puts.push_back(
					MawashiPutScore(second_field.GetLowestEmptyRows(3) - 1,
						second_field.GetLowestEmptyRows(2) - 1, second_field.GetLowestEmptyRows(4) - 1, second_rest_ojama,
						sum_frame, second_field.CountMawashiEmptyPuyos(), first_p));

			}

			// 1手目で死亡が確定している時は1手目を代入
			if (second_mawashi_puts.empty()) {
				mawashi_puts[first_i] = MawashiPutScore(first_field.GetLowestEmptyRows(3) - 1,
					first_field.GetLowestEmptyRows(2) - 1, first_field.GetLowestEmptyRows(4) - 1,
					rest_ojama, pre_frame,  first_field.CountMawashiEmptyPuyos(), first_p);
			}
			// 2手目が置けるならば、2手目の内最も条件が良い評価を採用
			else {
				std::sort(second_mawashi_puts.begin(), second_mawashi_puts.end(), MawashiPutScoreCompare);
				mawashi_puts[first_i] = second_mawashi_puts[0];
			}
		}

		// 最も良い置き場所に置く
		std::sort(mawashi_puts, mawashi_puts + PUTTYPE_PATTERN, MawashiPutScoreCompare);
		_best_put = mawashi_puts[0].Put();

		_mawashistate = Put();
	};
}



MawashiState::MawashiStateType MawashiState::Put() {

	return [&](const State& state_, BpuyoPad* bpuyopad_) {
		float wait_x = _best_put.column - 1;

		Pad pad;
		// set move
		if (state_.now_kumipuyo.X() < wait_x
			&& !bpuyopad_->IsRight()) {
			pad.SetRight(true);
		}
		else if (state_.now_kumipuyo.X() > wait_x
			&& !bpuyopad_->IsLeft()) {
			pad.SetLeft(true);
		}

		// set rotate
		if (state_.now_kumipuyo.now_rotate != _best_put.rotate) {
			if ((_best_put.rotate == RotateType::ROTATE_90
				|| _best_put.rotate == RotateType::ROTATE_0)
				&& (state_.now_kumipuyo.now_rotate == RotateType::ROTATE_0
					|| state_.now_kumipuyo.now_rotate == RotateType::ROTATE_270)
				&& !bpuyopad_->IsRotateRight()) {
				pad.SetRotateRight(true);
			}
			else if ((_best_put.rotate == RotateType::ROTATE_270
				|| _best_put.rotate == RotateType::ROTATE_0)
				&& (state_.now_kumipuyo.now_rotate == RotateType::ROTATE_0
					|| state_.now_kumipuyo.now_rotate == RotateType::ROTATE_90)
				&& !bpuyopad_->IsRotateLeft()) {
				pad.SetRotateLeft(true);
			}
			else if (!bpuyopad_->IsRotateRight() && !bpuyopad_->IsRotateLeft()) {
				pad.SetRotateRight(true);
			}
		}

		bpuyopad_->Press(pad);
	};
}