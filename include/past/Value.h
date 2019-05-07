#pragma once

#include <algorithm>
#include <vector>
#include <queue>
#include <tuple>

#include "Simulator.h"
#include "Chain.h"
#include "Assert.h"

class Value
{
public:
	Chain one_puyo_max_score_chain;
	Chain kumipuyo_max_score_chain;
	Chain two_puyo_max_score_chain;

	int connection_tochain_num;
	Chain max_chain;
	Chain sum_chain;
	Value();
	~Value();

	void CaluculatePotential(const Field& field, int put_frame, int firstput_column = 0);
	void ConnectionPotential(const Field& field, int put_frame);
	void ConnectionAddedPotential(const Field& field, int put_frame);

	bool GreaterThanByConnectionValueWay(const Value& value)const;
	inline static Value& GetGreater(Value& a, Value& b) {
		return (std::max)({ a.one_puyo_max_score_chain ,a.kumipuyo_max_score_chain, a.two_puyo_max_score_chain }) <
			(std::max)({b.one_puyo_max_score_chain ,b.kumipuyo_max_score_chain, b.two_puyo_max_score_chain })
			? b : a;
	};
	inline bool operator<(const Value& a)const {
//		Chain d = kumipuyo_max_score_chain;
//			Chain e = kumipuyo_max_score_chain;
//		return d < e;
		return (std::max)({ one_puyo_max_score_chain ,kumipuyo_max_score_chain, two_puyo_max_score_chain }) <
			(std::max)({ a.one_puyo_max_score_chain, a.kumipuyo_max_score_chain, a.two_puyo_max_score_chain });
	}

	inline bool operator>(const Value& a)const { return a < *this;}
private:
	void SetChainIfMaxScore(const Chain& chain, int puyo_num, bool kumipuyo);
//	std::vector<std::vector<int> > CaluculateConnectionsAndEmptyRoomsCount(const Field& field)const;
	std::vector<std::vector<int> > CaluculateConnections(const Field& field)const;
	void DeleteFieldConnection(Field* field, const std::vector<int>& deletes)const;
	int AddOnePuyoToConnections(std::vector<std::vector<int> >* connections, const Field& field, int put_index)const;


};

