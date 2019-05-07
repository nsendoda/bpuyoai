#pragma once

#include "Kumipuyo.h"
#include "PutType.h"

/// Bぷよ側に操作を出力するクラス
class BpuyoPad
{
public:
	static void Drop(const Kumipuyo& kumipuyo);
	static void Move(const Kumipuyo& kumipuyo);
private:
	BpuyoPad();
};

