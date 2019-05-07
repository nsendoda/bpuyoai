#include "BpuyoPad.h"



BpuyoPad::BpuyoPad()
{
}


/*
 *引数のmoveinfoのfileIndexとrot通りにぷよを落とす。
 *ai_SetTumiと違う点はai_SetPadを用いて制御している
 */
void BpuyoPad::Drop(const Kumipuyo& kumipuyo){
	const PutType& desire = kumipuyo.desirable_put;
		static bool pressedrotate = false;
		static bool pressedmove = false;

		int up_operate = 0;
		int right_operate = 0;
		int down_operate = 0;
		int left_operate = 0;
		int a_rotate = 0;
		int b_rotate = 0;
    /* 回転が一致していない場合 */
		if (kumipuyo.rotate != desire.rotate) {
			if (!pressedrotate) {
				if (desire.rotate == RotateType::ROTATE_270)a_rotate = 1;
				else b_rotate = 1;
			}
			else {
				a_rotate = b_rotate = 0;
			}
			/*ボタンが押されているかどうかのフラグを反転*/
			pressedrotate ^= 1;
			
		}
		else pressedrotate = false;
		
		/* 列が一致していない場合 */
		if (kumipuyo.x != (desire.index - 1)) {
			if (!pressedmove) {
				if (desire.index - 1 < kumipuyo.x) {
					left_operate = 1;
					right_operate = 0;
				}
				else if (desire.index - 1 > kumipuyo.x) {
					left_operate = 0;
					right_operate = 1;
				}
			}
			else	right_operate = left_operate = 0;
			/*ボタンが押されているかどうかのフラグを反転*/
			pressedmove ^= 1;
			if (kumipuyo.y <= 12.0) down_operate = 1;//回転と横移動を先に入れる
			}
		else {
			pressedmove = false;
			down_operate = 1;
		}		

		ai_SetPad(up_operate, right_operate, down_operate, left_operate
			, a_rotate, b_rotate);
}

/*
 *引数のmoveinfoのfileIndexとrot通りにぷよを落とす。
 *DropByPadと違い,
 *1.下入力はしない.
 *2.回しを考慮した回転
 */
void BpuyoPad::Move(const Kumipuyo& kumipuyo){
	const PutType& desire = kumipuyo.desirable_put;
	static bool pressedrotate = false;
	static bool pressedmove = false;

	int up_operate = 0;
	int right_operate = 0;
	int down_operate = 0;
	int left_operate = 0;
	int a_rotate = 0;
	int b_rotate = 0;
/* 回転が一致していない場合 */
	if (kumipuyo.rotate != desire.rotate) {
		if (!pressedrotate) {

			if (kumipuyo.rotate == RotateType::ROTATE_90) {
				if (desire.rotate == RotateType::ROTATE_180)b_rotate = 1, a_rotate = 0;
				else a_rotate = 1, b_rotate = 0;
			}
			else if (kumipuyo.rotate == RotateType::ROTATE_270) {
				if (desire.rotate == RotateType::ROTATE_180)b_rotate = 0, a_rotate = 1;
				else a_rotate = 0, b_rotate = 1;
			}
			else {a_rotate = 1, b_rotate = 0;
			}
		}
		else {
			a_rotate = b_rotate = 0;
		}
		/*ボタンが押されているかどうかのフラグを反転*/
		pressedrotate ^= 1;
			
	}
	else pressedrotate = false;
		
	/* 列が一致していない場合 */
	if (kumipuyo.x != (desire.index - 1)) {
		if (!pressedmove) {
			if (desire.index - 1 < kumipuyo.x) {
				left_operate = 1;
				right_operate = 0;
			}
			else if (desire.index - 1 > kumipuyo.x) {
				left_operate = 0;
				right_operate = 1;
			}
		}
		else	right_operate = left_operate = 0;
		/*ボタンが押されているかどうかのフラグを反転*/
		pressedmove ^= 1;
		}
	else {
		pressedmove = false;
	}		

	ai_SetPad(up_operate, right_operate, down_operate, left_operate
		, a_rotate, b_rotate);
}

