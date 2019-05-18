#include "researcher.h"

void Researcher::Init() {
	unsigned char fi[109];
	std::fill(fi, fi + 109, 0);
	for (int i = 0; i < 12; i++) {
		//		fi[i * 6 + 2] = 6;
		fi[i * 6 + 5] = 6;
	}
	for (int i = 0; i < 11; i++) {
		//		fi[i * 6 + 3] = 6;
		fi[i * 6 + 3] = 6;
		//		fi[i * 6 + 5] = 6;
		//		fi[i * 6 + 6] = 6;
	}

	ai_SetField(1, fi);

	ai_SetDefSpeed(9); // デフォルト速度
	ai_SetTable(1, 0, "RR RG GG YG YY YP PP");
	ai_SetName("REPLAY INVESTIGATION");
	ai_SetBMainMode(1);
	ai_SetMainMode(1);
}

void Researcher::PreProcess() {
	static State busy_state(MYSELF);
	static int frame_busy_ct = 0;

	if (ai_GetPlayerMode(MYSELF) != 10 && ai_GetPlayerMode(MYSELF) != 11) {
		/*if (busy_state.UpdateWhenDetectTurnChange()) {
			frame_busy_ct = 0;
			Debug::Print("turn is changed! now_turn: %d\n", busy_state.turn);
		}
		*/

#ifdef PRINT
		if (ai_GetPlayerMode(MYSELF) != 1)
			/*			//		Debug::Print("turn is %d. frame is %d. \n", busy_state.turn, ++frame_busy_ct);
			*/
			//		Debug::Print("turn is %d. frame is %d. \n", busy_state.turn, ++frame_busy_ct);
			Debug::Print("mymode:%d, enmode: %d, turn: %d\n", ai_GetPlayerMode(MYSELF),
				ai_GetPlayerMode(ENEMY), ai_GetCount2(ENEMY));


#endif

	}
}

void Researcher::Main() {
	static State state(MYSELF);
	static State enemy(ENEMY);

	static BpuyoPad my_pad;

	static MawashiState mawashi_state(ai_GetDropSpeed(MYSELF));


	static std::vector<Pad> pad_orders;

	// #define DEBUG

	static int frame_ct = 0;



	frame_ct++;

	int MAWASHI_COUNT = 57;
	static int mawashi_ct = 0;

	enemy.DetectTurnChange();
	state.DetectTurnChange();

	if (state.VisibleStateFirstTurn()) {
		frame_ct = 0;
		static int r = 20;
		state.now_kumipuyo.desirable_put = PutType::GetPutType(r);
		mawashi_state.Init(ai_GetDropSpeed(MYSELF));
		r++; r %= 22;
		my_pad.Press(NEUTRAL);
		return;
	}
#undef DEBUG
#ifdef DEBUG
	/*
	else if (frame_ct == 1) {
		my_pad.Press({RIGHT, ROTATE_LEFT });
		mawashi_ct = 0;
	}
	else if (frame_ct == 15) {
		Pad pad;
		my_pad.Press({ RIGHT });
	}
	else if (frame_ct == MAWASHI_COUNT) {
		my_pad.Press(ROTATE_LEFT);
	}
	else if (frame_ct == MAWASHI_COUNT + 1) {
		my_pad.Press({ ROTATE_RIGHT });
	}
	else if (frame_ct == MAWASHI_COUNT + 2) {
		my_pad.Press(LEFT);
	}
	else if (mawashi_ct == 9) {
		mawashi_ct++;
		my_pad.Press({ LEFT, ROTATE_RIGHT });
	}
	else if (mawashi_ct > 9) {
		my_pad.Press(NEUTRAL);
	}
	else if (frame_ct > 52 && (frame_ct - MAWASHI_COUNT) % 25 == 0) {
		my_pad.Press({ ROTATE_LEFT, RIGHT });
	}
	else if (frame_ct > 52 && (frame_ct - MAWASHI_COUNT) % 25 == 1) {
		my_pad.Press({ ROTATE_RIGHT });
	}
	else if (frame_ct > 52 && (frame_ct - MAWASHI_COUNT) % 25 == 3) {
		my_pad.Press({ LEFT });
		mawashi_ct++;
}

	else {
		my_pad.Press(NEUTRAL);
	}
*/
	else {

		mawashi_state.Excute(state, &my_pad);
	}
#else
	else if (frame_ct == 1) {
		pad_orders = PadSearch::DropOrder(state.now_kumipuyo, state.field, my_pad, true);
		if (pad_orders.empty()) {
			ai_SetName("KABEGOE");
			pad_orders = PadSearch::DropOrder(state.now_kumipuyo, state.field, my_pad,false );
		}
		else {
			ai_SetName("DROP");

		}
	}
	// この式はframe_ct==1の時にpad_ordersを求めてる場合のみ有効なことに注意
	if (frame_ct < pad_orders.size()) {
		my_pad.Press(pad_orders[frame_ct]);
	}
	else {
		my_pad.Neutral();
		my_pad.Press(DOWN);
	}
#endif

#ifdef PUYO_PRINT
	Debug::Print("x is %f, y is %f, rot is %d. ct %d\n", state.now_kumipuyo.X(), state.now_kumipuyo.Y(), state.now_kumipuyo.now_rotate, frame_ct);
#endif

	/*	if (frame_ct == 0) {
	//		BpuyoPad::Press(RIGHT);
			BpuyoPad::Press(NEUTRAL);
		}
		else if (karumeru) {
			int parent, child;
			int now_parent, now_child;
			std::tie(parent, child) = Simulator::CalculatePutIndex(state.now_kumipuyo.desirable_put, state.field);
			std::tie(now_parent, now_child) = state.now_kumipuyo.GetPairIndex();
			static int rest_frame = 8;
			if (!state.now_kumipuyo.YHasPointFive()){
				if (now_parent == parent && now_child == child) {
					rest_frame--;
					if (rest_frame == 1) {
						BpuyoPad::Press(ROTATE_LEFT);
					}
					else if (rest_frame == 0) {
						BpuyoPad::Press({ LEFT, ROTATE_RIGHT });
						rest_frame = 8;
					}
					else {
						BpuyoPad::Press(NEUTRAL);
					}
				}
				else {
					rest_frame--;
					if (rest_frame == 1) {
						BpuyoPad::Press(RIGHT);
					}
					else if (rest_frame == 0) {
						BpuyoPad::Press(ROTATE_LEFT);
						rest_frame = 8;
					}
					else {
						BpuyoPad::Press(NEUTRAL);
					}
				}
			}
			else{
					BpuyoPad::Press(NEUTRAL);
			}
		}
		else if (frame_ct > 0) {
			int parent, child;
			int now_parent, now_child;

			std::tie(parent, child) = Simulator::CalculatePutIndex(state.now_kumipuyo.desirable_put, state.field);
			std::tie(now_parent, now_child) = state.now_kumipuyo.GetPairIndex();
			Debug::Print("n_p, n_c = %d, %d, p, c = %d, %d\n", now_parent, now_child, parent, child);
			if (! state.now_kumipuyo.YHasPointFive() && now_parent == parent && now_child == child) {
					karumeru = true;
					Debug::Print("CANCEL_DROPMODE\n");
					BpuyoPad::Press({ NEUTRAL });
				}
			else {
				BpuyoPad::Press(DOWN);
			}
		}*/

}