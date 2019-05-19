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
	int dist;
	int estimated;
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
	                      // ÉäÉgÉãÉGÉìÉfÉBÉAÉìÇ≈0Ç©ÇÁUP, ...(CommandÇÃenumÇ∆àÍívÅj

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

	//ç∂âEì¸óÕÇÕîΩâfÇ™íxÇ¢ÇÃÇ≈ëÅÇﬂÇ…îΩâf
	void SetDistEstimated(int dist_, const Kumipuyo& kumipuyo_) {
		dist = dist_;
		estimated = dist + std::abs(kumipuyo_.desirable_put.column - 1 - (static_cast<int>(kumipuyo_.X())
			- IsLeft() + IsRight()))
			+ std::abs(kumipuyo_.desirable_put.rotate - kumipuyo_.now_rotate);
	}

	Pad GetPad() const {
		return Pad(pre_pad);
	}

	bool Equal(Node n) {
		return x == n.x && rotate == n.rotate && y == n.y && pre_pad == n.pre_pad;
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

class DistinctNode {
public:
	bool operator()(const Node &a, const Node &b) const {
		if (a.x != b.x) return a.x < b.x;
		if (a.rotate != b.rotate) return a.rotate < b.rotate;
		if (a.y != b.y) return a.y < b.y;
		return a.pre_pad < b.pre_pad;
	}
};

class CompareNode {
public:
	bool operator()(const Node &a, const Node &b) const {
		return a.estimated > b.estimated;
	}
};

class PadSearch {
public:
	static std::vector<Pad> DropOrder(const Kumipuyo &kumipuyo,
		const Field &field,
		const Pad& previous_pad, bool always_down);

	static std::queue<Pad> CancelDrop(const Kumipuyo &kumipuyo,
		const Field &field,
		const Pad& previous_pad);

private:

	PadSearch();

	static void PushMove(const Field & field_, const Pad & pre_pad, const Node & now_node,
		std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode>* pre_node, Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down);

	static void PushRotate(const Field & field_, const Pad & pre_pad, const Node & now_node,
		std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode>* pre_node, Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down);

	static void PushMoveAndRotate(const Field & field_, const Pad & pre_pad, const Node & now_node,
		std::priority_queue<Node, std::vector<Node>, CompareNode>* que, std::map<Node, Node, DistinctNode>* pre_node, Kumipuyo now_kumipuyo, Node next_node, const int next_dist, bool down);

};
#endif
