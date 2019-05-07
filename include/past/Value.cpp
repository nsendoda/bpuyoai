#include "Value.h"



Value::Value():sum_chain(0,0,0)
{
}


Value::~Value()
{
}

// @todo 壁越え判定を行っていないので必須
/// 一時的に中身を変えるのでconstでない参照
void Value::CaluculatePotential(const Field& field, int put_frame, int firstput_column) {
	Field simulational_field(field);
	for (int column_i = 1; column_i < Field::COLUMN; ++column_i) {
		for (Color c = Parameter::RED; c <= Parameter::PURPLE; ++c) {

			int put_index = Simulator::PutOnePuyoAndGetIndex(&simulational_field, column_i, c);
			Chain chain = Simulator::Simulate(&simulational_field, put_index, -1);
			// 最初の呼び出し、つまりぷよ1個目なら2個目を探索
			// 連鎖が起こっている場合は更新できるか確認
			// この際、置くまでの時間も加算
			if (chain.score > 0) {
				chain.frame += put_frame;
				if(firstput_column == 0) SetChainIfMaxScore(chain, 1, false);
				else if (abs(firstput_column - column_i) <= 1) SetChainIfMaxScore(chain, 2, true);
				else SetChainIfMaxScore(chain, 2, false);
				if(firstput_column == 0) CaluculatePotential(field, column_i, firstput_column + 1);
				simulational_field = field; /// UNDO
			}
			else {
				if(firstput_column == 0) CaluculatePotential(field, column_i, firstput_column + 1);
				Simulator::EliminateOnePuyo(&simulational_field, column_i); /// UNDO
			}
		}
	}
}

void Value::ConnectionPotential(const Field& field, int put_frame) {
	Field simulational_field(field);
	std::vector<std::vector<int> > connections(CaluculateConnections(simulational_field));
	/// CONNECTION DELETE
	for (int connect_i = 0; connect_i < connections.size(); ++connect_i) {
		DeleteFieldConnection(&simulational_field, connections[connect_i]);

		int first_chaindrop_frame = Simulator::GetDropRowAndFallAll(&simulational_field);
		Chain connect_delete_chain(Simulator::Simulate(&simulational_field, -1, -1, 1));
		if(connections[connect_i].size() == 2) connect_delete_chain.score *= 0.9;

		connect_delete_chain.frame += first_chaindrop_frame;

		max_chain = (std::max)(connect_delete_chain, max_chain);
		sum_chain += connect_delete_chain;
		++connection_tochain_num;
		/// CONNECTION COUNT
		std::vector<std::vector<int> > chainend_connections = CaluculateConnections(simulational_field);
		for (int i = 0; i < chainend_connections.size(); i++) {
			if (chainend_connections.size() == 2) sum_chain.score += 100;
			if (chainend_connections.size() == 3) sum_chain.score += 300;
		}


		/// UNDO
		simulational_field = field;
	}
	sum_chain.score /= connection_tochain_num;
}


void Value::ConnectionAddedPotential(const Field& field, int put_frame) {
	Field simulational_field(field);
	std::vector<std::vector<int> > connections(CaluculateConnections(simulational_field));
	for (int column_i = 2; column_i < Field::COLUMN - 1; ++column_i) {
		for (Color c = Parameter::RED; c <= Parameter::PURPLE; ++c) {
			int put_index = Simulator::PutOnePuyoAndGetIndex(&simulational_field, column_i, c);
			Chain chain = Simulator::Simulate(&simulational_field, put_index, -1);
			if (chain.score > 0) {
				/// UNDO
				simulational_field = field;
				continue;
			}
			int connection_group_index = AddOnePuyoToConnections(&connections, simulational_field, put_index);

			Chain tmp_max_chain, tmp_sum_chain(0,0,0);
			int tmp_connection_tochain_num = 0;

			/// CONNECTION DELETE
			for (int connect_i = 0; connect_i < connections.size(); ++connect_i) {
				DeleteFieldConnection(&simulational_field, connections[connect_i]);

				int first_chaindrop_frame = Simulator::GetDropRowAndFallAll(&simulational_field);
				Chain connect_delete_chain(Simulator::Simulate(&simulational_field, -1, -1, 1));
				if(connections[connect_i].size() == 2) connect_delete_chain.score *= 0.9;

				connect_delete_chain.frame += first_chaindrop_frame;

				tmp_max_chain = (std::max)(connect_delete_chain, tmp_max_chain);
				tmp_sum_chain += connect_delete_chain;
				++tmp_connection_tochain_num;

				/// UNDO
				simulational_field = field;
			}

			/// UPDATE
			/// 色ごとの尖った部分を観たいので、混じっても良い
			if (tmp_max_chain.GreaterKillerThan(max_chain)) max_chain = tmp_max_chain;
			sum_chain = (std::max)(tmp_sum_chain, sum_chain);
			connection_tochain_num = (std::max)(tmp_connection_tochain_num, connection_tochain_num);

			/// CONNECTION UNDO
			if (connection_group_index != -1) {
				if(connection_group_index == -2) connections.pop_back();
				else connections[connection_group_index].pop_back();

			}
		}
	}
}

bool Value::GreaterThanByConnectionValueWay(const Value& value) const{
	if (max_chain.score > Parameter::LOWER_FATAL_DOSE) {
		if (value.max_chain.score < Parameter::LOWER_FATAL_DOSE) {
			return true;
		}
		int frame_diff = (max_chain.frame - value.max_chain.frame) * 2;
		int score_diff = max_chain.score - value.max_chain.score;
		return frame_diff + score_diff > 0;
	}
	else {
		if (value.max_chain.score > Parameter::LOWER_FATAL_DOSE) {
			return false;
		}
		int sum_score_diff = sum_chain.score - value.sum_chain.score;
		int max_score_diff = max_chain.score - value.max_chain.score;
		int chain_diff = max_chain.number - value.max_chain.number;
		return sum_score_diff + max_score_diff - std::pow(chain_diff,5) > 0;
	}
}


/// fieldに繋がっている連結をそれぞれ返す
/// 何連結以上を使うかは以下の通り
const int CONNECTION_LOWER_BOUND = 2;
const int FIRE_HEIGHT_UPPER_BOUND = 3;
std::vector<std::vector<int> > Value::CaluculateConnections(const Field& field)const {
	std::vector<std::vector<int> > connections;
	bool used[Field::FIELD_SIZE];
	std::fill_n(used, Field::FIELD_SIZE, false);
	for (int i = Field::FIELD_START; i <= Field::Field::FIELD_END; i++) {
		if (field[i] == Parameter::EMPTY || field[i] == Parameter::WALL || field[i] == Parameter::OJAMA) continue;
		/// 左、下は探索済み
		if (field[i] == field[i - Field::COLUMN] || field[i] == field[i - 1] || used[i]) continue;
		std::queue<int> que;
		que.push(i);
		std::vector<int> tmp_connections;
		bool isempty = false;
		while (!que.empty()) {
			int j = que.front();
			que.pop();
			tmp_connections.push_back(j);
			used[j] = true;
			for (int d : {-Field::COLUMN, 1, -1, Field::COLUMN}) {
				/// 該当部分が空で、発火点として高すぎず、発火点に合法な設置が可能で、他の連結を巻き込まないならOK 
				bool fire_collision = false;
				for (int dd : {-Field::COLUMN, 1, -1, Field::COLUMN}) {
					if (j == j + d + dd) continue;
					if (field[j + d + dd] == field[j]) {
						fire_collision = true;
						break;
					}
				}
				if (field[j + d] == Parameter::EMPTY 
					&& (j+d) / Field::COLUMN - field.GetLowestEmptyRowsIndex((j+d)%Field::COLUMN) < FIRE_HEIGHT_UPPER_BOUND
					&& field.CanDropOnePuyo((j+d) %Field::COLUMN)
					&& !fire_collision) isempty = true;
				if (used[j + d] || field[j + d] != field[j]) continue;
				que.push(j + d);
			}
		}
		if (tmp_connections.size() >= CONNECTION_LOWER_BOUND && isempty){
			connections.push_back(tmp_connections);
		}
	}
	return connections;
}

void Value::DeleteFieldConnection(Field* field, const std::vector<int>& deletes)const {
	for (int delete_index : deletes){
		(*field)[delete_index] = Parameter::EMPTY;
	}
}

/// ぷよを置いた時にできた新しい連結グループを作成する
/// 新しい連結グループが出来た時は-2を返す
/// 見つからなかった時は何もせずに-1を返す
int Value::AddOnePuyoToConnections(std::vector<std::vector<int> >* connections, const Field& field, int put_index)const {

	for (int i = 0; i < (*connections).size(); ++i) {
		for (int puyo_index : (*connections)[i]) {
			for (int k : {-Field::COLUMN, -1, 1, Field::COLUMN}) {
				/// 隣接しているグループと色が同じなら
				if (put_index + k == puyo_index && field[put_index] == field[puyo_index]) {
					(*connections)[i].push_back(put_index);
					return i;
				}
			}
		}
	}

	std::vector<int> new_connect;
	for (int d : {-Field::COLUMN, -1, 1, Field::COLUMN}){
		if (field[put_index] == field[put_index + d]) {
			new_connect.push_back(put_index + d);
		}
	}
	if (new_connect.size() >= 2) {
		new_connect.push_back(put_index);
		connections->push_back(new_connect);
		return -2;
	}
	else {
		return -1;
	}
}

void Value::SetChainIfMaxScore(const Chain& chain, int puyo_num, bool kumipuyo) {
	if (puyo_num == 1) {
		if (chain.score > one_puyo_max_score_chain.score) {
			one_puyo_max_score_chain = chain;
		}
	}
	else if (kumipuyo == true) {
		if (chain.score > kumipuyo_max_score_chain.score) {
			kumipuyo_max_score_chain = chain;
		}
	}
	else if (puyo_num == 2) {
		if (chain.score > two_puyo_max_score_chain.score) {
			two_puyo_max_score_chain = chain;
		}
	}
	else {
		ASSERT_MESSAGE(false, "Value::SetChainIfMaxScore error : puyo_num = %d\n", puyo_num)
	}
}