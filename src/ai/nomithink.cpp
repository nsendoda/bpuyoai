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
		if (proto_puyos[i] == EMPTY) continue; // これによって1手目に呼ぶ時はpuyosは4文字になる
		if (mp[proto_puyos[i]] == NOTHING) {
			mp[proto_puyos[i]] = ct;
			ct++;
		}
		puyos += unique_s[mp[proto_puyos[i]]];
	}

	// 3手分まで補完
	while (puyos.size() < 6) {
		puyos += 'A';
	}

	// reverse
	// 1手目は都合上絶対に反転しない
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
	return NomiMemory::first_const_puts.at(p);
}

Score NomiThink::CalculateFatalDose(const State& enemy_, int enemy_all_ojama) {
	// [11, ..] 落下ボーナスを考慮に入れてない分1段甘めに見積もっている。
	auto OjamaRow = [](Row emptyrow, Row ojamarow) {
		return Field::VISIBLE_ROW - emptyrow - ojamarow;
	};
	Row ojama_row = enemy_all_ojama / Field::VISIBLE_COLUMN;
	Row max_need_ojamarow = OjamaRow(enemy_.field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN), ojama_row);
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, enemy_.field)) continue;
		Field first_field(enemy_.field);
		Simulator::Put(enemy_.now_kumipuyo, &first_field, first_put);
		Chain first_chain = Simulator::Simulate(&first_field);

		for (PutIndex pj = 0; pj < PUTTYPE_PATTERN; pj++) {
			PutType second_put(pi);
			if (!Simulator::CanPut(second_put, first_field)) continue;

			Field second_field(first_field);
			Simulator::Put(enemy_.next_kumipuyo, &second_field, second_put);
			Chain second_chain = Simulator::Simulate(&second_field);

			// 最もお邪魔が必要となる3列目の高さを代入
			Row need_ojamarow = OjamaRow(second_field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN), ojama_row);
			max_need_ojamarow = std::max(need_ojamarow, max_need_ojamarow);
		}
	}
	return max_need_ojamarow * (Field::VISIBLE_COLUMN) * Ojama::ONE_SCORE;
	// frameは？putだけでいいかもしれん。
}

// ScoreがFatalDoseを上回る時、trueを返し、fiにFieldIndexをセット
bool NomiThink::KillThink(const State& state, Score fatal_dose, FieldIndex * fi) {
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

	std::vector<KillRate> first_que;
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, state.field)) continue;
		Field first_field(state.field);
		Frame first_frame = Simulator::Put(state.now_kumipuyo, &first_field, first_put);
		Chain first_chain(Simulator::Simulate(&first_field));
		first_frame += first_chain.frame;

		// DEATH
		if (first_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

		first_que.push_back({ first_chain.score, first_frame, true, pi, fatal_dose });

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
		if (!second_que.empty()) first_que.push_back(second_que.top());
	}
	if (!first_que.empty()) {
		std::sort(first_que.begin(), first_que.end(), CompareKillRate());
		if (first_que.back().score >= fatal_dose) {
			(*fi) = first_que.back().pi;
			return true;
		}
	}
	return false;
}

bool NomiThink::ReactJab(const State & state, Score fatal_dose, FieldIndex * fi, bool deadly) {
	const Frame ShouldReactFrame = 60;
	// 2段以上が降ってきて残りフレーム数が少ない時
	if ( ! deadly && (state.ojamas.SumOjama() < Field::VISIBLE_COLUMN * 2
		|| state.ojamas.pre_ojama.rest_drop > ShouldReactFrame) ) return false;

	struct ReactRate {
		Score score;
		Frame frame;
		Score fatal_dose;
		FieldIndex pi;
	};
	struct CompareKillRate {
		bool operator () (const ReactRate& a, const ReactRate& b) const {
			if (a.score > a.fatal_dose && b.score > b.fatal_dose) {
				return a.frame < b.frame;
			}
			if (a.score > a.fatal_dose) return true;
			if (b.score > b.fatal_dose) return false;
			if (a.score != b.score) return a.score > b.score;
			return a.frame < b.frame;
		};
	};

	std::vector<ReactRate> rates;
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, state.field)) continue;
		Field first_field(state.field);
		Frame first_frame = Simulator::Put(state.now_kumipuyo, &first_field, first_put);
		Chain first_chain(Simulator::Simulate(&first_field));
		first_frame += first_chain.frame;

		// DEATH
		if (first_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

		rates.push_back({ first_chain.score, first_frame, fatal_dose, pi });


		// 2手目以降で発火が出来ない
		if (first_frame >= state.ojamas.pre_ojama.rest_drop) continue;
		std::vector<ReactRate> second_que;
		for (PutIndex pj = 0; pj < PUTTYPE_PATTERN; pj++) {
			PutType second_put(pj);
			if (!Simulator::CanPut(second_put, first_field)) continue;

			Field second_field(first_field);
			Frame second_frame = Simulator::Put(state.next_kumipuyo, &second_field, second_put);
			Chain second_chain(Simulator::Simulate(&second_field));
			second_frame += second_chain.frame;


			// DEATH
			if (second_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

			second_que.push_back({ first_chain.score + second_chain.score, first_frame + second_frame, fatal_dose, pi });
		}
		if (!second_que.empty()) {
			std::sort(second_que.begin(), second_que.end(), CompareKillRate());
			rates.push_back(second_que[0]);
		}
	}
	if (!rates.empty()) {
		std::sort(rates.begin(), rates.end(), CompareKillRate());
		(*fi) = rates[0].pi;
		return true;
	}
	return false;
}


PutType NomiThink::ChainThink(const State& state, Score fatal_dose) {
	std::vector<ChainRate> rates;
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType first_put(pi);
		if (!Simulator::CanPut(first_put, state.field)) continue;
		Field first_field(state.field);
		Frame first_frame = Simulator::Put(state.now_kumipuyo, &first_field, first_put);
		Chain first_chain(Simulator::Simulate(&first_field));
		first_frame += first_chain.frame;

		// DEATH
		if (first_field[Field::FIELD_DEATH] != Color::EMPTY)  continue;

		rates.push_back(ChainSecondThink(first_field, state.next_kumipuyo, first_frame, pi, fatal_dose));
	}
	if (rates.empty()) return PutType(0);
	std::sort(rates.begin(), rates.end(), ChainRate::Compare);
	return PutType(rates[0].first_pi);
}

ChainRate NomiThink::ChainSecondThink(const Field& pre_field, const Kumipuyo& next, Frame pre_frame, const PutIndex first_pi, Score fatal_dose) {
	std::vector<ChainRate> rates;
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {
		PutType put(pi);
		if (!Simulator::CanPut(put, pre_field)) continue;
		Field field(pre_field);
		Frame frame = Simulator::Put(next, &field, put);
		Chain chain(Simulator::Simulate(&field));
		frame += chain.frame;

		// DEATH
		if (field[Field::FIELD_DEATH] != Color::EMPTY) continue;

		rates.push_back(ChainThirdThink(field, pre_frame + frame, first_pi, fatal_dose));
	}
	if (rates.empty()) return ChainRate();
	std::sort(rates.begin(), rates.end(), ChainRate::Compare);
	return rates[0];
}


struct MyVector {
	ChainRate crs[60];
	int _size;
	MyVector() : _size(0) {}

	void Push(const ChainRate& c) {
		if (_size >= 60) {
			return;
		}
		crs[_size++] = c;
	}
	bool empty() const {
		return _size == 0;
	}
};

ChainRate NomiThink::ChainThirdThink(const Field& pre_field, Frame pre_frame, const PutIndex first_pi, Score fatal_dose) {

	MyVector rates;
	bool used[Field::FIELD_SIZE] = {};

	const int IMPLEMENTABLE_MIN_CONNECTION = 2;
	const int FIRSTCHAIN_MAX_CONNECTION = 4;

	std::vector<FieldIndex> links[50];
	int c_i = 0;

	std::vector<FieldIndex> tmp_link;
	std::array<bool, Field::FIELD_SIZE> tmp_used = {};

//	auto Complement

	for (Column c = 1; c <= Field::VISIBLE_COLUMN; c++) {
		for (Row r = 1; r <= Field::VISIBLE_ROW; r++) {
			FieldIndex start_fi = r * Field::COLUMN + c;
			if (pre_field[start_fi] == EMPTY) break;
			if (used[start_fi]) continue;
			if (SetLink(pre_field, start_fi, &links[c_i], used)) {

				if (links[c_i].size() < IMPLEMENTABLE_MIN_CONNECTION) continue;

				Field f(pre_field);

				// 発火点として使用できるかチェック
				bool ok = false;
				for (FieldIndex li : links[c_i]) {
					for (int d : {-1, 1, Field::COLUMN}) {
						FieldIndex n_fi = li + d;
						// 空マスであり、下に床があって、置ける列なら補完
						if (f[n_fi] == Color::EMPTY && f[n_fi - Field::COLUMN] != Color::EMPTY
							&& Simulator::CanPut(PutType(n_fi%Field::COLUMN, RotateType::ROTATE_0), f)) {

							// ただし、補完後が4連結以下であること。

							// ぷよ補完
							f[n_fi] = f[start_fi];

							// 補完後,何連結になるかを調べる
							tmp_link.clear();
							tmp_used.fill(false);
							SetLink(f, n_fi, &tmp_link, tmp_used.data());

							// 補完UNDO
							f[n_fi] = Color::EMPTY;

							// 4連結以下
							if (tmp_link.size() <= FIRSTCHAIN_MAX_CONNECTION) {
								ok = true;
								break;
							}

						}
					}
					if (ok) break;
				}

				if (ok) {
					for (FieldIndex li : links[c_i]) {
						f[li] = Color::EMPTY;
					}
					Simulator::FallAll(&f);



					int required_puyo = 4 - links[c_i].size();

					Field complemented(f);
					const int FIRST_SCORE = 40;
					ChainRate complemented_cr(ComplementedChain(&complemented, pre_field, links[c_i], 1, FIRST_SCORE, pre_frame, required_puyo, first_pi, fatal_dose));

					Chain ch = Simulator::Simulate(&f, -1, -1, 1);
					ch.score += FIRST_SCORE;
					ch.frame += pre_frame;

					Row crisis_height = std::max(Field::AVAILABLE_ROW - f.GetLowestEmptyRows(3),
						std::min(Field::AVAILABLE_ROW - f.GetLowestEmptyRows(2), Field::AVAILABLE_ROW - f.GetLowestEmptyRows(4)));

					int empty_count = f.CountEmptyPuyos();

					ChainRate cr(ChainRate(ch, required_puyo, crisis_height, empty_count, first_pi, fatal_dose));
					if(ChainRate::Compare(cr, complemented_cr)) rates.Push(cr);
					else rates.Push(complemented_cr);
				}
				c_i++;
			}
		}
	}

	if (rates.empty()) return ChainRate();
	std::sort(rates.crs, rates.crs + rates._size, ChainRate::Compare);
	return rates.crs[0];
}

bool NomiThink::SetLink(const Field& f, FieldIndex fi, std::vector<FieldIndex>* links, bool* used){
	used[fi] = true;
	bool linked = false;
	for (int d : {-1, 1, Field::COLUMN, -Field::COLUMN}) {
		if (used[fi + d]) continue;
		if (f.ColorEqual(fi, fi + d)) {
			linked = true;
			if ( ! SetLink(f, fi + d, links, used)) {
				links->push_back(fi + d);
			}
		}
	}
	if (linked) links->push_back(fi);
	return linked;
}

// deleted_fには1連鎖目が消されて、落下した直後のFieldが渡される。
// 2連鎖目がこれから行われるため、4連結以上も含まれている。
ChainRate NomiThink::ComplementedChain(Field * deleted_f,
 const Field& pre_field, const std::vector<FieldIndex>& pre_link,
	int chain_num, Score pre_score, Frame pre_frame, int pre_needs, const PutIndex first_pi, Score fatal_dose) {

	// 補完を行う最低連結数
	const int IMPLEMENTABLE_MIN_CONNECTION_PUYO = 2;

	Field& f = *deleted_f;
	bool used[Field::FIELD_SIZE] = {};

	std::array<bool, Field::FIELD_SIZE> pre_used = {};

	std::vector<FieldIndex> link;
	std::vector<FieldIndex> pre_links;
	int c_i = 0;

	//	auto Complement
	// 中央に近いものを優先
	for (Column c : {3, 4, 5, 2, 6, 1}) {
		for (Row r = 1; r <= Field::VISIBLE_ROW; r++) {
			FieldIndex start_fi = r * Field::COLUMN + c;
			if (f[start_fi] == EMPTY) break;
			if (used[start_fi]) continue;
			link.clear();
			if (SetLink(f, start_fi, &link, used)) {
				// 4連結以上と規定連結未満は補完しない
				if (link.size() < IMPLEMENTABLE_MIN_CONNECTION_PUYO || link.size() >= PUYO_DELETE_NUMBER) continue;
				// 補完するぷよとして使用できるかチェック
				bool ok = false;
				int complemented_count = PUYO_DELETE_NUMBER - link.size();
				for (FieldIndex li : link) {
					for (int d : {-1, 1, Field::COLUMN}) {
						FieldIndex n_fi = li + d;
						// #現在補完するフィールドに対して.. 空マスであり、下に床がある
						// #発火前のフィールドに対して..     空マスであり、下に床があって、置ける列であり、その時点で消えない
						if (f[n_fi] == Color::EMPTY &&         f[n_fi - Field::COLUMN] != Color::EMPTY
							&& pre_field[n_fi] == Color::EMPTY && pre_field[n_fi - Field::COLUMN] != Color::EMPTY
							&& Simulator::CanPut(PutType(n_fi%Field::COLUMN, RotateType::ROTATE_0), pre_field)
							) {
							// ぷよ補完
							f[n_fi] = f[start_fi];
							if (link.size() == 2) f[n_fi + Field::COLUMN] = f[start_fi];

							// 発火前のフィールドに対して補完後、その時点で消えないことを調べる
							pre_links.clear();
							pre_used.fill(false);
							SetLink(pre_field, n_fi, &pre_links, pre_used.data());
							// 4連結以下
							if (pre_links.size() < PUYO_DELETE_NUMBER) {
								ok = true;
								break;
							}
							// 失敗時補完UNDO
							f[n_fi] = Color::EMPTY;
						}
					}
					if (ok) break;
				}

				if (ok) {
					Chain ch = Simulator::Simulate(&f, -1, -1, chain_num);
					ch.frame += pre_frame;
					ch.score += pre_score;
					Row crisis_height = std::max(Field::AVAILABLE_ROW - f.GetLowestEmptyRows(3),
						std::min(Field::AVAILABLE_ROW - f.GetLowestEmptyRows(2), Field::AVAILABLE_ROW - f.GetLowestEmptyRows(4)));

					int empty_count = f.CountEmptyPuyos();

					return ChainRate(ch, pre_needs + complemented_count, crisis_height, empty_count, first_pi, fatal_dose);
				}
			}
		}
	}
	return ChainRate();
}


ChainRate NomiThink::ChainThirdThink2(const Field& pre_field, Frame pre_frame, const PutIndex first_pi, Score fatal_dose) {
	
	MyVector rates;

	Field third_field(pre_field);

	for (Column c = 1; c <= Field::VISIBLE_COLUMN; c++) {
		PutType third_put(c, RotateType::ROTATE_180);
		if (!Simulator::CanPut(third_put, pre_field)) continue;
		for (Color puyo : {Color::RED, Color::GREEN, Color::YELLOW, Color::PURPLE}) {
				// ゾロを生成
				Kumipuyo virtual_kumi(puyo, puyo);

			Simulator::Put(virtual_kumi, &third_field, third_put);

			Chain third_chain = Simulator::Simulate(&third_field);

			Row crisis_height = std::max(Field::AVAILABLE_ROW - third_field.GetLowestEmptyRows(3),
				std::min(Field::AVAILABLE_ROW - third_field.GetLowestEmptyRows(2), Field::AVAILABLE_ROW - third_field.GetLowestEmptyRows(4)));

			int empty_count = third_field.CountEmptyPuyos();

			// 前手までのフレームを加算
			third_chain.frame += pre_frame;

			if (&third_chain == nullptr) {
				empty_count++;
			}

			ChainRate cr(third_chain, 0, fatal_dose, crisis_height, empty_count, first_pi);

			rates.Push(cr);

			// UNDO
			if (third_chain.score > 0) {
				third_field = pre_field;
			}
			else {
				std::pair<FieldIndex, FieldIndex> put_indexes(Simulator::CalculatePutIndex(third_put, third_field));
				third_field[put_indexes.first] = Color::EMPTY;
				third_field[put_indexes.second] = Color::EMPTY;
			}
		}
	}
	if (rates.empty()) return ChainRate();
	std::sort(rates.crs, rates.crs + rates._size, ChainRate::Compare);
	return rates.crs[0];
}


PutType NomiThink::Think(const State& state, Score fatal_dose) {

	auto RATETOWER = [](const TowerRate &a, const TowerRate &b){

		
		// 既に発火で致死量を超える場合
		if (a.score > a.fatal_dose && b.score > b.fatal_dose) {
			if (a.instant_delete != b.instant_delete) return a.instant_delete < b.instant_delete;
			else return a.frame > b.frame;
		}
		if (b.score > b.fatal_dose || a.score > a.fatal_dose) return a.score < b.score;

		// 潜在的に致死量を超える場合
		if (a.potential_score > a.fatal_dose && b.potential_score > b.fatal_dose) {
			if (a.instant_delete != b.instant_delete) return a.instant_delete < b.instant_delete;
			else if (a.potential_needs != b.potential_needs) return a.potential_needs > b.potential_needs;
			else if (a.potential_frame != b.potential_frame) return a.potential_frame > b.potential_frame;
			else  return a.frame > b.frame;
		}
		if (b.potential_score > b.fatal_dose || a.potential_score > a.fatal_dose) return a.potential_score < b.potential_score;

		// 即時発火可能か
		if (a.instant_delete != b.instant_delete) return a.instant_delete < b.instant_delete;
		
		if (a.Rate() != b.Rate()) return a.Rate() < b.Rate();
		//		if (a.potential_score + a.score != b.potential_score + b.score) return  a.potential_score + a.score < b.potential_score + b.score;
		if (a.frame != b.frame) return a.frame > b.frame;
		return a.putindex > b.putindex;
	};

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


			if (!CanFireTower(second_field, base.base[1], -1, base.cannot_fire_index)) {
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
	// 1段目土台と底上げ土台
	for (int d : {0, Field::COLUMN }) {

		// oは土台ぷよ
		// vの有無で分岐
		// ..vo..
		// ...o..

		if (f.ColorEqual(d + C + 4, d + C * 2 + 4)) {
			Color p = f[d + C + 4];
/*			if (f.ColorEqual(d + C + 4, d + C * 2 + 3))
				return TowerBase(p, d + C * 2 + 3, d + C + 4, d + C * 2 + 4, 4, true);
			// ずらしタワー、2式5式を許さない
			if (f.ColorEqual(d + C + 4, d + C + 3) || f.ColorEqual(d + C + 4, d + C + 5) || f.ColorEqual(d + C + 4, d + C * 2 + 5))
				return TowerBase(p, 0, 0, 0, 0, true);
			else
				return TowerBase(p, d + C + 4, d + C * 2 + 4, 4, true);*/
			// 全て3連結タワーと仮定
			// ずらしタワー、2式5式を許さない
			if (f.ColorEqual(d + C + 4, d + C + 3) || f.ColorEqual(d + C + 4, d + C + 5) || f.ColorEqual(d + C + 4, d + C * 2 + 5) || f.ColorEqual(d + C + 4, d + C * 3 + 3))
				return TowerBase(p, 0, 0, 0, 0, true);
			return TowerBase(p, d + C * 2 + 3, d + C + 4, d + C * 2 + 4,
				d + C * 3 + 4, 4, true);
		}
		// ..ov..
		// ..o...
		if (f.ColorEqual(d + C + 3, d + C * 2 + 3)) {
			Color p = f[d + C + 3];
			if (f.ColorEqual(d + C + 3, d + C * 2 + 4))
				return TowerBase(p ,d + C * 2 + 4, d + C * 2 + 3, d + C + 3,
					d + C * 3 + 3, 3, false);
			// ずらしタワー、2式5式を許さない
			if (f.ColorEqual(d + C + 3, d + C + 4) || f.ColorEqual(d + C + 3, d + C + 2) || f.ColorEqual(d + C + 3, d + C * 2 + 2))
				return TowerBase(p,0, 0, 0, 0, false);
			else
				return TowerBase(p, d + C * 2 + 3, d + C + 3, d + C * 3 + 3, 3, false);
		}
	}
	// 土台構築失敗
	return TowerBase(EMPTY, 0, 0, 0, 0, true);
}

// @note:必ずタワーの発火点のいずれかをiに渡すこと
// iを壁にすると必ずfalseを返す。
// @note:必ず3連結以下になるindexを渡すこと。でないと無限ループが発生しうる
// @args:iは現在のindex, preは前回の位置のFieldIndex
bool NomiThink::CanFireTower(const Field& f, FieldIndex i, FieldIndex pre, FieldIndex out) {
	if (f[i] == Color::WALL) return false;
	bool ok = false;
	for (int d : dt) {
		if (i + d == pre || i + d == out) continue;
		if (f[i] == f[i + d]) ok |= CanFireTower(f, i + d, i, out);
		if (f[i + d] == Color::EMPTY) return true;
	}
	return ok;
}


TowerRate NomiThink::Waruagaki(const Field& second_field, Score fatal_dose, PutIndex pi, Frame puts_frame) {
	std::priority_queue<TowerRate, std::vector<TowerRate>, CompareTowerRate> third_que;
	for (Color c : {Color::RED, Color::GREEN, Color::YELLOW, Color::PURPLE}) {
		// ゾロを生成
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



	if (f_[Field::FIELD_DEATH] != EMPTY || f_[Field::FIELD_DEATH - 1] != EMPTY || f_[Field::FIELD_DEATH - 2] != EMPTY
		|| f_[Field::FIELD_DEATH + 2] != EMPTY
		|| f_[Field::FIELD_DEATH + 1] != EMPTY) {
		rate.potential_score -= 2000;
	}

	return rate;
}

Chain NomiThink::ActualChain(const Field& f_, const TowerBase& t_base) {
	Field f(f_);
	BaseDelete(&f, t_base);
	Simulator::FallAll(&f);
	// 2連鎖目開始でタワー発火
	return Simulator::Chain2Simulatation(&f);
}

TowerRate NomiThink::VirtualChain(const Field & f_, const TowerBase& t_base, Score fatal_dose) {
	Field second_tmp(f_);
	// 土台を消してぷよを落とす
	BaseDelete(&second_tmp, t_base);

	Simulator::FallAll(&second_tmp);

	// 念のため

	second_tmp.ModifyLowestEmptyRows();

	// 補完
	bool used[Field::FIELD_SIZE];
	std::fill_n(used, Field::FIELD_SIZE, false);
	int complement_ct = ComplementTower3(&second_tmp, used, t_base);

	// 一回目の補完で致死を超えていたらそれを採用
	Field tmp(second_tmp);
	Chain first_chain(Simulator::Simulate(&tmp,-1,-1,1));
//	Chain first_chain(Simulator::Chain2Simulatation(&tmp));
	if (first_chain.score + 40 >= fatal_dose)
		return TowerRate(first_chain.score, complement_ct, first_chain.frame);

	// 致死を越えなければ、2連鎖目のみを考慮に入れて１～３連結の補完を行う。
	// @because 2->3を3->4にする。
	std::fill_n(used, Field::FIELD_SIZE, false);
	complement_ct = ComplementTower1And2And3(&second_tmp, used, t_base);

	Chain second_chain(Simulator::Chain2Simulatation(&second_tmp));
	return TowerRate(second_chain.score, complement_ct, second_chain.frame);
}



// タワーの土台と土台周りのお邪魔ぷよを消す。
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

// first_indexから連結されているぷよの数を返す。
// ぷよは消さない。
// @note 1-12段目までを見る。
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

int NomiThink::ComplementTower1And2And3(Field* f, bool* used, const TowerBase& t_base) {

	int ct = 0;
	ct += ComplementTower1(f, used, t_base);

	ct += ComplementTower2(f, used, t_base);

	ct += ComplementTower3(f, used, t_base);


	return ct;
}

int NomiThink::ComplementTower1(Field* f, bool* used, const TowerBase& t_base) {
	int ct = 0;
	for (Row r = 1; r <= Field::VISIBLE_ROW; r++) {
		for (int c_i = 0; c_i < t_base.complement_column.size(); c_i++) {
			Column complement_c = t_base.complement_column[c_i];
			// リザーブ列の探査は正当なCOLUMNが代入されている時のみ
			if (complement_c == TowerBase::NOTEXIST) continue;
			FieldIndex ind = r * Field::COLUMN + complement_c;

			if (used[ind]) continue;

			const int d = t_base.GetDirect();

			ct += Complement1Connection(f, used, ind, c_i, 0, d, false);
		}
	}
	return ct;
}

int NomiThink::ComplementTower2(Field* f, bool* used, const TowerBase& t_base) {
	int ct = 0;
	for (Row r = 1; r <= Field::VISIBLE_ROW; r++) {
		for (int c_i = 0; c_i < t_base.complement_column.size(); c_i++) {
			Column complement_c = t_base.complement_column[c_i];
			// リザーブ列の探査は正当なCOLUMNが代入されている時のみ
			if (complement_c == TowerBase::NOTEXIST) continue;
			FieldIndex ind = r * Field::COLUMN + complement_c;

			if (used[ind]) continue;

			const int d = t_base.GetDirect();

			// 土台と補完位置が被って正しく補完出来ない場合
			if (c_i == 1 && r == 2
				&& t_base.color == (*f)[ind]) {
				ct += Complement2Connection(f, used, ind, ind + Field::COLUMN, c_i, 0, d, true);
				ct += Complement2Connection(f, used, ind, ind + d, c_i, 1, d, true);
			}

			ct += Complement2Connection(f, used, ind, ind + Field::COLUMN, c_i, 0, d, false);
			ct += Complement2Connection(f, used, ind, ind + d, c_i, 1, d, false);
		}
	}
	return ct;
}

// 3連結の補完を行う。
// @note towerbaseの[0]はリザーブ列であるが、ここにNOTEXISTが代入されていればスキップする。
int NomiThink::ComplementTower3(Field* f, bool* used, const TowerBase& t_base) {
	int ct = 0;
	for (Row r = 1; r <= Field::VISIBLE_ROW; r++) {
		for (int c_i = 0; c_i < t_base.complement_column.size(); c_i++) {
			Column complement_c = t_base.complement_column[c_i];
			// リザーブ列の探査は正当なCOLUMNが代入されている時のみ
			if (complement_c == TowerBase::NOTEXIST) continue;

			FieldIndex ind = r * Field::COLUMN + complement_c;

			if (used[ind]) continue;

			const int d = t_base.GetDirect();

			// 土台と補完位置が被って正しく補完出来ない場合
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

// base, j, kの3連結からなるぷよを補完して4連結にする。
// もし既に4連結以上の場合は補完は行わない。
int NomiThink::Complement3Connection(Field* f, bool* used, FieldIndex base, FieldIndex j, FieldIndex k, int c_i, int shape_i, int direct, bool base_conflict) {
	// 13段目のぷよが紛れている、又は色が違えばreturn
	if (!f->ColorEqual(base, j, k) || j > Field::VISIBLE_FIELD_END || k > Field::VISIBLE_FIELD_END) return 0;
	used[base] = true;
	used[j] = true;
	used[k] = true;
	if (LinkCount(*f, base) >= 4) return 0;
	return NomiMemory::Complement3(f, base, direct, base_conflict, c_i, shape_i);
}

int NomiThink::Complement2Connection(Field* f, bool* used, FieldIndex base, FieldIndex j, int c_i, int shape_i, int direct, bool base_conflict) {
	// 13段目のぷよが紛れている、又は色が違えばreturn
	if (!f->ColorEqual(base, j) || j > Field::VISIBLE_FIELD_END) return 0;
	used[base] = true;
	used[j] = true;
	if (LinkCount(*f, base) >= 4) return 0;
	return NomiMemory::Complement2(f, base, direct, base_conflict, c_i, shape_i);
}

int NomiThink::Complement1Connection(Field* f, bool* used, FieldIndex base, int c_i, int shape_i, int direct, bool base_conflict) {
	// 13段目のぷよが紛れている、又は色が違えばreturn
	if ( ! f->IsColor(base)) return 0;
	used[base] = true;
	if (LinkCount(*f, base) >= 4) return 0;
	return NomiMemory::Complement1(f, base, direct, base_conflict, c_i, shape_i);
}