#include "padsearch.h"


// @arg field, pad�͒萔
// @arg kumipuyo, que�͕ύX���꓾��B
// @arg kumipuyo��UNDO����邽�ߕs��
// @arg next_node�̓R�s�[���ꂽ���̂��g���B
// PushLeft�ɂ��Ă����l
void PadSearch::PushMove(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::queue<std::pair<int, Node> >* que, std::map<Node, Node >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist) {

	// �O�t���[���ō��E�ړ���]���s��Ȃ���
	if (pre_pad.IsRight() || pre_pad.IsLeft()) return;

	bool is_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		Node next_dup_node(next_node);

		// �ړ���Pad�Z�b�g
		if (is_left[dt]) next_dup_node.SetLeft(true);
		else next_dup_node.SetRight(true);

		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// �ړ���������
		if (is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		// �ړ���̍��W�������łȂ��Ȃ�continue
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		// �ړ���߂�
		if (!is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		pre_node->emplace(next_dup_node, now_node);
		que->push({ next_dist, next_dup_node });

	}
}


// @pre next_node��pad������������Ă���
// @arg field, pad��const�Q�ƁB
// @arg que�͕ύX���꓾��B
// @arg kumipuyo, next_node�̓R�s�[���ꂽ���̂��g���B
// @arg next_dist�͍ŒZ�����B
// PushRotateLeft�ɂ��Ă����l

void PadSearch::PushRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::queue<std::pair<int, Node> >* que, std::map<Node, Node >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist) {



	bool is_rot_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		// �O�t���[���ŊY�������]���s���Ă��Ȃ��Ȃ�
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

		// 13�i�ڂ̎��A��]��90or270�ŖړI�̂��܂ŉ����Ȃ��]�����Ȃ�
		if (simulation_kumipuyo.Y() == 12.0 &&
			std::abs(simulation_kumipuyo.X() - (now_kumipuyo.desirable_put.column - 1)) > 1 &&
			(simulation_kumipuyo.now_rotate == RotateType::ROTATE_90 ||
				simulation_kumipuyo.now_rotate == RotateType::ROTATE_270)) {
			return;
		}

		// ��]������
		if (is_rot_left[dt]) simulation_kumipuyo.RotateLeft();
		else simulation_kumipuyo.RotateRight();



		// ��]��̍��W�������łȂ��Ȃ�ǂ��R��
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

			// �����Ő����łȂ��Ȃ�f�X�c�C���Ȃ̂ŉ�]�s��
			if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
				continue;
			}
		}

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		pre_node->emplace(next_dup_node, now_node);
		que->push({ next_dist, next_dup_node });

	}
}

// �ړ������������s���Ȃ�A��]�̂ݍs���B��]���s���Ȃ�A�ǏR��or�����オ��B������s���ɂȂ�Ȃ�A�����s��Ȃ�
// �ړ����āA��]���āA�����Ȃ�ʏ�ʂ��]�����ƈړ��\��iPAD)�̂�
// �ړ����āA��]���āA�s���Ȃ�A��s�ǏR��B��s�ǏR��̌��ʂ��s���Ȃ�ړ��̂ݍs���B��s�����オ��͕s���ɂ͕킢

void PadSearch::PushMoveAndRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::queue<std::pair<int, Node> >* que, std::map<Node, Node >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist) {

	// �O�t���[���ō��E�ړ���]���s��Ȃ���
	if (pre_pad.IsRight() || pre_pad.IsLeft()) return;

	// 13daname�ŉ�]����v����Ȃ炱�̊֐����ĂԕK�v�͂Ȃ�
	if (next_node.y == 12.0 && next_node.rotate == now_kumipuyo.desirable_put.rotate) return;

	bool is_left[4] = { true, false, true, false };
	bool is_rot_left[4] = { true, true, false, false };

	for (int dt = 0; dt < 4; dt++) {

		if (is_rot_left[dt] && pre_pad.IsRotateLeft()) continue;
		else if (!is_rot_left[dt] && pre_pad.IsRotateRight()) continue;

		Node next_dup_node(next_node);


		// ��]�ƈړ���Pad�Z�b�g
		if(is_left[dt]) next_dup_node.SetLeft(true);
		else next_dup_node.SetRight(true);

		if(is_rot_left[dt]) next_dup_node.SetRotateLeft(true);
		else next_dup_node.SetRotateRight(true);


		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// �ړ���������
		if(is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		// ���̎��_�Ő����łȂ��Ȃ烊�Z�b�g
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		// ��]��������
		if (is_rot_left[dt]) simulation_kumipuyo.RotateLeft();
		else simulation_kumipuyo.RotateRight();



		// ��]��̍��W�������łȂ��Ȃ�ǂ��R��
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

			// �ړ���߂�
			if ( ! is_left[dt]) simulation_kumipuyo.MoveLeft();
			else simulation_kumipuyo.MoveRight();

			// �����Ő����łȂ��Ȃ�f�X�c�C���Ȃ̂ŉ�]�s��
			if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
				continue;
			}
		}

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		pre_node->emplace(next_dup_node, now_node);
		que->push({ next_dist, next_dup_node });

	}
}

// �����������ɁA�ړI��PutIndex�܂ňړ����鑀����Ԃ�
// ������Ȃ���΁A����0��vector��Ԃ�
std::vector<Pad> PadSearch::CarefulOrder(const Kumipuyo &kumipuyo_,
	const Field &field_, const Pad &previous_pad_) {

	// �m�[�h�ǉ��̂��߂̃L���[
	std::queue<std::pair<int, Node> > que;
	// �e�m�[�h�܂ł̍ŒZ�o�H
	std::map < Node, int > dist;
	// �o�H����
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

		// �O��Pad�ňړ����͂�����Ă���Ȃ珈��
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

		// �������Ȃ�
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

		// 16����ȓ��Ɋ������Ȃ�or���݂̍ŒZ�菇��蒷���Ȃ�����I��
		if (pads.size() > DEFAULT_SIZE || best_orders.size() < pads.size()) {
			break;
		}
		 
		// �ŏI�I�ɒu���ꏊ��Index���擾
		int put_parent_index, put_child_index;
		std::tie(put_parent_index, put_child_index) =
			Simulator::CalculatePutIndex(now_kumipuyo.desirable_put, field_);


		// �O�t���[���ō��E�Ɉړ����Ă���Ȃ�ړ�����
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

		// �I��check
		// ��]�����ƈʒu����v���Ă���Ȃ�A�A������
		if (now_kumipuyo.IsDesirablePut()) {
			// �g�݂Ղ��Index���擾
			int parent_index, child_index;
			std::tie(parent_index, child_index) = now_kumipuyo.GetPairIndex();

			// downpad����
			Pad pad;
			pad.SetDown(true);
			while (true) {
				// �ݒu�����ꏊ����v����܂ŉ�
				// ������ɂ���ĕЕ���index������邱�Ƃ��l��
				if ((parent_index == put_parent_index || child_index == put_child_index)
					&& !now_kumipuyo.YHasPointFive()) {
					break;
				}
				pads.push_back(pad);
				now_kumipuyo.Down();
				std::tie(parent_index, child_index) = now_kumipuyo.GetPairIndex();
			}


			// ������ꍇ�͕Е�����v���Ă�΂悢
			if (best_orders.size() > pads.size()) {
				best_orders = pads;
			}
			continue;
		}

		// �ݒu�����ɓ���Ȃ�index�Ȃ痎��
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

		// �O�t���[���ō��E���͂������Ă��Ȃ��Ȃ���͂��l����
		if (!previous_right && !previous_left) {
			// �ǂ��Ȃ�
			if (now_kumipuyo.GetColumn() < now_kumipuyo.desirable_put.column) {
				// �E�ֈړ�������B
				now_kumipuyo.MoveRight();
				// �u�����������֍s����
				if (Simulator::ValidPosition(now_kumipuyo, field_)) {
					pad.SetRight(true);
				}
				// UNDO
				now_kumipuyo.MoveLeft();
			}
			// ����
			else if (now_kumipuyo.GetColumn() > now_kumipuyo.desirable_put.column) {
				now_kumipuyo.MoveLeft();
				if (Simulator::ValidPosition(now_kumipuyo, field_)) {
					pad.SetLeft(true);
				}
				// UNDO
				now_kumipuyo.MoveRight();
			}
		}

		// ��]��������v���ĂȂ����O�t���[���ŉ�]���s��Ȃ���
		// @note 2�t���[���ԂŉE��]������]���s������͂Ղ�𕂂������Ɏg�����A
		//       ���̊֐��ł͈���Ȃ�
		if (now_kumipuyo.now_rotate != now_kumipuyo.desirable_put.rotate
			&& !previous_pad.IsRotateRight() && !previous_pad.IsRotateLeft()) {
			// @note �K�؂ȉ�]�������v�Z�͂��Ȃ�
			//       �ǏR��̉e���ŋt��]�łȂ��Ƃ����Ȃ��ꍇ�����邽��
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
				// �����łȂ��Ȃ�ǏR��
				if (!Simulator::ValidPosition(now_kumipuyo, field_)) {
					kicked_wall = true;
					if (left_mode == 0) now_kumipuyo.MoveLeft();
					else now_kumipuyo.MoveRight();
					// �����Ő����łȂ��Ȃ�f�X�c�C���Ȃ̂ŉ�]�s��
					if (!Simulator::ValidPosition(now_kumipuyo, field_)) {
						can_rotate_right = false;
					}
				}
				// ��]�L�����Z��
				// B�Ղ�ł́A��]�ƈړ��𓯎��ɍs�������ʕǏR��̍��W�ɂ������ꍇ�A��s�ǏR�肪��������
				// ��s�ǏR��������ꍇ�A�ǏR�聨���E���́i����E�j�Ő���ȍ��W�֍s���B
				// ���̎��A�ǏR��Ł��Ɉ��]���Ɉړ����邪�A�������Ղ�Ŗ��܂��Ă�ƕǏR�肪���s���A
				// ��]������Ȃ��Ȃ�B
				// ���̏ꍇ�̓X�L�b�v����B
				else if ((pad.IsRight() && left_mode == 0 ) || (pad.IsLeft() && left_mode == 1) ) {
					if (left_mode == 0) now_kumipuyo.MoveRight();
					else now_kumipuyo.MoveLeft();
					// �ړ�������ŕǏR�肪��������ꍇ
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
					// �ǏR�莞�͌��UNDO
					else {
						if (left_mode == 0) now_kumipuyo.MoveLeft();
						else now_kumipuyo.MoveRight();
					}

				}

				// ��]push, ��]����push
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
			// ��]������v��, ���͉�]�҂�
			// ���EDown or Down
			down_push();
			// ���E���͂̂݁A����NEUTRAL��push
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


// src����dist�ɍł��߂��Ȃ��]��Ԃ��B
// ��v����NEUTRAL��Ԃ��B
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
