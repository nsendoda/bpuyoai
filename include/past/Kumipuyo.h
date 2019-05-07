#pragma once

#include <cmath>
#include "Parameter.hpp"
#include "PutType.h"
#include "Field.h"
#include "puyo_ai.h"

/// コンストラクタがプレイヤー初期化を伴うことに注意
class Kumipuyo
{
public:
	Color parent, child;
	float x, y;
	RotateType rotate;
	PutType desirable_put;
	Kumipuyo(int player);
	~Kumipuyo();

	void SetPositionAndRotate();
	void SetNowColor();
	void SetNextColor();
	///  ぷよを引数のfieldに置き、置くのにかかったフレームを返す
	int PutAndCaluculateFrame(Field* field, const PutType& put)const;
	inline int GetIndex()const {
		return static_cast<int>(x + 1 + Field::COLUMN * (std::round(y) + 1));
	}
private:
	int player_;
	Kumipuyo();
	void FloatInField(Field* field)const;
	int FallKumipuyo(Field* field_, int parentFileIndex, int rotate)const;
};

