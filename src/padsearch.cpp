#include "padsearch.h"


// @arg field, pad�͒萔
// @arg kumipuyo, que�͕ύX���꓾��B
// @arg kumipuyo��UNDO����邽�ߕs��
// @arg next_node�̓R�s�[���ꂽ���̂��g���B
// PushLeft�ɂ��Ă����l
void PadSearch::PushMove(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode>* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down) {

	// �O�t���[���ō��E�ړ���]���s��Ȃ���
	if (pre_pad.IsRight() || pre_pad.IsLeft()) return;

	bool is_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		Node next_dup_node(next_node);

		// �ړ���Pad�Z�b�g
		if (is_left[dt]) next_dup_node.SetLeft(true);
		else next_dup_node.SetRight(true);
		if (down) next_dup_node.SetDown(true);

		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// �ړ���������
		if (is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();
		if (down) simulation_kumipuyo.Down();

		// �ړ���̍��W�������łȂ��Ȃ�continue
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		// ���E�̈ړ���߂�
		if (!is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		next_dup_node.SetDistEstimated(next_dist, simulation_kumipuyo);

		pre_node->emplace(next_dup_node, now_node);
		que->push( next_dup_node);

	}
}


// @pre next_node��pad������������Ă���
// @arg field, pad��const�Q�ƁB
// @arg que�͕ύX���꓾��B
// @arg kumipuyo, next_node�̓R�s�[���ꂽ���̂��g���B
// @arg next_dist�͍ŒZ�����B
// PushRotateLeft�ɂ��Ă����l

void PadSearch::PushRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down) {



	bool is_rot_left[2] = { true, false };

	for (int dt = 0; dt < 2; dt++) {

		// �O�t���[���ŊY�������]���s���Ă��Ȃ��Ȃ�
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
		if (down) simulation_kumipuyo.Down();



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
		next_dup_node.SetDistEstimated(next_dist, simulation_kumipuyo);

		pre_node->emplace(next_dup_node, now_node);
		que->push(next_dup_node);

	}
}

// �ړ������������s���Ȃ�A��]�̂ݍs���B��]���s���Ȃ�A�ǏR��or�����オ��B������s���ɂȂ�Ȃ�A�����s��Ȃ�
// �ړ����āA��]���āA�����Ȃ�ʏ�ʂ��]�����ƈړ��\��iPAD)�̂�
// �ړ����āA��]���āA�s���Ȃ�A��s�ǏR��B��s�ǏR��̌��ʂ��s���Ȃ�ړ��̂ݍs���B��s�����オ��͕s���ɂ͕킢

void PadSearch::PushMoveAndRotate(const Field& field_, const Pad& pre_pad, const Node& now_node,
	std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode >* pre_node,
	Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down) {

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

		if (down) next_dup_node.SetDown(true);

		Kumipuyo simulation_kumipuyo(now_kumipuyo);
		simulation_kumipuyo.SetX(next_dup_node.x);
		simulation_kumipuyo.SetY(next_dup_node.y);
		simulation_kumipuyo.SetRotate(next_dup_node.rotate);

		// �ړ���������
		if(is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();
		if (down) simulation_kumipuyo.Down();

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
		}

		// �ړ���߂�
		if (!is_left[dt]) simulation_kumipuyo.MoveLeft();
		else simulation_kumipuyo.MoveRight();

		// �����Ő����łȂ��Ȃ�f�X�c�C���Ȃ̂ŉ�]�s��
		if (!Simulator::ValidPosition(simulation_kumipuyo, field_)) {
			continue;
		}

		next_dup_node.SetXYROTATE(simulation_kumipuyo);
		next_dup_node.SetDistEstimated(next_dist, simulation_kumipuyo);

		pre_node->emplace(next_dup_node, now_node);
		que->push(next_dup_node);

	}
}

// �����������ɁA�ړI��PutIndex�܂ňړ����鑀����Ԃ�
// ������Ȃ���΁A����0��vector��Ԃ�
std::vector<Pad> PadSearch::CarefulOrder(const Kumipuyo &kumipuyo_,
	const Field &field_, const Pad &previous_pad_) {
	return std::vector<Pad>();
}

std::vector<Pad> PadSearch::DropOrder(const Kumipuyo &kumipuyo_,
	const Field &field_, const Pad &previous_pad_) {

	// �m�[�h�ǉ��̂��߂̃L���[
	std::priority_queue<Node, std::vector<Node>, CompareNode> que;
	// �e�m�[�h�܂ł̍ŒZ�o�H
	std::map < Node, int, DistinctNode > dist;
	// �o�H����
	std::map<Node, Node, DistinctNode > pre_nodes;
	Node start_n(kumipuyo_.X(), kumipuyo_.now_rotate, kumipuyo_.Y());
	start_n.SetPad(previous_pad_);
	start_n.SetDistEstimated(0, kumipuyo_);
	que.push(start_n);

	while (!que.empty()) {
		Node node;
		node = que.top(), que.pop();

		// USED CHECK
//		int ct = dist.count(node);
		auto it = dist.find(node);
		if (it != dist.end()) { continue; }

		// Update
		dist.insert({ node, node.dist });

		Node next_node(node);
		next_node.pre_pad = 0;
		next_node.dist++;

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

		PushMove(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, next_node.dist, true);
		if (node.rotate != kumipuyo_.desirable_put.rotate) {
			PushRotate(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, next_node.dist, true);
			PushMoveAndRotate(field_, Pad(node.pre_pad), node, &que, &pre_nodes, kumipuyo_, next_node, next_node.dist, true);
		}

			// �������Ȃ�
		if (node.pre_pad != 0) {
			pre_nodes.emplace(next_node, node);
			que.push(next_node);
		}

	}
	return std::vector<Pad>();
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
