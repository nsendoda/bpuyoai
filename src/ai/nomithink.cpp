#include "nomithink.h"


const int NomiThink::dt[4] = { -1, 1, Field::COLUMN, -Field::COLUMN };

std::vector<PutType> NomiThink::ConstantPut(const State& state_, const Kumipuyo& pre) {
	std::string puyos;
	int NOTHING = -1;
	std::map<int, int> mp = { {EMPTY, NOTHING}, {RED, NOTHING}, {YELLOW, NOTHING}, {GREEN, NOTHING}, {PURPLE, NOTHING} };
	bool reversed[3] = { false, false, false };
	int proto_puyos[6] = { pre.parent, pre.child, state_.now_kumipuyo.parent, state_.now_kumipuyo.child, state_.next_kumipuyo.parent, state_.next_kumipuyo.child };
	std::string unique_s = "ABCDE";
	int ct = 0;
	for (int i = 0; i < 6; i ++){
		if (proto_puyos[i] == EMPTY) continue; // ����ɂ����1��ڂɌĂԎ���puyos��4�����ɂȂ�
		if (mp[proto_puyos[i]] == NOTHING) {
			mp[proto_puyos[i]] = ct;
			ct++;
		}
		puyos += unique_s[mp[proto_puyos[i]]];
	}

	// 3�蕪�܂ŕ⊮
	while (puyos.size() < 6) {
		puyos += 'A';
	}

	// reverse
	// 1��ڂ͓s�����΂ɔ��]���Ȃ�
	for (int ri = 0; ri < 3; ri++) {
		if (puyos[ri * 2] > puyos[ri * 2 + 1]) {
			std::swap(puyos[ri * 2], puyos[ri * 2 + 1]);
			reversed[ri] = true;
		}
	}



	std::vector<PutType> ans(FirstConstants(puyos));

	for (int ri = 0; ri < 3; ri++) {
		if (reversed[ri]) {
			PutIndex p = PutType::Convert(ans[ri]);
			ans[ri] = PutType::REVERSE_PUTS[p];
		}
	}
	return ans;
}

std::vector<PutType> NomiThink::FirstConstants(const std::string& p) {
	std::map<std::string, std::vector<PutType> > mp =
	{ 
	{"AAAAAA", {PutType(4, ROTATE_180), PutType(1, ROTATE_180), PutType(1, ROTATE_180)}},
	{"AAAAAB", {PutType(4, ROTATE_180), PutType(1, ROTATE_180), PutType(3, ROTATE_180)}},
	{"AAAABB", {PutType(4, ROTATE_180), PutType(1, ROTATE_180), PutType(5, ROTATE_180)}},

	{"AAABAA", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(1, ROTATE_180)}},
	{"AAABAB", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(3, ROTATE_180)}},
	{"AAABAC", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(3, ROTATE_180)}},
	{"AAABBB", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(4, ROTATE_90 )}},
	{"AAABBC", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(4, ROTATE_0  )}},
	{"AAABCC", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(5, ROTATE_180)}},
	{"AAABCD", {PutType(4, ROTATE_180), PutType(3, ROTATE_180), PutType(5, ROTATE_180)}},

	{"AABBAA", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(4, ROTATE_180)}},
	{"AABBAB", {PutType(4, ROTATE_180), PutType(4, ROTATE_270), PutType(3, ROTATE_180)}},
	{"AABBAC", {PutType(4, ROTATE_180), PutType(3, ROTATE_270), PutType(3, ROTATE_180)}},
	{"AABBBB", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_180)}},
	{"AABBBC", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(4, ROTATE_90 )}},
	{"AABBCC", {PutType(4, ROTATE_180), PutType(3, ROTATE_270), PutType(5, ROTATE_180)}},
	{"AABBCD", {PutType(4, ROTATE_180), PutType(3, ROTATE_270), PutType(5, ROTATE_180)}},

	{"AABCAA", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(5, ROTATE_180)}},
	{"AABCAB", {PutType(4, ROTATE_180), PutType(4, ROTATE_0  ), PutType(3, ROTATE_180)}},
	{"AABCAC", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_180)}},
	{"AABCAD", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(3, ROTATE_180)}},
	{"AABCBB", {PutType(4, ROTATE_180), PutType(4, ROTATE_0  ), PutType(3, ROTATE_270)}},
	{"AABCBC", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(5, ROTATE_270)}},
	{"AABCBD", {PutType(4, ROTATE_180), PutType(5, ROTATE_0  ), PutType(4, ROTATE_90 )}},
	{"AABCCC", {PutType(4, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_270)}},
	{"AABCCD", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(4, ROTATE_90 )}},
	{"AABCDD", {PutType(4, ROTATE_180), PutType(5, ROTATE_180), PutType(5, ROTATE_180)}},

	{"ABAAAA", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(1, ROTATE_180)}},
	{"ABAAAB", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(3, ROTATE_180)}},
	{"ABAAAC", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(3, ROTATE_180)}},
	{"ABAABB", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(4, ROTATE_180)}},
	{"ABAABC", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(4, ROTATE_0  )}},
	{"ABAACC", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(5, ROTATE_180)}},
	{"ABAACD", {PutType(4, ROTATE_270), PutType(4, ROTATE_270), PutType(5, ROTATE_180)}},

	{"ABABAA", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(4, ROTATE_180)}},
	{"ABABAB", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(3, ROTATE_180)}},
	{"ABABAC", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(3, ROTATE_180)}},
	{"ABABBB", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(3, ROTATE_270)}},
	{"ABABBC", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(5, ROTATE_0  )}},
	{"ABABCC", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(5, ROTATE_180)}},
	{"ABABCD", {PutType(4, ROTATE_90), PutType(4, ROTATE_0), PutType(5, ROTATE_180)}},

	{"ABACAA", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(5, ROTATE_180)}},
	{"ABACAB", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
	{"ABACAC", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
	{"ABACAD", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
	{"ABACBB", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(6, ROTATE_180)}},
	{"ABACBC", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(4, ROTATE_90 )}},
	{"ABACBD", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(4, ROTATE_90 )}},
	{"ABACCC", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(6, ROTATE_180)}},
	{"ABACCD", {PutType(4, ROTATE_90), PutType(4, ROTATE_90), PutType(5, ROTATE_0  )}},

	{"ABBBAA", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(4, ROTATE_180)}},
	{"ABBBAB", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(3, ROTATE_0  )}},
	{"ABBBAC", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(4, ROTATE_0  )}},
	{"ABBBBB", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(1, ROTATE_180)}},
	{"ABBBBC", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(3, ROTATE_180)}},
	{"ABBBCC", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(5, ROTATE_180)}},
	{"ABBBCD", {PutType(3, ROTATE_90), PutType(3, ROTATE_90), PutType(5, ROTATE_180)}},

	{"ABBCAA", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(4, ROTATE_180)}},
	{"ABBCAB", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(3, ROTATE_0  )}},
	{"ABBCAC", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(4, ROTATE_90 )}},
	{"ABBCAD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(4, ROTATE_90 )}},
	{"ABBCBB", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(5, ROTATE_180)}},
	{"ABBCBC", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
	{"ABBCBD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(3, ROTATE_180)}},
	{"ABBCCC", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(6, ROTATE_180)}},
	{"ABBCCD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(5, ROTATE_0  )}},
	{"ABBCDD", {PutType(5, ROTATE_270), PutType(4, ROTATE_90), PutType(5, ROTATE_180)}},

	// B����
	{"ABCCAA", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(6, ROTATE_180)}},
	{"ABCCAB", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_270)}},
	{"ABCCAC", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_0  )}},
	{"ABCCAD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_0  )}},
	{"ABCCBB", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_90 )}},
	{"ABCCBC", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(4, ROTATE_90 )}},
	{"ABCCBD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(4, ROTATE_90 )}},
	{"ABCCCC", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(5, ROTATE_180)}},
	{"ABCCCD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_180)}},
	{"ABCCDD", {PutType(5, ROTATE_180), PutType(4, ROTATE_180), PutType(3, ROTATE_270)}},

	{"ABCDAA", {PutType(3, ROTATE_90 ), PutType(2, ROTATE_180), PutType(3, ROTATE_90)}},
	{"ABCDAB", {PutType(3, ROTATE_90 ), PutType(5, ROTATE_180), PutType(4, ROTATE_180)}},
	{"ABCDAC", {PutType(3, ROTATE_90 ), PutType(2, ROTATE_0  ), PutType(3, ROTATE_0  )}},
	{"ABCDAD", {PutType(3, ROTATE_90 ), PutType(2, ROTATE_180), PutType(3, ROTATE_0  )}},
	{"ABCDBB", {PutType(3, ROTATE_90 ), PutType(5, ROTATE_180), PutType(3, ROTATE_90 )}},
	{"ABCDBC", {PutType(3, ROTATE_90 ), PutType(5, ROTATE_0  ), PutType(4, ROTATE_0  )}},
	{"ABCDBD", {PutType(3, ROTATE_90 ), PutType(5, ROTATE_180), PutType(4, ROTATE_0  )}},
	{"ABCDCC", {PutType(3, ROTATE_90 ), PutType(4, ROTATE_270), PutType(3, ROTATE_90 )}},
	{"ABCDCD", {PutType(3, ROTATE_90 ), PutType(3, ROTATE_180), PutType(4, ROTATE_180)}},
	{"ABCDDD", {PutType(3, ROTATE_90 ), PutType(3, ROTATE_90 ), PutType(3, ROTATE_90 )}},
	};

	return mp[p];
}

Score NomiThink::CalculateFatalDose(const State& state_) {

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

			// �ł����ז����K�v�ƂȂ�3��ڂ̍�������
			Row need_ojamarow = Field::VISIBLE_ROW - second_field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN);
			max_need_ojamarow = std::max(need_ojamarow, max_need_ojamarow);
		}
	}
	return max_need_ojamarow * (Field::VISIBLE_COLUMN) * Ojama::ONE_SCORE;
	// frame�́Hput�����ł������������B
}

// Score��FatalDose�����鎞�Atrue��Ԃ��Afi��FieldIndex���Z�b�g
bool NomiThink::KillThink(const State& state, Score fatal_dose, FieldIndex * fi) {
	int best_score = 0;
	PutIndex best_put = 0;

	struct KillRate {
		Score score;
		Frame frame;
		bool first_put_kill;
		PutIndex pi;
		Score fatal_dose;
	};
	struct CompareKillRate {
		bool operator () (const KillRate& a, const KillRate& b) const {
			if (a.score > a.fatal_dose && b.score > b.fatal_dose) {
				if (a.first_put_kill != b.first_put_kill) return b.first_put_kill;
				return a.frame > b.frame;
			}
			if (b.score > b.fatal_dose) return true;
			if (a.score > a.fatal_dose) return false;
			return a.pi < b.pi;
		}
	};

	std::priority_queue<KillRate, std::vector<KillRate>, CompareKillRate> first_que;
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, state.field)) continue;
		Field first_field(state.field);
		Frame first_frame = Simulator::Put(state.now_kumipuyo, &first_field, first_put);
		Chain first_chain(Simulator::Simulate(&first_field));
		first_frame += first_chain.frame;

		// DEATH
		if (first_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

		first_que.push({ first_chain.score, first_frame, true, pi, fatal_dose });

		std::priority_queue<KillRate, std::vector<KillRate>, CompareKillRate> second_que;
		for (PutIndex pj = 0; pj < PUTTYPE_PATTERN; pj++) {
			PutType second_put(pj);
			if (!Simulator::CanPut(second_put, first_field)) continue;

			Field second_field(first_field);
			Frame second_frame = Simulator::Put(state.next_kumipuyo, &second_field, second_put);
			Chain second_chain(Simulator::Simulate(&second_field));
			second_frame += second_chain.frame;


			// DEATH
			if (second_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

			second_que.push({ first_chain.score + second_chain.score, first_frame + second_frame, false, pi, fatal_dose });
		}
		if (!second_que.empty()) first_que.push(second_que.top());
	}
	if (!first_que.empty() && first_que.top().score >= fatal_dose) {
		(*fi) = first_que.top().pi;
		return true;
	}
	return false;
}

PutType NomiThink::Think(const State& state, Score fatal_dose) {

	auto RATETOWER = [](const TowerRate &a, const TowerRate &b){

		
		// ���ɔ��΂Œv���ʂ𒴂���ꍇ
		if (a.score > a.fatal_dose && b.score > b.fatal_dose) {
			if (a.instant_delete != b.instant_delete) return a.instant_delete < b.instant_delete;
			else return a.frame > b.frame;
		}
		if (b.score > b.fatal_dose || a.score > a.fatal_dose) return a.score < b.score;

		// ���ݓI�ɒv���ʂ𒴂���ꍇ
		if (a.potential_score > a.fatal_dose && b.potential_score > b.fatal_dose) {
			if (a.instant_delete != b.instant_delete) return a.instant_delete < b.instant_delete;
			else if (a.potential_needs != b.potential_needs) return a.potential_needs > b.potential_needs;
			else if (a.potential_frame != b.potential_frame) return a.potential_frame > b.potential_frame;
			else  return a.frame > b.frame;
		}
		if (b.potential_score > b.fatal_dose || a.potential_score > a.fatal_dose) return a.potential_score < b.potential_score;

		// �������Ή\��
		if (a.instant_delete != b.instant_delete) return a.instant_delete < b.instant_delete;
		
		if (a.Rate() != b.Rate()) return a.Rate() < b.Rate();
		//		if (a.potential_score + a.score != b.potential_score + b.score) return  a.potential_score + a.score < b.potential_score + b.score;
		if (a.frame != b.frame) return a.frame > b.frame;
		return a.putindex > b.putindex;
	};
	int best_score = 0;
	PutIndex best_put = 0;
	std::vector<TowerRate> first_que;
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, state.field)) continue;
		Field first_field(state.field);
		Frame first_frame = Simulator::Put(state.now_kumipuyo, &first_field, first_put);
		Chain first_chain(Simulator::Simulate(&first_field));
		first_frame += first_chain.frame;

		// DEATH
		if (first_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

		std::vector<TowerRate> second_que;
		for (PutIndex pj = 0; pj < PUTTYPE_PATTERN; pj++) {
			PutType second_put(pj);
			if (!Simulator::CanPut(second_put, first_field)) continue;

			Field second_field(first_field);
			Frame second_frame = Simulator::Put(state.next_kumipuyo, &second_field, second_put);
			Chain second_chain(Simulator::Simulate(&second_field));
			second_frame += second_chain.frame;

			// DEATH
			if (second_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

			TowerBase base(BaseDecide(second_field));
			if (!CanFireTower(second_field, base.base[1], -1)) {
				second_que.push_back(Waruagaki(second_field, fatal_dose, pi, first_frame + second_frame));
			}
			else {
				second_que.push_back(RateTower(second_field, base, fatal_dose, pi, first_frame + second_frame));
			}
		}
		if (!second_que.empty()) {
			std::sort(second_que.begin(), second_que.end(), RATETOWER);

			first_que.push_back(second_que.back());
		}
	}
	if (first_que.size()) {

		std::sort(first_que.begin(), first_que.end(), RATETOWER);
		return first_que.back().GetPutIndex();
	}

	return PutType(0);
}

TowerBase NomiThink::BaseDecide(const Field& f) {
	const int C = Field::COLUMN;
	// 1�i�ړy��ƒ�グ�y��
	for (int d : {0, Field::COLUMN }) {

		// o�͓y��Ղ�
		// v�̗L���ŕ���
		// ..vo..
		// ...o..

		if (f.ColorEqual(d + C + 4, d + C * 2 + 4)) {
			Color p = f[d + C + 4];
			if (f.ColorEqual(d + C + 4, d + C * 2 + 3))
				return TowerBase(p, d + C * 2 + 3, d + C + 4, d + C * 2 + 4, 4, true);
			// ���炵�^���[�A2��5���������Ȃ�
			if (f.ColorEqual(d + C + 4, d + C + 3) || f.ColorEqual(d + C + 4, d + C + 5) || f.ColorEqual(d + C + 4, d + C * 2 + 5))
				return TowerBase(p, 0, 0, 0, 0, true);
			else
				return TowerBase(p, d + C + 4, d + C * 2 + 4, 4, true);
		}
		// ..ov..
		// ..o...
		if (f.ColorEqual(d + C + 3, d + C * 2 + 3)) {
			Color p = f[d + C + 3];
			if (f.ColorEqual(d + C + 3, d + C * 2 + 4))
				return TowerBase(p ,d + C * 2 + 4, d + C * 2 + 3, d + C + 3, 3, false);
			// ���炵�^���[�A2��5���������Ȃ�
			if (f.ColorEqual(d + C + 3, d + C + 4) || f.ColorEqual(d + C + 3, d + C + 2) || f.ColorEqual(d + C + 3, d + C * 2 + 2))
				return TowerBase(p,0, 0, 0, 0, false);
			else
				return TowerBase(p, d + C * 2 + 3, d + C + 3, 3, false);
		}
	}
	// �y��\�z���s
	return TowerBase(EMPTY, 0, 0, 0, 0, true);
}

// @note:�K���^���[�̔��Γ_�̂����ꂩ��i�ɓn������
// i��ǂɂ���ƕK��false��Ԃ��B
// @note:�K��3�A���ȉ��ɂȂ�index��n�����ƁB�łȂ��Ɩ������[�v������������
// @args:i�͌��݂�index, pre�͑O��̈ʒu��FieldIndex
bool NomiThink::CanFireTower(const Field& f, FieldIndex i, FieldIndex pre) {
	if (f[i] == Color::WALL) return false;
	bool ok = false;
	for (int d : dt) {
		if (i + d == pre) continue;
		if (f[i] == f[i + d]) ok |= CanFireTower(f, i + d, i);
		if (f[i + d] == Color::EMPTY) return true;
	}
	return ok;
}


TowerRate NomiThink::Waruagaki(const Field& second_field, Score fatal_dose, PutIndex pi, Frame puts_frame) {
	std::priority_queue<TowerRate, std::vector<TowerRate>, CompareTowerRate> third_que;
	for (Color c : {Color::RED, Color::GREEN, Color::YELLOW, Color::PURPLE}) {
		// �]���𐶐�
		Kumipuyo virtual_kumi(c, c);
		for (PutIndex pk = 0; pk < PUTTYPE_PATTERN; pk++) {
			PutType third_put(pk);
			if (!Simulator::CanPut(third_put, second_field)) continue;

			Field third_field(second_field);
			Simulator::Put(virtual_kumi, &third_field, third_put);
			Chain third_chain = Simulator::Simulate(&third_field);

			TowerRate res(third_chain.score, 2, third_chain.frame);
			res.frame = puts_frame;
			res.SetPutIndex(pi);
			res.SetInstantDelete(false);
			res.SetFatalDose(fatal_dose);
			third_que.push(res);
		}
	}
	if (third_que.empty()) return TowerRate(0, 0, 0);
	return third_que.top();
}

TowerRate NomiThink::RateTower(const Field& f_, const TowerBase& t_base, Score fatal_dose, PutIndex pi, Frame puts_frame) {

	static const Score ONECHAIN_AND_DROPBONUS = 200;

	Chain actual(ActualChain(f_, t_base));
	TowerRate rate(VirtualChain(f_, t_base, fatal_dose));
	// rate
	actual.frame += puts_frame;
	rate.SetActual(actual);
	rate.SetPutIndex(pi);
	rate.SetInstantDelete(true);
	rate.SetFatalDose(fatal_dose - ONECHAIN_AND_DROPBONUS);

	// 3�A���y��͗ǂ���
	if (f_[t_base.base.at(0)] == f_[t_base.base.at(1)] && f_[t_base.base.at(1)] == f_[t_base.base.at(2)]) {
		rate.potential_score += 2000;
	}
	return rate;
}

Chain NomiThink::ActualChain(const Field& f_, const TowerBase& t_base) {
	Field f(f_);
	BaseDelete(&f, t_base);
	Simulator::FallAll(&f);
	// 2�A���ڊJ�n�Ń^���[����
	return Simulator::Simulate(&f, -1, -1, 1);
}

TowerRate NomiThink::VirtualChain(const Field & f_, const TowerBase& t_base, Score fatal_dose) {
	Field f(f_);
	// �y��������ĂՂ�𗎂Ƃ�
	BaseDelete(&f, t_base);

	Simulator::FallAll(&f);

	// �⊮
	bool used[Field::FIELD_SIZE];
	std::fill_n(used, Field::FIELD_SIZE, false);
	int complement_ct = ComplementTower3And2(&f, used, t_base);

	// ���ڂ̕⊮�Œv���𒴂��Ă����炻����̗p
	Field tmp(f);
	Chain first_chain(Simulator::Simulate(&tmp,-1,-1,1));
//	if (first_chain.score + 40 >= fatal_dose)
		return TowerRate(first_chain.score, complement_ct, first_chain.frame);

	// ������x�⊮���s���B
	// @because 2->3��3->4�ɂ���B
	std::fill_n(used, Field::FIELD_SIZE, false);
	complement_ct += ComplementTower3(&f, used, t_base);

	Chain second_chain(Simulator::Simulate(&f, -1, -1, 1));
	return TowerRate(second_chain.score, complement_ct, second_chain.frame);
}

// �^���[�̓y��Ɠy�����̂��ז��Ղ�������B
void NomiThink::BaseDelete(Field* f, const TowerBase& t_base) {

	bool used[Field::FIELD_SIZE];
	std::fill(used, used + Field::FIELD_SIZE, false);
	std::queue<int> que;
	for (FieldIndex base : t_base.base) {
		que.push(base);
	}
	while ( ! que.empty()) {
		int i = que.front();
		que.pop();
		if (used[i]) continue;
		(*f)[i] = Color::EMPTY;
		used[i] = true;

		for (int d : {-1, 1, -Field::COLUMN, Field::COLUMN}) {
			if (used[i + d]) continue;
			if ((*f)[i + d] == Color::OJAMA) {
				(*f)[i + d] = Color::EMPTY;
				continue;
			}
			if (f->ColorEqual(i, i + d)) que.push(i + d);
		}
	}
}

// first_index����A������Ă���Ղ�̐���Ԃ��B
// �Ղ�͏����Ȃ��B
// @note 1-12�i�ڂ܂ł�����B
int NomiThink::LinkCount(const Field& field, FieldIndex first_index) {
	if (!field.IsColor(first_index)) return 0;
	std::queue<int> que;
	que.push(first_index);
	bool used[Field::FIELD_SIZE];
	std::fill_n(used, Field::FIELD_SIZE, false);
	int count = 0;
	while (!que.empty()) {
		int j = que.front();
		que.pop();
		used[j] = true;
		count++;
		for (int d : {-Field::COLUMN, +1, +Field::COLUMN, -1}) {
			if (used[j + d] || field[j] != field[j + d] || j + d > Field::VISIBLE_FIELD_END)
				continue;
			que.push(j + d);
		}
	}
	return count;
}

int NomiThink::ComplementTower3And2(Field* f, bool* used, const TowerBase& t_base) {

	int ct = ComplementTower3(f, used, t_base);

	for (int c_i = 0; c_i < t_base.complement_column.size(); c_i++) {
		Column complement_c = t_base.complement_column[c_i];
		// ���U�[�u��̒T���͐�����COLUMN���������Ă��鎞�̂�
		if (complement_c == TowerBase::NOTEXIST) continue;
		for (Row r = 1; r <= Field::VISIBLE_ROW; r++) {
			FieldIndex ind = r * Field::COLUMN + complement_c;

			if (used[ind]) continue;

			const int d = t_base.GetDirect();

			// �y��ƕ⊮�ʒu������Đ������⊮�o���Ȃ��ꍇ
			if (c_i == 1 && r == 2
				&& t_base.color == (*f)[ind]) {
				if (Complement2Connection(f, used, ind, ind + Field::COLUMN, c_i, 0, d, true)) ct++;
				if (Complement2Connection(f, used, ind, ind + d, c_i, 1, d, true)) ct++;
			}

			if (Complement2Connection(f, used, ind, ind + Field::COLUMN, c_i, 0, d, false)) ct++;
			if (Complement2Connection(f, used, ind, ind + d, c_i, 1, d, false)) ct++;
		}
	}
	return ct;
}

// 3�A���̕⊮���s���B
// @note towerbase��[0]�̓��U�[�u��ł��邪�A������NOTEXIST���������Ă���΃X�L�b�v����B
int NomiThink::ComplementTower3(Field* f, bool* used, const TowerBase& t_base) {
	int ct = 0;
	for (int c_i = 0; c_i < t_base.complement_column.size(); c_i++) {
		Column complement_c = t_base.complement_column[c_i];
		// ���U�[�u��̒T���͐�����COLUMN���������Ă��鎞�̂�
		if (complement_c == TowerBase::NOTEXIST) continue;
		for (Row r = 1; r <= Field::VISIBLE_ROW; r++) {
			FieldIndex ind = r * Field::COLUMN + complement_c;

			if (used[ind]) continue;
			if ((*f)[ind] == Color::EMPTY) break;

			const int d = t_base.GetDirect();

			// �y��ƕ⊮�ʒu������Đ������⊮�o���Ȃ��ꍇ
			if ( ((c_i == 0 && r == 3) || (c_i == 1 && r == 2))
				&& t_base.color == (*f)[ind]) {
				ct += Complement3Connection(f, used, ind, ind + Field::COLUMN, ind + 2 * Field::COLUMN, c_i, 0, d, true);
				ct += Complement3Connection(f, used, ind, ind + Field::COLUMN, ind + d + Field::COLUMN, c_i, 1, d, true);
				ct += Complement3Connection(f, used, ind, ind + d, ind + d - Field::COLUMN, c_i, 2, d, true);
				ct += Complement3Connection(f, used, ind, ind + d, ind + d + Field::COLUMN, c_i, 3, d, true);
				ct += Complement3Connection(f, used, ind, ind + Field::COLUMN, ind + d, c_i, 4, d, true);
				ct += Complement3Connection(f, used, ind, ind + d, ind + 2 * d, c_i, 5, d, true);
				// left, right
				continue;
			}

			ct += Complement3Connection(f, used, ind, ind + Field::COLUMN, ind + 2 * Field::COLUMN, c_i, 0, d, false);
			ct += Complement3Connection(f, used, ind, ind + Field::COLUMN, ind + d + Field::COLUMN, c_i, 1, d, false);
			ct += Complement3Connection(f, used, ind, ind + d, ind + d - Field::COLUMN, c_i, 2, d, false);
			ct += Complement3Connection(f, used, ind, ind + d, ind + d + Field::COLUMN, c_i, 3, d, false);
			ct += Complement3Connection(f, used, ind, ind + Field::COLUMN, ind + d, c_i, 4, d, false);
			ct += Complement3Connection(f, used, ind, ind + d, ind + 2 * d, c_i, 5, d, false);
		}
	}
	return ct;
}

// base, j, k��3�A������Ȃ�Ղ��⊮����4�A���ɂ���B
// ��������4�A���ȏ�̏ꍇ�͕⊮�͍s��Ȃ��B
bool NomiThink::Complement3Connection(Field* f, bool* used, FieldIndex base, FieldIndex j, FieldIndex k, int c_i, int shape_i, int direct, bool base_conflict) {
	// 13�i�ڂ̂Ղ悪����Ă���A���͐F���Ⴆ��return
	if (!f->ColorEqual(base, j, k) || j > Field::VISIBLE_FIELD_END || k > Field::VISIBLE_FIELD_END) return false;
	used[base] = true;
	used[j] = true;
	used[k] = true;
	if (LinkCount(*f, base) >= 4) return false;
	NomiMemory::ComplementTower3Connection[c_i][shape_i](f, base, direct, base_conflict);
	return true;
}

bool NomiThink::Complement2Connection(Field* f, bool* used, FieldIndex base, FieldIndex j, int c_i, int shape_i, int direct, bool base_conflict) {
	// 13�i�ڂ̂Ղ悪����Ă���A���͐F���Ⴆ��return
	if (!f->ColorEqual(base, j) || j > Field::VISIBLE_FIELD_END) return false;
	used[base] = true;
	used[j] = true;
	if (LinkCount(*f, base) >= 4) return false;
	NomiMemory::ComplementTower2Connection[c_i][shape_i](f, base, direct, base_conflict);
	return true;
}