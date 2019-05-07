#pragma once

#include "Parameter.hpp"

class Chain
{
public:
	int number, score, frame;

	Chain();
	Chain(int number, int score, int frame);
	~Chain();
	static const int FRAME_PARAMETER = 1;
	static const int NUMBER_PARAMETER = -50;

	bool GreaterKillerThan(const Chain& comp)const;

	inline bool operator<(const Chain& a) const{
		return score - Chain::FRAME_PARAMETER * frame + Chain::NUMBER_PARAMETER * number * number
			< a.score - Chain::FRAME_PARAMETER * a.frame + Chain::NUMBER_PARAMETER * a.number * a.number;
	}
	inline bool operator>(const Chain& a) const{
		return a < *this;
	}
	inline Chain operator+=(const Chain& a){
		this->frame += a.frame;
		this->number += a.number;
		this->score += a.score;
		return *this;
	}
};


