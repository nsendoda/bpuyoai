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
		// 0.5段落ちるのにかかる落下フレームを計算するために2で割る
		// 16 -> 8
		// 12 -> 6
		// 8 -> 4
		// 7 -> 3
		_drop_frame = drop_speed_ / 2;
		// 落下速度が8より大きいなら
		const Frame NORMAL_DELAY_PUT = 43;
		// 落下速度が8以下なら
		const Frame FAST_DELAY_PUT = 39;
		// 落下速度が8以下だと、回転を入れるタイミングを遅らせた方が延命できる
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

	// 右回しをするか、左まわしをするかを決める。
	// また、カルメル式回しができるかどうかを判断する。
	MawashiStateType Decide();

	// 回す位置まで移動
	MawashiStateType Move();

	// 遅発回しの最初の遅延部分と一回目のぷよの浮かし
	MawashiStateType KarumeruMawashiWait();
	MawashiStateType MawashiWait();

	// 回す
	MawashiStateType KarumeruMawashi();
	MawashiStateType Mawashi();

	// 最終的に置く場所を決める
	MawashiStateType DecidePut();
	MawashiStateType DecidePutQuickly();

	// 目的の場所へ置く
	MawashiStateType Put();

	// 急いで目的の場所へ置く（下を押す）
	MawashiStateType PutQuickly();

	// 負けを認める
	MawashiStateType GiveUp();
};



#endif