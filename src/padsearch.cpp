#include "padsearch.h"


// @arg field, padは定数
// @arg kumipuyo, queは変更され得る。
// @arg kumipuyoはUNDOされるため不変
// @arg next_nodeはコピーされたものを使う。
// PushLeftについても同様
void PadSearch::PushMove(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::queue<std::pair<int, Node> >* que, std::map<Node, Node >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist) {

	// 前フレームで左右移動回転を行わない時
	if (pre_pad.IsRight() || pre_pad.IsLeft()) return;

	bool is_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		Node next_dup_node(next_node);

		// 移動のPadセット
		if (is_left[dt]) next_dup_node.SetLeft(true);
		else next_dup_node.SetRight(true);

		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// 移動をさせる
		if (is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		// 移動後の座標が正当でないならcontinue
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		// 移動を戻す
		if (!is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		pre_node->emplace(next_dup_node, now_node);
		que->push({ next_dist, next_dup_node });

	}
}


// @pre next_nodeのpadが初期化されている
// @arg field, padはconst参照。
// @arg queは変更され得る。
// @arg kumipuyo, next_nodeはコピーされたものを使う。
// @arg next_distは最短距離。
// PushRotateLeftについても同様

void PadSearch::PushRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::queue<std::pair<int, Node> >* que, std::map<Node, Node >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist) {



	bool is_rot_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		// 前フレームで該当する回転を行っていないなら
		if (is_rot_left[dt] && pre_pad.IsRotateLeft()) continue;
		else if (!is_rot_left[dt] && pre_pad.IsRotateRight()) continue;

		Node next_dup_node(next_node);

		// pad set
		if (is_rot_left[dt]) next_dup_node.SetRotateLeft(true);
		else next_dup_node.SetRotateRight(true);


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
		pre_node->emplace(next_dup_node, now_node);
		que->push({ next_dist, next_dup_node });

	}
}

// 移動がそもそも不正なら、回転のみ行う。回転も不正なら、壁蹴りor浮き上がり。それも不正になるなら、何も行わない
// 移動して、回転して、正当なら通常通り回転処理と移動予約（PAD)のみ
// 移動して、回転して、不正なら、先行壁蹴り。先行壁蹴りの結果が不正なら移動のみ行う。先行浮き上がりは不正には倣い

void PadSearch::PushMoveAndRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::queue<std::pair<int, Node> >* que, std::map<Node, Node >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist) {

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


		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// 移動をさせる
		if(is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

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

			// 移動を戻す
			if ( ! is_left[dt]) simulation_kumipuyo.MoveLeft();
			else simulation_kumipuyo.MoveRight();

			// ここで正当でないならデスツインなので回転不可
			if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
				continue;
			}
		}

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		pre_node->emplace(next_dup_node, now_node);
		que->push({ next_dist, next_dup_node });

	}
}

// 下を押さずに、目的のPutIndexまで移動する操作列を返す
// 見つからなければ、長さ0のvectorを返す
std::vector<Pad> PadSearch::CarefulOrder(const Kumipuyo &kumipuyo_,
	const Field &field_, const Pad &previous_pad_) {

	// ノード追加のためのキュー
	std::queue<std::pair<int, Node> > que;
	// 各ノードまでの最短経路
	std::map < Node, int > dist;
	// 経路復元
	std::map<Node, Node > pre_nodes;
	Node start_n( kumipuyo_.X(), kumipuyo_.now_rotate, kumipuyo_.Y() );
	start_n.SetPad(previous_pad_);
	que.push({ 0, start_n });

	while (!que.empty()) {
		int d;
		Node node;
		std::tie(d, node) = que.front(), que.pop();

		// USED CHECK
		int ct = dist.count(node);
		auto it = dist.find(node);
		if (it != dist.end()) { continue; }

		// Update
		dist.insert({ node, d });
		d++;

		Node next_node(node.x, node.rotate, node.y, 0);

		Debug::Print("nodoe pre pad %d\n", node.pre_pad);

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



		Kumipuyo now_kumipuyo(node.x, node.rotate, node.y);



		PushMove(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, d);
		PushRotate(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, d);
	//	PushMoveAndRotate(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, d);

		// 何もしない
		if(node.pre_pad != 0){
			pre_nodes.emplace(next_node, node);
			que.push({ d, next_node });
		}

	}
	return std::vector<Pad>();
}

std::vector<Pad> PadSearch::DropOrder(const Kumipuyo &kumipuyo_,
                                          const Field &field_, const Pad &previous_pad_) {

  std::queue<std::pair<Kumipuyo, std::vector<Pad>>> que;
	const int DEFAULT_SIZE = 10;
  std::vector<Pad> best_orders(DEFAULT_SIZE);
	que.push({ kumipuyo_, {previous_pad_} });
	while (!que.empty()) {
		Kumipuyo now_kumipuyo;
		std::vector<Pad> pads;
		std::tie(now_kumipuyo, pads) = que.front();
		que.pop();

		Pad& previous_pad = pads.back();
		Pad pad;

		// 16操作以内に完結しないor現在の最短手順より長くなったら終了
		if (pads.size() > DEFAULT_SIZE || best_orders.size() < pads.size()) {
			break;
		}
		 
		// 最終的に置く場所のIndexを取得
		int put_parent_index, put_child_index;
		std::tie(put_parent_index, put_child_index) =
			Simulator::CalculatePutIndex(now_kumipuyo.desirable_put, field_);


		// 前フレームで左右に移動しているなら移動処理
		bool previous_right = pads.back().IsRight();
		bool previous_left = pads.back().IsLeft();
		if (previous_right) {
			now_kumipuyo.MoveRight();
			if (!Simulator::ValidPosition(now_kumipuyo, field_)) now_kumipuyo.MoveLeft();
		}
		else if (previous_left){
			now_kumipuyo.MoveLeft();
			if (!Simulator::ValidPosition(now_kumipuyo, field_)) now_kumipuyo.MoveRight();
		}

		// 終了check
		// 回転方向と位置が一致しているなら、連続落下
		if (now_kumipuyo.IsDesirablePut()) {
			// 組みぷよのIndexを取得
			int parent_index, child_index;
			std::tie(parent_index, child_index) = now_kumipuyo.GetPairIndex();

			// downpad生成
			Pad pad;
			pad.SetDown(true);
			while (true) {
				// 設置される場所が一致するまで回す
				// ちぎりによって片方のindexがずれることも考慮
				if ((parent_index == put_parent_index || child_index == put_child_index)
					&& !now_kumipuyo.YHasPointFive()) {
					break;
				}
				pads.push_back(pad);
				now_kumipuyo.Down();
				std::tie(parent_index, child_index) = now_kumipuyo.GetPairIndex();
			}


			// ちぎる場合は片方が一致してればよい
			if (best_orders.size() > pads.size()) {
				best_orders = pads;
			}
			continue;
		}

		// 設置処理に入らないindexなら落下
		auto down_push = [&] {
			if (!(now_kumipuyo.EqualsIndex(put_parent_index, put_child_index))
			|| now_kumipuyo.YHasPointFive()) {
			pad.SetDown(true);
			now_kumipuyo.Down();
			pads.push_back(pad);
			que.push({ now_kumipuyo, pads });
			pads.pop_back();
			// UNDO
			pad.SetDown(false);
			now_kumipuyo.DownUndo();
		 }
		};

		// 前フレームで左右入力が入っていないなら入力を考える
		if (!previous_right && !previous_left) {
			// 壁がない
			if (now_kumipuyo.GetColumn() < now_kumipuyo.desirable_put.column) {
				// 右へ移動をする。
				now_kumipuyo.MoveRight();
				// 置きたい方向へ行ける
				if (Simulator::ValidPosition(now_kumipuyo, field_)) {
					pad.SetRight(true);
				}
				// UNDO
				now_kumipuyo.MoveLeft();
			}
			// 左へ
			else if (now_kumipuyo.GetColumn() > now_kumipuyo.desirable_put.column) {
				now_kumipuyo.MoveLeft();
				if (Simulator::ValidPosition(now_kumipuyo, field_)) {
					pad.SetLeft(true);
				}
				// UNDO
				now_kumipuyo.MoveRight();
			}
		}

		// 回転方向が一致してないかつ前フレームで回転を行わない時
		// @note 2フレーム間で右回転→左回転を行う操作はぷよを浮かす時に使うが、
		//       この関数では扱わない
		if (now_kumipuyo.now_rotate != now_kumipuyo.desirable_put.rotate
			&& !previous_pad.IsRotateRight() && !previous_pad.IsRotateLeft()) {
			// @note 適切な回転方向を計算はしない
			//       壁蹴りの影響で逆回転でないといけない場合もあるため
			for (int left_mode = 0; left_mode <= 1; left_mode++) {
				bool kicked_wall = false;
				bool can_rotate_right = true;
				if (left_mode == 0) {
					pad.SetRotateRight(true);
					now_kumipuyo.RotateRight();
				}
				else {
					pad.SetRotateLeft(true);
					now_kumipuyo.RotateLeft();
				}
				// 正当でないなら壁蹴り
				if (!Simulator::ValidPosition(now_kumipuyo, field_)) {
					kicked_wall = true;
					if (left_mode == 0) now_kumipuyo.MoveLeft();
					else now_kumipuyo.MoveRight();
					// ここで正当でないならデスツインなので回転不可
					if (!Simulator::ValidPosition(now_kumipuyo, field_)) {
						can_rotate_right = false;
					}
				}
				// 回転キャンセル
				// Bぷよでは、回転と移動を同時に行った結果壁蹴りの座標にいった場合、先行壁蹴りが発生する
				// 先行壁蹴りをした場合、壁蹴り→左右入力（今回右）で正常な座標へ行く。
				// この時、壁蹴りで←に一列余分に移動するが、ここがぷよで埋まってると壁蹴りが失敗し、
				// 回転がされなくなる。
				// その場合はスキップする。
				else if ((pad.IsRight() && left_mode == 0 ) || (pad.IsLeft() && left_mode == 1) ) {
					if (left_mode == 0) now_kumipuyo.MoveRight();
					else now_kumipuyo.MoveLeft();
					// 移動した先で壁蹴りが発生する場合
					if (!Simulator::ValidPosition(now_kumipuyo, field_)) {
						kicked_wall = true;
						if (left_mode == 0) {
							now_kumipuyo.MoveLeft();
							now_kumipuyo.MoveLeft();
						}
						else {
							now_kumipuyo.MoveRight();
							now_kumipuyo.MoveRight();
						}
						if (!Simulator::ValidPosition(now_kumipuyo, field_)) {
							can_rotate_right = false;
						}
					}
					// UNDO
					// 壁蹴り時は後でUNDO
					else {
						if (left_mode == 0) now_kumipuyo.MoveLeft();
						else now_kumipuyo.MoveRight();
					}

				}

				// 回転push, 回転落下push
				if (can_rotate_right) {
					pads.push_back(pad);
					que.push({ now_kumipuyo, pads });
					pads.pop_back();
					down_push();
				}


				// UNDO
				if (kicked_wall) {
					if (left_mode == 0) now_kumipuyo.MoveRight();
					else now_kumipuyo.MoveLeft();
				}
				if (left_mode == 0) {
					pad.SetRotateRight(false);
					now_kumipuyo.RotateLeft();
				}
				else {
					pad.SetRotateLeft(false);
					now_kumipuyo.RotateRight();
				}
			}
		}
		else {
			// 回転方向一致時, 又は回転待ち
			// 左右Down or Down
			down_push();
			// 左右入力のみ、又はNEUTRALのpush
			pads.push_back(pad);
			que.push({ now_kumipuyo, pads });
			pads.pop_back();
		}
	}

	if (best_orders.size() == DEFAULT_SIZE) return std::vector<Pad>();
  return best_orders;
}

bool PadSearch::CanRightDown(const Kumipuyo &kumipuyo, const Field &field) {
  int parent_index, child_index;
  std::tie(parent_index, child_index) = kumipuyo.GetPairIndex();

  bool can_moveright =
      field[parent_index + 1] == EMPTY && field[child_index + 1] == EMPTY;
  if (!can_moveright)
    return false;

  bool parent_can_moveright =
      ((kumipuyo.YHasPointFive() &&
        field[parent_index - Field::COLUMN] == EMPTY) ||
       field[parent_index - 2 * Field::COLUMN] == EMPTY) &&
      field[parent_index - Field::COLUMN + 1] == EMPTY;
  bool child_can_moveright =
      ((kumipuyo.YHasPointFive() &&
        field[child_index - Field::COLUMN] == EMPTY) ||
       field[child_index - 2 * Field::COLUMN] == EMPTY) &&
      field[child_index - Field::COLUMN + 1] == EMPTY;
  if (kumipuyo.now_rotate == ROTATE_0) {
    return parent_can_moveright;
  } else if (kumipuyo.now_rotate == ROTATE_180) {
    return child_can_moveright;
  }
  return parent_can_moveright && child_can_moveright;
}

bool PadSearch::CanLeftDown(const Kumipuyo &kumipuyo, const Field &field) {
  int parent_index, child_index;
  std::tie(parent_index, child_index) = kumipuyo.GetPairIndex();

  bool can_moveright =
      field[parent_index - 1] == EMPTY && field[child_index - 1] == EMPTY;
  if (!can_moveright)
    return false;

  bool parent_can_moveright =
      ((kumipuyo.YHasPointFive() &&
        field[parent_index - Field::COLUMN] == EMPTY) ||
       field[parent_index - 2 * Field::COLUMN] == EMPTY) &&
      field[parent_index - Field::COLUMN - 1] == EMPTY;
  bool child_can_moveright =
      ((kumipuyo.YHasPointFive() &&
        field[child_index - Field::COLUMN] == EMPTY) ||
       field[child_index - 2 * Field::COLUMN] == EMPTY) &&
      field[child_index - Field::COLUMN - 1] == EMPTY;
  if (kumipuyo.now_rotate == ROTATE_0) {
    return parent_can_moveright;
  } else if (kumipuyo.now_rotate == ROTATE_180) {
    return child_can_moveright;
  }
  return parent_can_moveright && child_can_moveright;
}


// srcからdistに最も近くなる回転を返す。
// 一致時はNEUTRALを返す。
Command PadSearch::FasterRotateCommand(RotateType src, RotateType dist) {
	if (src < dist) {
		if (dist - src < ROTATE_NUMBER + src - dist) {
			return Command::ROTATE_RIGHT;
		}
		else {
			return Command::ROTATE_LEFT;
		}
	}
	else if (src == dist) {
		return Command::NEUTRAL;
	}
	else {
		if (src - dist < ROTATE_NUMBER + dist - src) {
			return Command::ROTATE_LEFT;
		}
		else {
			return Command::ROTATE_RIGHT;
		}
	}
}
