#include "nomithink.h"

Score NomiThink::CaluculateFatalDose(const State& state_) {

	Row max_need_ojamarow = Field::VISIBLE_ROW - state_.field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN);
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, state_.field)) continue;
		Field first_field(state_.field);
		Simulator::Put(state_.now_kumipuyo, &first_field, first_put);
		Chain first_chain = Simulator::Simulate(&first_field);

		for (PutIndex pj = 0; pj < PUTTYPE_PATTERN; pj++) {
			PutType second_put(pi);
			if (!Simulator::CanPut(second_put, first_field)) continue;

			Field second_field(first_field);
			Simulator::Put(state_.next_kumipuyo, &second_field, second_put);
			Chain second_chain = Simulator::Simulate(&second_field);

			Row need_ojamarow = Field::VISIBLE_ROW - second_field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN);
			max_need_ojamarow = std::max(need_ojamarow, max_need_ojamarow);
		}
	}
	return max_need_ojamarow * (Field::VISIBLE_COLUMN) * Ojama::ONE_SCORE;
	// frame‚ÍHput‚¾‚¯‚Å‚¢‚¢‚©‚à‚µ‚ê‚ñB
}

bool NomiThink::KillThink(const State& state, Score fatal_dose) {
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {

	}
	return false;
}

void NomiThink::Think(const State& state) {
	int best_score = 0;
	PutIndex best_put = 0;
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, state.field)) continue;
		Field first_field(state.field);
		Simulator::Put(state.now_kumipuyo, &first_field, first_put);
		Chain first_chain = Simulator::Simulate(&first_field);

		for (PutIndex pj = 0; pj < PUTTYPE_PATTERN; pj++) {
			PutType second_put(pi);
			if (!Simulator::CanPut(second_put, first_field)) continue;

			Field second_field(first_field);
			Simulator::Put(state.next_kumipuyo, &second_field, second_put);
			Chain second_chain = Simulator::Simulate(&second_field);
			for (Color c : {Color::RED, Color::GREEN, Color::YELLOW, Color::PURPLE}) {
				// ƒ]ƒ‚ð¶¬
				Kumipuyo virtual_kumi(c, c);
				for (PutIndex pk = 0; pk < PUTTYPE_PATTERN; pk++) {
					PutType third_put(pi);
					if (!Simulator::CanPut(third_put, second_field)) continue;

					Field third_field(second_field);
					Simulator::Put(virtual_kumi, &third_field, third_put);
					Chain third_chain = Simulator::Simulate(&third_field);
					if (third_chain.score > 0) {

					}

				}
			}
		}
	}
	state.now_kumipuyo.desirable_put = PutType::GetPutType(2);
}

void NomiThink::LinkCount(const Field& field_) {

}
