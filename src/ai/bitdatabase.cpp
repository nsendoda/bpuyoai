#include "bitdatabase.h"


const PutIndex BitDatabase::REVERSE_PUTS[PUTTYPE_PATTERN] =
{ 2, 6, 0,
	5, 10, 3, 1,
	9, 14, 7, 4,
	13, 18, 11, 8,
	17, 21, 15, 12,
	20, 19, 16 };

std::pair<BitData, bool> BitDatabase::Bit(const Field& field_, const Kumipuyo& now_, const Kumipuyo& next_) const {
	__int64
		f1 = 0,
		f2 = 0,
		f3 = 0,
		f4 = 0,
		now = 0,
		next = 0;

	std::map<Color, int> color_map = {
		{Color::RED, 0},
	{Color::GREEN, 0},
	{Color::PURPLE, 0},
	{Color::YELLOW, 0},
	{Color::BLUE, 0}
	};
	int color_number = 1;

	int color_puyo_ct = 0;
	for (FieldIndex i = Field::FIELD_START;
		i <= Field::FIELD_END; i++) {
		if (field_[i] == Color::WALL) continue;
		int unique_c = 0;
		if (field_[i] > Color::EMPTY && field_[i] < Color::OJAMA) {
			if (color_map.at(field_[i]) == 0) {
				color_map[field_[i]] = color_number;
				color_number++;
			}
			unique_c = color_map.at(field_[i]);
		}

		if (color_puyo_ct < VARIABLE_PUYO_SIZE) {
			f1 |= unique_c << ((color_puyo_ct % VARIABLE_PUYO_SIZE) * PUYO_BIT);
		}
		else if (color_puyo_ct < 2 * VARIABLE_PUYO_SIZE) {
			f2 |= unique_c << ((color_puyo_ct % VARIABLE_PUYO_SIZE) * PUYO_BIT);
		}
		else if (color_puyo_ct < 3 * VARIABLE_PUYO_SIZE) {
			f3 |= unique_c << ((color_puyo_ct % VARIABLE_PUYO_SIZE) * PUYO_BIT);
		}
		else {
			f4 |= unique_c << ((color_puyo_ct % VARIABLE_PUYO_SIZE) * PUYO_BIT);
		}

		color_puyo_ct++;

	}

	// F‚ªŽá‚¢‚È‚ç”½“]‚³‚¹‚é‚±‚Æ‚É’ˆÓ
	auto kumipuyo_bit = [&](const Kumipuyo& k) -> std::pair<__int64, bool> {
		__int64 ret = 0;
		int unique_p = 0;
		int unique_c = 0;
		if (color_map.at(k.parent) == 0) {
			color_map[k.parent] = color_number;
			color_number++;
		}
		unique_p = color_map[k.parent];
		if (color_map.at(k.child) == 0) {
			color_map[k.child] = color_number;
			color_number++;
		}
		unique_c = color_map[k.child];

		bool swapped = false;
		if (unique_c < unique_p) {
			std::swap(unique_p, unique_c);
			swapped = true;
		}
		ret |= unique_p;
		ret |= unique_c << PUYO_BIT;
		return { ret, swapped };
	};

	bool reversed, __;

	std::tie(now, reversed) = kumipuyo_bit(now_);
	std::tie(next, __) = kumipuyo_bit(next_);

	return std::make_pair(BitData(f1, f2, f3, f4, now, next), reversed);
}