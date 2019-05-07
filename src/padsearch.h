#ifndef BPUYOAI_PADSEARCH_H_
#define BPUYOAI_PADSEARCH_H_

#include <vector>
#include <queue>
#include <map>
#include <unordered_map>

#include "command.h"
#include "field.h"
#include "kumipuyo.h"
#include "types.h"
#include "simulator.h"
#include "pad.h"

class Node {
public:
	Node() : pre_pad(0) {}
	Node(float x, RotateType rotate, float y) : x(x), rotate(rotate), y(y), pre_pad(0) {}
	Node(float x, RotateType rotate, float y, int pre) : x(x), rotate(rotate), y(y), pre_pad(pre) {}
	float x;
	RotateType rotate;
	float y;
	unsigned int pre_pad; // 6bit
												// 000000
												// ||||||
												// U|||||
												//  R||||
	                      //   D|||
	                      //    L||
												//     R|
												//     L|
												//      R
												//      R
	                      // リトルエンディアンで0からUP, ...(Commandのenumと一致）

	void SetXYROTATE(const Kumipuyo& kumipuyo) {
		x = kumipuyo.X();
		rotate = kumipuyo.now_rotate;
		y = kumipuyo.Y();
	}

	void SetPad(const Pad& pad) {
		pre_pad = 0;
		for (int i = 0; i < Pad::PAD_SIZE; i++)
			if(pad.Is(i))
			pre_pad |= 1U << i;
	}

	Pad GetPad() const {
		return Pad(pre_pad);
	}

	bool Equal(Node n) {
		return x == n.x && rotate == n.rotate && y == n.y && pre_pad == n.pre_pad;
	}

	bool operator < (const Node& a) const {
		if (x < a.x) return true;
		if (x > a.x) return false;
		if (y < a.y) return true;
		if (y > a.y) return false;
		if (rotate < a.rotate) return true;
		if (rotate > a.rotate) return false;
		return pre_pad < a.pre_pad;
	}

	inline bool IsUp()   const { return (1U << Command::UP) & pre_pad; }
	inline bool IsLeft() const { return (1U << Command::LEFT) & pre_pad; }
	inline bool IsDown() const { return (1U << Command::DOWN) & pre_pad; }
	inline bool IsRight() const { return (1U << Command::RIGHT) & pre_pad; }
	inline bool IsRotateRight() const { return (1U << Command::ROTATE_RIGHT) & pre_pad; }
	inline bool IsRotateLeft() const { return (1U << Command::ROTATE_LEFT) & pre_pad; }

	void SetUp(bool flag) { if (flag) pre_pad |= (1U << Command::UP); else pre_pad &= ~(1U << Command::UP); }
	void SetLeft(bool flag) { if (flag) pre_pad |= (1U << Command::LEFT); else pre_pad &= ~(1U << Command::LEFT); }
	void SetDown(bool flag) { if (flag) pre_pad |= (1U << Command::DOWN); else pre_pad &= ~(1U << Command::DOWN); }
	void SetRight(bool flag) { if (flag) pre_pad |= (1U << Command::RIGHT); else pre_pad &= ~(1U << Command::RIGHT); }
	void SetRotateRight(bool flag) { if (flag) pre_pad |= (1U << Command::ROTATE_RIGHT); else pre_pad &= ~(1U << Command::ROTATE_RIGHT); }
	void SetRotateLeft(bool flag) { if (flag) pre_pad |= (1U << Command::ROTATE_LEFT); else pre_pad &= ~(1U << Command::ROTATE_LEFT); }

};

class PadSearch {
public:
  static std::vector<Pad> DropOrder(const Kumipuyo &kumipuyo,
                                        const Field &field,
		                                    const Pad& previous_pad);
	static std::vector<Pad> CarefulOrder(const Kumipuyo &kumipuyo,
		const Field &field,
		const Pad& previous_pad);

private:

	PadSearch();

	static bool CanRightDown(const Kumipuyo &kumipuyo, const Field &field);
	static bool CanLeftDown(const Kumipuyo &kumipuyo, const Field &field);
	static Command FasterRotateCommand(RotateType src, RotateType dist);

	static void PushMove(const Field & field_, const Pad & pre_pad, const Node & now_node, std::queue<std::pair<int, Node>>* que, std::map<Node, Node>* pre_node, Kumipuyo now_kumipuyo, Node next_node, const int next_dist);

	static void PushRotate(const Field & field_, const Pad & pre_pad, const Node & now_node, std::queue<std::pair<int, Node>>* que, std::map<Node, Node>* pre_node, Kumipuyo now_kumipuyo, Node next_node, const int next_dist);

	static void PushMoveAndRotate(const Field & field_, const Pad & pre_pad, const Node & now_node, std::queue<std::pair<int, Node>>* que, std::map<Node, Node>* pre_node, Kumipuyo now_kumipuyo, Node next_node, const int next_dist);

};
#endif
