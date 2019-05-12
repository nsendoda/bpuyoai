#include "nomi.h"

Nomi::Nomi() :
	state(MYSELF),
	enemy(ENEMY),
	mawashi_state(ai_GetDropSpeed(MYSELF)){}



void Nomi::Init() {

	ai_SetBMainMode(1);
	ai_SetMainMode(1);
//	ai_SetTable(1, 0, "GR GR GG YG YY YP PP");
	ai_SetTable(1, 0, "GR GR GG YG YY YP PP");
#ifdef USE_DATABASE
	for (int i = 0; i < DATABASE_SIZE; i++)
		RawData::SetDatabase(i, &database[i]);
#endif
}

void Nomi::PreProcess() {


	state.UpdateUnitFrame();
	enemy.UpdateUnitFrame();

	enemy.DetectTurnChange();

	if (enemy.VisibleStateFirstTurn()) {
		enemy.UpdateUnitHand();
	}


	// お邪魔テーブルを相手の連鎖発火から検知
	state.DetectOjama(enemy);
	enemy.DetectOjama(state);

	// 有効スコアを更新
//	state

	Debug::Print("preprocess: mymode:%d, enmode: %d, turn: %d\n", ai_GetPlayerMode(MYSELF),
		ai_GetPlayerMode(ENEMY), ai_GetCount2(ENEMY));

}

void Nomi::Main() {


	if (state.DetectTurnChange()) {

		mawashi_state.Init(ai_GetDropSpeed(MYSELF));

		my_pad.Press(NEUTRAL);
		return;
	}

	if (state.VisibleStateFirstTurn()) {
		
		state.UpdateUnitHand();

		Decide();
		PadDecide();
		
	}
	Operate();
}

void Nomi::Decide() {


/*	int color_count = ColorPuyoCount();
	if (color_count < DATABASE_SIZE) {
		PutIndex database_put = database[color_count].BestIndex(state);
	Debug::Print("data: %d", database_put);
		if (database_put != -1) {
			state.now_kumipuyo.desirable_put = PutType::GetPutType(database_put);
			ai_SetName("FOUND");
			return;
		}
		ai_SetName("NOT FOUND");
	}*/

	if (state.turn <= 3) {
		static std::vector<PutType> ans;
		static Kumipuyo pre(EMPTY, EMPTY);

		if (state.turn < 3) {
			ans = NomiThink::ConstantPut(state, pre);
			pre = state.now_kumipuyo;
		}

		state.now_kumipuyo.desirable_put = ans[state.turn];
		return;
	}

	Score fatal_dose = NomiThink::CalculateFatalDose(state);

	state.now_kumipuyo.desirable_put = NomiThink::Think(state, fatal_dose);

}

void Nomi::PadDecide() {
		pad_orders = PadSearch::DropOrder(state.now_kumipuyo, state.field, my_pad);
		if (pad_orders.empty()) {
//			ai_SetName("KABEGOE");
			pad_orders = PadSearch::CarefulOrder(state.now_kumipuyo, state.field, my_pad);
		}
}

void Nomi::Operate() {
	// この式はframe_ct==1の時にpad_ordersを求めてる場合のみ有効なことに注意
	if (state.hand_frame < pad_orders.size()) {
		my_pad.Press(pad_orders[state.hand_frame]);
	}
	else {
		my_pad.Neutral();
		my_pad.Press(DOWN);
	}
}


int Nomi::ColorPuyoCount() {
	int ret = 0;
	for (FieldIndex i = Field::FIELD_START;
		i <= Field::FIELD_END; i++) {
		if (state.field[i] == Color::WALL) continue;
		if (state.field[i] > Color::EMPTY && state.field[i] < Color::OJAMA) ret++;
	}
	return ret;
}