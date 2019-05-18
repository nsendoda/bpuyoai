#include "mawashiputsimulator.h"

bool MawashiSimulator::CanKarumeruMawashi(const Field& field_) {
	for (int c = 1; c < Field::COLUMN; c++) {

		int height = field_.GetLowestEmptyRows(c);
		if (height >= 13) {
			// 2��ڂ�12�i�Ȃ�m����1��ڂł͉񂹂Ȃ��̂Ń��Z�b�g
			if (c < 3) {
				continue;
			}
			// 4��ڈȍ~��12�i���Ɗm���ɉE�ŉ񂹂Ȃ��̂�break
			if (c > 3) {
				break;
			}
		}
		int base_index = field_.GetLowestEmptyRows(c) * Field::COLUMN + c;
		// �������E���̃J�������񂵂��o����Ηǂ�
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
		// �f�X�c�C��or�������m�肵�Ă���Ȃ�A�������Ԃ��ŗD��
		if (a.DeadRow() >= 12 && b.DeadRow() >= 12)
			// �f�X�c�C��������Ȃ炻�����D��
			if (a.Row3() != b.Row3() && (a.Row3() < 12 || b.Row3() < 12)) return a.Row3() < b.Row3();
			else return a.Frame() > b.Frame();
		if (a.DeadRow() != b.DeadRow()) return a.DeadRow() < b.DeadRow();
		if (a.Row3() != b.Row3()) return a.Row3() < b.Row3();
		if (a.RestOjama() != b.RestOjama()) return a.RestOjama() < b.RestOjama();
		if (a.RestField() != b.RestField()) return a.RestField() > b.RestField(); // Field�̋�ƃt���[���͑��������ǂ��̂Œ���
		if (a.Frame() != b.Frame()) return a.Frame() > b.Frame();
		if (std::abs(CENTER - a.Put().column) != std::abs(CENTER - b.Put().column)) return std::abs(CENTER - a.Put().column) > std::abs(CENTER - b.Put().column);
		return a.Put().column > b.Put().column;
	};

	MawashiPutScore mawashi_puts[PUTTYPE_PATTERN];

	for (int first_i = 0; first_i < PUTTYPE_PATTERN; first_i++) {
		PutType first_p(PutType::GetPutType(first_i));
		if (!Simulator::CanPut(first_p, state_.field)) continue;

		Field first_field(state_.field);
		// �u���܂łɂ�����t���[����
		int pre_frame = _drop_frame * ((12 - first_field.GetLowestEmptyRows(first_p.column)) * 2 + 1);

		Simulator::Put(state_.now_kumipuyo, &first_field, first_p);

		Chain c(Simulator::Simulate(&first_field));
		int rest_ojama = OjamaSimulator::DropOnceRoughly(&first_field, state_.ojamas.SumOjama());
		pre_frame += c.frame;


		// 2��ڂ̐���

		std::vector<MawashiPutScore> second_mawashi_puts;

		for (int second_i = 0; second_i < PUTTYPE_PATTERN; second_i++) {
			PutType second_p(PutType::GetPutType(second_i));

			if (!Simulator::CanPut(second_p, first_field)) continue;

			Field second_field(first_field);

			int sum_frame = pre_frame;

			// �u���܂łɂ�����t���[����
			sum_frame += _drop_frame * ((Field::VISIBLE_ROW - second_field.GetLowestEmptyRows(second_p.column)) * 2 + 1);

			// �񂵂��o����Ȃ炻�̕��t���[�������Z
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

		// 1��ڂŎ��S���m�肵�Ă��鎞��1��ڂ���
		if (second_mawashi_puts.empty()) {
			mawashi_puts[first_i] = MawashiPutScore(first_field.GetLowestEmptyRows(3) - 1,
				first_field.GetLowestEmptyRows(2) - 1, first_field.GetLowestEmptyRows(4) - 1,
				rest_ojama, pre_frame, first_field.CountMawashiEmptyPuyos(), first_p);
		}
		// 2��ڂ��u����Ȃ�΁A2��ڂ̓��ł��������ǂ��]�����̗p
		else {
			std::sort(second_mawashi_puts.begin(), second_mawashi_puts.end(), MawashiPutScoreCompare);
			mawashi_puts[first_i] = second_mawashi_puts[0];
		}
	}

	// �ł��ǂ��u���ꏊ�ɒu��
	std::sort(mawashi_puts, mawashi_puts + PUTTYPE_PATTERN, MawashiPutScoreCompare);
	return mawashi_puts[0].Put();
}