#pragma once

#include <tuple>
#include <algorithm>

#include "PutType.h"
#include "State.h"
#include "Simulator.h"
#include "Value.h"

class Nomi
{
public:
	Nomi();
	~Nomi();
	PutType Decide(const State& state);
	PutType Normal(const State& state);
	void Dig();
	void Mawashi();
	void Cope();
};

