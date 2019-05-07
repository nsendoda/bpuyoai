#ifndef BPUYOAI_BITDATABASE_AI_
#define BPUYOAI_BITDATABASE_AI_

#include "../state.h"
#include "../types.h"
#include "bitdata.h"

#include <map>

class BitDatabase {
public:

	static constexpr int VARIABLE_PUYO_SIZE = 21;
	static constexpr int PUYO_BIT = 3;

	static const PutIndex REVERSE_PUTS[PUTTYPE_PATTERN];


	BitDatabase() {}



	inline void Insert(__int64 f1_, __int64 f2_, __int64 f3_, __int64 f4_, __int64 now_, __int64 next_, const std::vector<PutIndex>& index_ct) {
		PutIndex best_index = std::distance(index_ct.begin(), std::max_element(index_ct.begin(), index_ct.end()));
		puyofu_map.emplace(BitData(f1_, f2_, f3_, f4_, now_, next_), best_index);
		onlynow_puyofu_map.emplace(BitData(f1_, f2_, f3_, f4_, now_, next_), best_index);
	}

	inline void Insert(__int64 f1_, __int64 f2_, __int64 f3_, __int64 f4_, __int64 now_, __int64 next_, FieldIndex index_) {
		puyofu_map.emplace(BitData(f1_, f2_, f3_, f4_, now_, next_), index_);
		onlynow_puyofu_map.emplace(BitData(f1_, f2_, f3_, f4_, now_, next_), index_);
	}

	// ���v����f�[�^�����鎞�A���̃f�[�^�ōł��u���ꂽPutIndex��Ԃ�
	// ���v����f�[�^��������΁A-1��Ԃ�
	inline PutIndex BestIndex(const State& state_) const {
		std::pair<BitData, bool> bits = Bit(state_.field, state_.now_kumipuyo, state_.next_kumipuyo);
		auto itr = puyofu_map.find(bits.first);
		if (itr != puyofu_map.end()) {
			if (bits.second) return REVERSE_PUTS[itr->second];
			return itr->second;
		}
		auto second_itr = onlynow_puyofu_map.find(bits.first);
		if (second_itr != onlynow_puyofu_map.end()) {
			if (bits.second) return REVERSE_PUTS[second_itr->second];
			return second_itr->second;
		}

		return -1;
	}

private:

	std::map<BitData, PutIndex, CompareBitData> puyofu_map;
	std::map<BitData, PutIndex, CompareBitDataOnlyNow> onlynow_puyofu_map;

	// ��ӂ���6�̕ϐ��ɑg�Ղ��ƃt�B�[���h��ϊ�����
	// �ԍ���t���鏇�Ԃ́AField��FieldStart�`FieldEnd�A���ݎ�A�l�N�X�g�ł���B
	// �܂��A���ݎ�A�l�N�X�g�͐e���Ⴍ�Ȃ�悤�ɁA����ւ���B
	// ���ݎ�����ւ����ꍇ�A2�Ԗڂ̋A��l��true�ɂȂ�
	// ���̕ϊ������́A�f�[�^�x�[�X�ɓ������toml�t�@�C���Ɠ���ł���B
	std::pair<BitData, bool> Bit(const Field& field_, const Kumipuyo& now_, const Kumipuyo& next_) const;

};


#endif