#include "padsearch.h"


// @arg field, padは定数
// @arg kumipuyo, queは変更され得る。
// @arg kumipuyoはUNDOされるため不変
// @arg next_nodeはコピーされたものを使う。
// PushLeftについても同様
void PadSearch::PushMove(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode>* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down) {

	// 前フレームで左右移動回転を行わない時
	if (pre_pad.IsRight() || pre_pad.IsLeft()) return;

	bool is_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		Node next_dup_node(next_node);

		// 移動のPadセット
		if (is_left[dt]) next_dup_node.SetLeft(true);
		else next_dup_node.SetRight(true);
		if (down) next_dup_node.SetDown(true);

		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// 移動をさせる
		if (is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();
		if (down) simulation_kumipuyo.Down();

		// 移動後の座標が正当でないならcontinue
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		// 左右の移動を戻す
		if (!is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		next_dup_node.SetDistEstimated(next_dist, simulation_kumipuyo);

		pre_node->emplace(next_dup_node, now_node);
		que->push( next_dup_node);

	}
}


// @pre next_nodeのpadが初期化されている
// @arg field, padはconst参照。
// @arg queは変更され得る。
// @arg kumipuyo, next_nodeはコピーされたものを使う。
// @arg next_distは最短距離。
// PushRotateLeftについても同様

void PadSearch::PushRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down) {



	bool is_rot_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		// 前フレームで該当する回転を行っていないなら
		if (is_rot_left[dt] && pre_pad.IsRotateLeft()) continue;
		else if (!is_rot_left[dt] && pre_pad.IsRotateRight()) continue;

		Node next_dup_node(next_node);

		// pad set
		if (is_rot_left[dt]) next_dup_node.SetRotateLeft(true);
		else next_dup_node.SetRotateRight(true);
		if (down) next_dup_node.SetDown(true);

		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// 13段目の時、回転が90or270で目的のｘまで遠いなら回転させない
		if (simulation_kumipuyo.Y() == 12.0 &&
			std::abs(simulation_kumipuyo.X() - (now_kumipuyo.desirable_put.column - 1)) > 1 &&
			(simulation_kumipuyo.now_rotate == RotateType::ROTATE_90 ||
				simulation_kumipuyo.now_rotate == RotateType::ROTATE_270)) {
			return;
		}

		// 回転させる
		if (is_rot_left[dt]) simulation_kumipuyo.RotateLeft();
		else simulation_kumipuyo.RotateRight();
		if (down) simulation_kumipuyo.Down();



		// 回転後の座標が正当でないなら壁を蹴る
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			if (simulation_kumipuyo.now_rotate == RotateType::ROTATE_90) {
				simulation_kumipuyo.MoveLeft();
			}
			else if (simulation_kumipuyo.now_rotate == RotateType::ROTATE_180) {
				simulation_kumipuyo.Up();
			}
			else if (simulation_kumipuyo.now_rotate == RotateType::ROTATE_270) {
				simulation_kumipuyo.MoveRight();
			}

			// ここで正当でないならデスツインなので回転不可
			if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
				continue;
			}
		}

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		next_dup_node.SetDistEstimated(next_dist, simulation_kumipuyo);

		pre_node->emplace(next_dup_node, now_node);
		que->push(next_dup_node);

	}
}

// 移動がそもそも不正なら、回転のみ行う。回転も不正なら、壁蹴りor浮き上がり。それも不正になるなら、何も行わない
// 移動して、回転して、正当なら通常通り回転処理と移動予約（PAD)のみ
// 移動して、回転して、不正なら、先行壁蹴り。先行壁蹴りの結果が不正なら移動のみ行う。先行浮き上がりは不正には倣い

void PadSearch::PushMoveAndRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down) {

	// 前フレームで左右移動回転を行わない時
	if (pre_pad.IsRight() || pre_pad.IsLeft()) return;

	// 13danameで回転が一致するならこの関数を呼ぶ必要はない
	if (next_node.y == 12.0 && next_node.rotate == now_kumipuyo.desirable_put.rotate) return;

	bool is_left[4] = { true, false, true, false };
	bool is_rot_left[4] = { true, true, false, false };

	for (int dt = 0; dt < 4; dt++) {

		if (is_rot_left[dt] && pre_pad.IsRotateLeft()) continue;
		else if (!is_rot_left[dt] && pre_pad.IsRotateRight()) continue;

		Node next_dup_node(next_node);


		// 回転と移動のPadセット
		if(is_left[dt]) next_dup_node.SetLeft(true);
		else next_dup_node.SetRight(true);

		if(is_rot_left[dt]) next_dup_node.SetRotateLeft(true);
		else next_dup_node.SetRotateRight(true);

		if (down) next_dup_node.SetDown(true);

		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// 移動をさせる
		if(is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();
		if (down) simulation_kumipuyo.Down();

		// この時点で正当でないならリセット
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		// 回転もさせる
		if (is_rot_left[dt]) simulation_kumipuyo.RotateLeft();
		else simulation_kumipuyo.RotateRight();



		// 回転後の座標が正当でないなら壁を蹴る
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			if (simulation_kumipuyo.now_rotate == RotateType::ROTATE_90) {
				simulation_kumipuyo.MoveLeft();
			}
			else if (simulation_kumipuyo.now_rotate == RotateType::ROTATE_180) {
				simulation_kumipuyo.Up();
			}
			else if (simulation_kumipuyo.now_rotate == RotateType::ROTATE_270) {
				simulation_kumipuyo.MoveRight();
			}
		}

		// 移動を戻す
		if (!is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		// ここで正当でないならデスツインなので回転不可
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		next_dup_node.SetDistEstimated(next_dist, simulation_kumipuyo);

		pre_node->emplace(next_dup_node, now_node);
		que->push(next_dup_node);

	}
}

// 目的のPutIndexまで移動する操作列を返す
// 見つからなければ、長さ0のvectorを返す


std::vector<Pad> PadSearch::DropOrder(const Kumipuyo &kumipuyo_,
	const Field &field_, const Pad &previous_pad_, bool always_down) {
	const int MAX_DISTANCE = 14;
	// ノード追加のためのキュー
	std::priority_queue<Node, std::vector<Node>, CompareNode> que;
	// 各ノードまでの最短経路
	std::map < Node, int, DistinctNode > dist;
	// 経路復元
	std::map<Node, Node, DistinctNode > pre_nodes;
	Node start_n(kumipuyo_.X(), kumipuyo_.now_rotate, kumipuyo_.Y());
	start_n.SetPad(previous_pad_);
	start_n.SetDistEstimated(0, kumipuyo_);
	que.push(start_n);

	while (!que.empty()) {
		Node node;
		node = que.top(), que.pop();

		if (node.dist > MAX_DISTANCE) continue;

		// USED CHECK
		auto it = dist.find(node);
		if (it != dist.end()) { continue; }

		// Update
		dist.insert({ node, node.dist });

		Node next_node(node);
		next_node.pre_pad = 0;
		next_node.dist++;

		// 前回Padで移動入力がされているなら処理
		if (!(node.IsRight() && node.IsLeft())) {
			if (node.IsRight()) next_node.x += 1.0;
			if (node.IsLeft()) next_node.x -= 1.0;
		}

		// END CHECK
		if (kumipuyo_.desirable_put.column - 1 == next_node.x &&  kumipuyo_.desirable_put.rotate == next_node.rotate) {
			std::vector<Pad> ans;
			ans.push_back(node.GetPad());
			while (!node.Equal(start_n)) {
				ans.push_back(pre_nodes[node].GetPad());
				node = pre_nodes[node];
			}
			std::reverse(ans.begin(), ans.end());

			return ans;
		}

		PushMove(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, next_node.dist, always_down);
		if (node.rotate != kumipuyo_.desirable_put.rotate) {
			PushRotate(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, next_node.dist, always_down);
			if (always_down) {
				PushMoveAndRotate(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, next_node.dist, always_down);
			}
		}

			// 何もしない
		if (node.pre_pad != 0) {
			pre_nodes.emplace(next_node, node);
			que.push(next_node);
		}

	}
	return std::vector<Pad>();
}

// @note 必ずDropOrderが全て実行された後で呼ぶこと。
std::queue<Pad> PadSearch::CancelDrop(const Kumipuyo & kumipuyo, const Field & field, const Pad & previous_pad)
{
	if (kumipuyo.now_rotate != RotateType::ROTATE_0 || ! kumipuyo.IsDesirablePut()) {
		return std::queue<Pad>();
	}
	FieldIndex parent_fi, _;
	std::tie(parent_fi, _) = Simulator::CalculatePutIndex(kumipuyo.desirable_put, field);
	if (field[parent_fi + 1] != Color::EMPTY && field[parent_fi - 1] != Color::EMPTY) {
		return std::queue<Pad>();
	}
	// キーをリセットするために0.5段落とす
	std::queue<Pad> que;
	Pad redundant_pad;
	redundant_pad.SetDown(true);
	que.push(redundant_pad);
	Kumipuyo k(kumipuyo);
	k.Down();

	while (k.Y() > static_cast<float>(parent_fi / Field::COLUMN - 1) + 1.0f) {
		que.push(redundant_pad);
		k.Down();
	}
	if (k.Y() != static_cast<float>(parent_fi / Field::COLUMN - 1) + 1.0f) return que;
	// 右方向へキャンセル
	bool right_cancel = field[parent_fi + 1] == Color::EMPTY;
		Pad pad;

		if(right_cancel) pad.SetLeft(true);
		else             pad.SetRight(true);
		pad.SetDown(true);
		que.push(pad); pad.SetNeutral();

		pad.SetDown(true);
		if(right_cancel) pad.SetRotateRight(true);
		else             pad.SetRotateLeft(true);
		que.push(pad); pad.SetNeutral();

		if(right_cancel) pad.SetRight(true);
		else             pad.SetLeft(true);
		que.push(pad); pad.SetNeutral();

		if (right_cancel) pad.SetRotateLeft(true); 
		else              pad.SetRotateRight(true);
		pad.SetDown(true);
		que.push(pad); pad.SetNeutral();

		pad.SetDown(true);
		que.push(pad); pad.SetNeutral();
		return que;
}
