#include "kumipuyo.h"

Kumipuyo::Kumipuyo() {}

Kumipuyo::Kumipuyo(Color parent, Color child) : parent(parent), child(child) {}

Kumipuyo::Kumipuyo(Color parent, Color child, RotateType now_rotate, float x, float y)
	: parent(parent), child(child), now_rotate(now_rotate), x(x), y(y) {}

Kumipuyo::Kumipuyo(float x, RotateType now_rotate, float y) : x(x), now_rotate(now_rotate), y(y) {}


Kumipuyo::~Kumipuyo() {}
