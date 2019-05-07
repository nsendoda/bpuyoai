#pragma once


#include <utility>

#include "Parameter.hpp"
#include "RotateType.h"
#include "Field.h"
//
/// ぷよの置き方を表すクラス.
class PutType
{
public:
	int index;
	RotateType rotate;
	PutType();
	PutType(int index, RotateType rotate);
	~PutType();
	void SetPattern(int index);
	static PutType GetPattern(int index);
/// STLをやめれば解決する可能性？でもそれはfuckだよね
	std::pair<int, int> CaluculatePutIndex(const Field& field)const;
	bool CanPut(const Field& field)const;
private:
};
