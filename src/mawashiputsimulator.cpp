#include "mawashiputsimulator.h"

bool MawashiSimulator::CanKarumeruMawashi(const Field& field_) {
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

bool MawashiSimulator::CanNormalMawashi(const Field& field_) {
	return field_[Field::FIELD_DEATH - 1] == EMPTY || field_[Field::FIELD_DEATH + 1] == EMPTY;
}

PutType MawashiSimulator::BestMawashi(const State& state_, const int _drop_frame) {

	auto MawashiPutScoreCompare = [](const MawashiPutScore& a, const MawashiPutScore& b) {
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
			sum_frame += _drop_frame * ((Field::VISIBLE_ROW - second_field.GetLowestEmptyRows(second_p.column)) * 2 + 1);

			// 回しが出来るならその分フレームを加算
			if (CanKarumeruMawashi(second_field)) sum_frame += _drop_frame * 18;
			else if (CanNormalMawashi(second_field)) sum_frame += _drop_frame * 16;

			Simulator::Put(state_.next_kumipuyo, &second_field, second_p);

			Chain second_c(Simulator::Simulate(&second_field));
			int second_rest_ojama = OjamaSimulator::DropOnceRoughly(&second_field, rest_ojama);
			sum_frame += second_c.frame;

			second_mawashi_puts.push_back(
				MawashiPutScore(second_field.GetLowestEmptyRows(3) - 1,
					second_field.GetLowestEmptyRows(2) - 1, second_field.GetLowestEmptyRows(4) - 1, second_rest_ojama,
					sum_frame, second_field.CountMawashiEmptyPuyos(), first_p));

		}

		// 1手目で死亡が確定している時は1手目を代入
		if (second_mawashi_puts.empty()) {
			mawashi_puts[first_i] = MawashiPutScore(first_field.GetLowestEmptyRows(3) - 1,
				first_field.GetLowestEmptyRows(2) - 1, first_field.GetLowestEmptyRows(4) - 1,
				rest_ojama, pre_frame, first_field.CountMawashiEmptyPuyos(), first_p);
		}
		// 2手目が置けるならば、2手目の内最も条件が良い評価を採用
		else {
			std::sort(second_mawashi_puts.begin(), second_mawashi_puts.end(), MawashiPutScoreCompare);
			mawashi_puts[first_i] = second_mawashi_puts[0];
		}
	}

	// 最も良い置き場所に置く
	std::sort(mawashi_puts, mawashi_puts + PUTTYPE_PATTERN, MawashiPutScoreCompare);
	return mawashi_puts[0].Put();
}