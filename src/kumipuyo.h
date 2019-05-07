#ifndef BPUYOAI_KUMIPUYO_AI_
#define BPUYOAI_KUMIPUYO_AI_

#include <cmath>
#include <algorithm>

#include "field.h"
#include "puttype.h"
#include "rotatetype.h"
#include "types.h"

/// コンストラクタがプレイヤー初期化を伴うことに注意
class Kumipuyo {
public:
	Color parent, child;
	RotateType now_rotate;
	PutType desirable_put;
	Kumipuyo();
	Kumipuyo(Color parent, Color child);
	Kumipuyo(Color parent, Color child, RotateType now_rotate, float x, float y);
	Kumipuyo(float x, RotateType now_rotate, float y);
	~Kumipuyo();

	static constexpr float X_MAX = 5.0;
	static constexpr float X_MIN = 0.0;
	static constexpr float Y_MAX = 12.0; // 13 にはならない？
	static constexpr float Y_MIN = 0.0;

	inline float X() const { return x; }

	inline float Y() const { return y; }

	void SetX(float a) { x = a; }

	void SetRotate(RotateType r) { now_rotate = r;  }

	void SetY(float a) { y = a; }

	// Bぷよで右を入力するとxは1.0増える
	void MoveRight() { x = std::max(X_MIN, x + 1.0f); }

	// Bぷよで左を入力するとxは1.0減る
	void MoveLeft() { x = std::min(X_MAX, x - 1.0f); }

	// Bぷよ準拠で1フレームDownを入力する。
	// つまり、yを0.5減算する。
	void Down() { y = std::max(Y_MIN, y - 0.5f); }

	// DownのUndo用途
	void DownUndo() { y = std::min(Y_MAX, y + 0.5f); }


	// Bぷよでは回転によって浮き上がった時にy座標は増加する。
	// その時、yは1.0増加する。
	void Up() { y = std::min(Y_MAX, y + 1.0f); }


	void AddY(float a) { y = std::min(Y_MAX, std::max(Y_MIN, y + a)); }

	inline int GetColumn() const {
		return static_cast<int>(x) + 1;
	}

	// xとyの情報から座標を返す
	inline int GetIndex() const {
		return static_cast<int>(x) + 1 + Field::COLUMN * (static_cast<int>(std::floor(y)) + 1);
	}

	// @ret parent_index, child_index
	inline std::pair<int, int> GetPairIndex() const {
 		int parent_index = GetIndex();
		int child_index = parent_index;
		switch (now_rotate) {
		case RotateType::ROTATE_0: {
			child_index += Field::COLUMN;
			break;
		}
		case RotateType::ROTATE_90: {
			child_index += 1;
			break;
		}
		case RotateType::ROTATE_180: {
			child_index -= Field::COLUMN;
			break;
		}
		case RotateType::ROTATE_270: {
			child_index -= 1;
			break;
		}
		}
		return { parent_index, child_index };
	}

	inline void RotateLeft() {
		switch (now_rotate) {
		case RotateType::ROTATE_0: {
			now_rotate = RotateType::ROTATE_270;
			break;
		}
		case RotateType::ROTATE_90: {
			now_rotate = RotateType::ROTATE_0;
			break;
		}
		case RotateType::ROTATE_180: {
			now_rotate = RotateType::ROTATE_90;
			break;
		}
		case RotateType::ROTATE_270: {
			now_rotate = RotateType::ROTATE_180; 
				break;
		}

		}
	}

	inline void RotateRight() {
		switch (now_rotate) {
		case RotateType::ROTATE_0: {
			now_rotate = RotateType::ROTATE_90;
			break;
		}
		case RotateType::ROTATE_90: {
			now_rotate = RotateType::ROTATE_180;
			break;
		}
		case RotateType::ROTATE_180: {
			now_rotate = RotateType::ROTATE_270;
			break;
		}
		case RotateType::ROTATE_270: {
			now_rotate = RotateType::ROTATE_0;
			break;
		}

		}
	}

	inline bool YHasPointFive() const {
		return std::round(y) != std::floor(y);
	}

	inline bool IsDesirablePut() const {
		return (static_cast<int>(x) + 1) == desirable_put.column && now_rotate == desirable_put.rotate;
	}

	inline bool OnLeftIndex(int parent, int child) {
		int now_parent, now_child;
		std::tie(now_parent, now_child) = GetPairIndex();
		return now_parent == parent - 1 && now_child == child - 1;
	}

	
	inline bool AboveIndex(int parent, int child) {
		int now_parent, now_child;
		std::tie(now_parent, now_child) = GetPairIndex();
		return now_parent == parent + Field::COLUMN && now_child == child + Field::COLUMN;
	}
	inline bool OnUpperLeftIndex(int parent, int child) {
		int now_parent, now_child;
		std::tie(now_parent, now_child) = GetPairIndex();
		return now_parent == parent + Field::COLUMN - 1 && now_child == child + Field::COLUMN - 1;
	}

	inline bool EqualsIndex(int parent, int child) {
		int now_parent, now_child;
		std::tie(now_parent, now_child) = GetPairIndex();
		return now_parent == parent && now_child == child;
	}

private:
	// x=[0, 5], y=[0, 11.5]
  // この組みぷよの位置xとy
	float x, y; 
};

#endif