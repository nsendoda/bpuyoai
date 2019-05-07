#include "Simulator.h"



Simulator::Simulator()
{
}




 
/// フィールドを引数にとり, その状態から始まる連鎖の得点と連鎖数と連鎖時間を返す.
/// よって, 連結数4以上のぷよが一つもない時には, score, chainNum,time_chainともに0となる.
/// 発見した連鎖数が０の時は組みぷよ周りだけ、
/// 　　　　　　　　１以上の時は全部を見ていく
/// @note chain_numはデフォルト引数0. 1だと既に1連鎖が行われた状態で始まる
/// @pre 全てのぷよが落下済み
Chain Simulator::Simulate(Field* field_, int parent_index, int child_index, int chain_num) {
	Field& field = *field_;
	int score = 0;
	int chain_frame = 0;
	while (true) {
		bool isDisappeared = false;

		bool color[Parameter::WALL] = { false }; /* 連結していたぷよの色のindexにtrueが格納される配列 */
		int colorNum = 0; /* 連結していたぷよの色の種類 */
		int disappeardNum = 0; /* この1連鎖で消えたぷよの量の和 */
		int bonus_chain, bonus_link, bonus_color; /* 各ボーナスの値が格納される */

		bonus_link = 0; /* 連結ボーナス */
		/*
			* 4つ以上連結しているぷよを探す.
			* 見つけたら, そのぷよたちを消す. 
			*/
		for (int i, ct = 0; ; ) {
			if (chain_num == 0) {
				if (ct == 0) i = parent_index;
				else if (ct == 1) i = child_index;
				else break;
				ct++;
				if (i < Field::FIELD_START || i > Field::FIELD_END) {
					continue;
				}

			}
			else {
				if (ct == 0) {
					ct++;
					i = Field::FIELD_START;
				}
				else {
					i++;
					if (i > Field::FIELD_END) break;
				}
			}


			bool linked[Field::FIELD_SIZE] = { false }; /* 連結している場所にtrueが格納される配列 */
			/*  連鎖数が1以上で, 下か左が同じ色のぷよの場合, 連結数を再探索する必要はない */
			if (chain_num > 0 && (field[i] == field[i - Field::COLUMN] || field[i] == field[i - 1])) {
				continue;
			}

			if (field[i] == Parameter::EMPTY || field[i] == Parameter::OJAMA) {
				continue;
			}

			/* 連結数を取得する */
			int linkNum = CaluculateLinkCount(field, i, linked);
			/* 4つ以上連結していたら */
			if (linkNum >= 4) {
			isDisappeared = true;
			disappeardNum += linkNum;
			bonus_link += Parameter::kBonusLink[linkNum];
			/* 連結しているぷよの色を保存 */
			if (!color[field[i]]) {
				color[field[i]] = true;
				colorNum++;
			}
			/* 連結しているぷよを消す */
			/// @todo 関数に書き出す deletelink
			DeleteLink(&field, i, chain_num, linked);
			}
		}
		if (isDisappeared) {
			/* ぷよが消えているので, ぷよたちを落下させる. 同時に連鎖時間も取得. */
			chain_frame += Parameter::kTimeOneChainAndDrop[GetDropRowAndFallAll(field_)];
			/* 連鎖数を増やす */
			chain_num++;
			/* 得点の計算 */
			bonus_chain = Parameter::kBonusChain[chain_num];
			bonus_color = Parameter::kBonusColor[colorNum];
			/* ボーナスの和が0なら, ボーナス全体を1として計算するため, ここでは連鎖ボーナスを1として代える. */
			if (bonus_chain + bonus_link + bonus_color == 0) {
			bonus_chain = 1;
			}
			score += 10 * disappeardNum * (bonus_chain + bonus_link + bonus_color);
		} else {
			break;   
		}
	}
	//scoreがfatal_doseを超えてる場合矯正
	if (score > Parameter::UPPER_FATAL_DOSE)score = Parameter::UPPER_FATAL_DOSE;
	return Chain(chain_num, score, chain_frame);
}

void Simulator::DeleteLink(Field* field_, int i, int chain_num, bool linked[]) {
	Field& field = *field_;
	if (chain_num == 0) {
		std::queue<int> que;
		que.push(i);
		bool used[Field::FIELD_SIZE];
		std::fill_n(used, Field::FIELD_SIZE, false);
		while (!que.empty()) {
			int j = que.front();
			que.pop();
			field[j] = Parameter::EMPTY;
			used[j] = true;
			for (int d : { -Field::COLUMN, +1, +Field::COLUMN, -1 }) {
				if (used[j + d] || !linked[j + d]) continue;
				que.push(j + d);
			}
		}
	}
	else {
		for (int j = i; j <= Field::FIELD_END; j++) {
			if (linked[j]) {
				field[j] = Parameter::EMPTY;
				/* お邪魔ぷよを消す */
				int d[4] = { -Field::COLUMN, +1, +Field::COLUMN, -1 };
				for (int k = 0; k < 4; k++) {
					if (field[j + d[k]] == Parameter::OJAMA) {
						field[j + d[k]] = Parameter::EMPTY;
					}
				}
			}
		}
	}
}

/*
 * 浮いているぷよをすべて落下させる関数.
 * 落下段数を返す.
 * 連鎖シュミレート時のぷよの落下にのみ使う.
 */
int Simulator::GetDropRowAndFallAll(Field* field_) {
	Field& field = *field_;
	int num_max_drop_row = 0;
	/// 右の列から各ぷよの落ちる段数を計算する
	for (int x = Field::FIELD_START; x < Field::FIELD_START + Field::COLUMN - 1; ++x) {
		bool isLocalFall = false;
		int f[Field::ROW] = { 0 };
		int emptyCount = 0;
		int num_drop_row = 0;
		for (int y = 0; y < Field::ROW - 2; y++) {
			if (field[x + Field::COLUMN * y] == Parameter::EMPTY) {
				emptyCount++;
			} else {
				f[y] = emptyCount;
				if (emptyCount > 0) {
					num_drop_row = emptyCount;
					isLocalFall = true;
				}
			}
		}
		/// 実際にぷよを落下させる
		if (isLocalFall) {
			for (int y = 0; y < Field::ROW - 2; y++) {
				if (f[y] > 0) {
					field[x + Field::COLUMN * (y - f[y])] = field[x + Field::COLUMN * y];
					field[x + Field::COLUMN * y] = Parameter::EMPTY;
				}
			}
			field.AddLowestEmptyRowsIndex(x % Field::COLUMN, - num_drop_row);
			num_max_drop_row = max(num_drop_row, num_max_drop_row);
		}
	}
	return num_max_drop_row;
}


/// 連結数を計算する関数.
/// linkedに, 連結しているぷよの場所にtrueが立った配列が代入される.
/// 返り値は連結数.
int Simulator::CaluculateLinkCount(const Field& field, int index, bool* linked) {
	// indexにぷよがない、又は13段目にいるなら0を返す.
	if (field[index] == Parameter::EMPTY || 
		field[index] == Parameter::WALL  || 
		field[index] == Parameter::OJAMA ||
		       index >= 13 * Field::COLUMN){
		return 0;
	}
	linked[index] = true;
	int link_ct = 1;
	static const int dx[4] = { -Field::COLUMN, +1, +Field::COLUMN, -1 };
	for (int d = 0; d < 4; ++d) {
		if (field[index] == field[index + dx[d]]) {
			if (linked[index + dx[d]]) {
				continue;
			}
			link_ct += CaluculateLinkCount(field, index + dx[d], linked);
		}
	}
	return link_ct;
}

