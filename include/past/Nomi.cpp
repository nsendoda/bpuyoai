#include "Nomi.h"
#undef max
#undef min


Nomi::Nomi()
{
}


Nomi::~Nomi()
{
}

PutType Nomi::Decide(const State& state) {
	if (state.decided) {
		return state.now_kumipuyo.desirable_put;
	}
	else {
		PutType best_put;
		best_put = Normal(state);
		return best_put;
	}
}

PutType Nomi::Normal(const State& state) {
	Chain chains[Parameter::PUTTYPE_NUMBER];
	Value values[Parameter::PUTTYPE_NUMBER];
	bool kill_mode = false; /// chainが致死を超えたらkill_mode = trueでchainのみ？で評価
	for (int put_i = 0; put_i < Parameter::PUTTYPE_NUMBER; ++put_i) {
		PutType cur_puttype(PutType::GetPattern(put_i));
		if (!(cur_puttype.CanPut(state.field))) continue;
		int parent_index, child_index;
		std::tie(parent_index, child_index) = cur_puttype.CaluculatePutIndex(state.field);
		Field simulational_field(state.field);
		int put_i_frame = state.now_kumipuyo.PutAndCaluculateFrame(&simulational_field, cur_puttype);
		chains[put_i] = Simulator::Simulate(&simulational_field, parent_index, child_index);
		if (simulational_field[Field::FIELD_DEATH] != Parameter::EMPTY) {
			chains[put_i].score = -10000;
			continue;
		}
		chains[put_i].frame += put_i_frame;
		for (int put_j = 0; put_j < Parameter::PUTTYPE_NUMBER; ++put_j) {
			PutType sec_cur_puttype(PutType::GetPattern(put_j));

			if (!(sec_cur_puttype.CanPut(simulational_field))) continue;
			int sec_parent_index, sec_child_index;
			std::tie(sec_parent_index, sec_child_index) = cur_puttype.CaluculatePutIndex(simulational_field);
			Field second_simulational_field(simulational_field);

			int put_j_frame = state.next_kumipuyo.PutAndCaluculateFrame(&second_simulational_field, sec_cur_puttype);

			Chain tmp_chain(Simulator::Simulate(&second_simulational_field, sec_parent_index, sec_child_index));
			tmp_chain.frame += put_i_frame + put_j_frame;
			Value tmp_value;
			tmp_value.ConnectionPotential(second_simulational_field, put_i_frame + put_j_frame);

			// update
			if (chains[put_i].score >= Parameter::LOWER_FATAL_DOSE || tmp_chain.score >= Parameter::LOWER_FATAL_DOSE) {
				if (tmp_chain.GreaterKillerThan(chains[put_i])) {
					chains[put_i] = tmp_chain;
				}
			}
			else {
				chains[put_i] = (std::max)(chains[put_i], tmp_chain);
			}
			if (tmp_value.GreaterThanByConnectionValueWay(values[put_i])) {
				values[put_i] = tmp_value;
			}

			/// chains[put_i] = max(chains[put_i], tmp_chain);
			/// chainsには一回目に置いた時にかかったフレーム分をtmp_chainに足してから比較すると、1手目致死と2手目致死を纏めて評価することが出来る
			/// values[put_i] = max(values[put_i], tmp_value);
		}
		if (chains[put_i].score >= Parameter::LOWER_FATAL_DOSE) kill_mode = true;
	}

	int max_put_i = 0;
	for (int put_i = 1; put_i < Parameter::PUTTYPE_NUMBER; ++put_i) {
		if (kill_mode && chains[put_i].GreaterKillerThan(chains[max_put_i])
			|| (!kill_mode && values[put_i].GreaterThanByConnectionValueWay(values[max_put_i]))) {
			max_put_i = put_i;
		}
	}
	return PutType::GetPattern(max_put_i);
}

void Nomi::Dig() {}
void Nomi::Mawashi() {}
void Nomi::Cope() {}