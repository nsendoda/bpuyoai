#ifndef BPUYOAI_MAWASHISTATE_H_
#define BPUYOAI_MAWASHISTATE_H_

#include "bpuyopad.h"
#include "field.h"
#include "kumipuyo.h"
#include "state.h"
#include "types.h"
#include "simulator.h"
#include "ojamasimulator.h"
#include "mawashiputscore.h"
#include "mawashiputsimulator.h"

#include <functional>
#include <vector>
#include <algorithm>

class MawashiState {
public:

	MawashiState(Frame drop_speed_) {
		Init(drop_speed_);
	};

	inline void Init(Frame drop_speed_) {
		// 0.5�i������̂ɂ����闎���t���[�����v�Z���邽�߂�2�Ŋ���
		// 16 -> 8
		// 12 -> 6
		// 8 -> 4
		// 7 -> 3
		_drop_frame = drop_speed_ / 2;
		// �������x��8���傫���Ȃ�
		const Frame NORMAL_DELAY_PUT = 43;
		// �������x��8�ȉ��Ȃ�
		const Frame FAST_DELAY_PUT = 39;
		// �������x��8�ȉ����ƁA��]������^�C�~���O��x�点�����������ł���
		const Frame _FAST_DELAY_REROTATE = 3;
		_now_put_delay_frame = _drop_frame > 4 ? NORMAL_DELAY_PUT : FAST_DELAY_PUT;
		_now_delay_rerotate = _drop_frame > 4 ? 0 : _FAST_DELAY_REROTATE;
		_karumeru_delay_frame = 0;
		_mawashi_delay_frame = 0;
		_mawashi_count = 0;

		_max_height = 0;
		_best_column = 0;
		_can_karumeru_mawashi = false;
		_start_right_side = false;
		_wait_y = 0;

		_mawashistate = Decide();
	}

	inline void Excute(const State& state_, BpuyoPad* bpuyopad_){
		_mawashistate(state_, bpuyopad_);
	}

	inline void SetGiveUp() {
		_mawashistate = GiveUp();
	}

	inline void SetQuicklyDrop() {
		_mawashistate = DecidePutQuickly();
	}

private:


	using MawashiStateType = std::function<void(const State& state, BpuyoPad* bpuyopad_)>;

	Frame _now_put_delay_frame;
	Frame _now_delay_rerotate;

	int _max_height;
	Column _best_column;
	bool _can_karumeru_mawashi;
	bool _start_right_side;
	float _wait_y;


	Frame _drop_frame;
	Frame _karumeru_delay_frame;
	Frame _mawashi_delay_frame;

	int _mawashi_count;

	PutType _best_put;

	MawashiStateType _mawashistate;

	// �E�񂵂����邩�A���܂킵�����邩�����߂�B
	// �܂��A�J���������񂵂��ł��邩�ǂ����𔻒f����B
	MawashiStateType Decide();

	// �񂷈ʒu�܂ňړ�
	MawashiStateType Move();

	// �x���񂵂̍ŏ��̒x�������ƈ��ڂ̂Ղ�̕�����
	MawashiStateType KarumeruMawashiWait();
	MawashiStateType MawashiWait();

	// ��
	MawashiStateType KarumeruMawashi();
	MawashiStateType Mawashi();

	// �ŏI�I�ɒu���ꏊ�����߂�
	MawashiStateType DecidePut();
	MawashiStateType DecidePutQuickly();

	// �ړI�̏ꏊ�֒u��
	MawashiStateType Put();

	// �}���ŖړI�̏ꏊ�֒u���i���������j
	MawashiStateType PutQuickly();

	// ������F�߂�
	MawashiStateType GiveUp();
};



#endif