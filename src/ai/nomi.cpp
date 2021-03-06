#include "nomi.h"

Nomi::Nomi() :
	state(MYSELF),
	enemy(ENEMY),
	mawashi(ai_GetDropSpeed(MYSELF)){}



void Nomi::Init() {

#ifdef TYPE_CHAIN
	ai_SetName("NOMI");
#else
	ai_SetName("NOMI_TOWER");
#endif
	ai_SetBMainMode(1);
	ai_SetMainMode(1);

#ifdef USE_DATABASE
	for (int i = 0; i < DATABASE_SIZE; i++)
		RawData::SetDatabase(i, &database[i]);
#endif

//	ai_SetTable(1, 0, "PY RR GY PP RR PP RG RR PY PR RR RY RY YG RR GR RG RG PY PP YR PP ");

}

void Nomi::PreProcess() {


	state.UpdateUnitFrame();
	enemy.UpdateUnitFrame();

	enemy.DetectTurnChange();

	if (enemy.VisibleStateFirstTurn()) {
		enemy.UpdateUnitHand();
	}


	// お邪魔テーブルを相手の連鎖発火から検知
	state.DetectOjama(&enemy);
	enemy.DetectOjama(&state);

	// 相手が連鎖をしていない時は、常にお邪魔量を更新
	state.UpdateOjama(enemy);
	enemy.UpdateOjama(state);


	// 有効スコアを更新
	Debug::Print("preprocess: mymode:%d, enmode: %d, turn: %d, frame:%d\n", ai_GetPlayerMode(MYSELF),
		ai_GetPlayerMode(ENEMY), ai_GetCount2(ENEMY), state.hand_frame);

}

void Nomi::Main() {

	Debug::Print("main: frame:%d\n", state.hand_frame);

	if (state.DetectTurnChange()) {

		mawashi.Init(ai_GetDropSpeed(MYSELF));

		my_pad.Press(NEUTRAL);
		return;
	}

	if (state.VisibleStateFirstTurn()) {
		
		state.UpdateUnitHand();


		Decide();
		PadDecide();
		
	}
	if (mawashi.ShouldMawashi(state, enemy)) {
		mawashi.Execute(state, &my_pad);
		return;
	}
	Operate();
}

void Nomi::Decide() {


#ifdef USE_DATABASE
	int color_count = ColorPuyoCount();
	if (color_count < DATABASE_SIZE) {
		PutIndex database_put = database[color_count].BestIndex(state);
	Debug::Print("data: %d", database_put);
		if (database_put != -1) {
			state.now_kumipuyo.desirable_put = PutType::GetPutType(database_put);
			ai_SetName("FOUND");
			return;
		}
		ai_SetName("NOT FOUND");
	}
#endif


	if (state.turn <= 3) {
		static std::vector<PutType> ans;
		static Kumipuyo pre(EMPTY, EMPTY);

		if (state.turn < 3) {
			ans = NomiThink::ConstantPut(state, pre);
			pre = state.now_kumipuyo;
		}

		state.now_kumipuyo.desirable_put = ans[state.turn - 1];
		return;
	}

	Score temporary_fatal_dose = NomiThink::CalculateFatalDose(enemy, enemy.ojamas.SumOjama());
	Score fatal_dose = 4550; // 65

	// 相手がある程度以上お邪魔で埋まっているならその分減らす
	const int EFFECTIVE_OJAMA = 12;
	int enemy_field_ojama = enemy.field.CountOjama();
	if (enemy_field_ojama >= EFFECTIVE_OJAMA) fatal_dose -= enemy_field_ojama * Ojama::ONE_SCORE;

	// とりあえず消しに行かなきゃいけない空きぷよ
	const int DEADLY_EMPTY = 24;

	FieldIndex kill_index;
	if (NomiThink::KillThink(state, fatal_dose, &kill_index)) {
		state.now_kumipuyo.desirable_put = PutType(kill_index);
		Debug::Print("kill decide. turn:%d, c:%d, rotate:%d\n", state.turn, state.now_kumipuyo.desirable_put.column, state.now_kumipuyo.desirable_put.rotate);
		return;
	}

	// 相手は発火を決定したため、発火後の形は既に見えているので、その発火後の致死を送れば良い
	if (NomiThink::ReactJab(state, temporary_fatal_dose, &kill_index, false)) {
		state.now_kumipuyo.desirable_put = PutType(kill_index);
		Debug::Print("react decide. turn:%d, c:%d, rotate:%d\n", state.turn, state.now_kumipuyo.desirable_put.column, state.now_kumipuyo.desirable_put.rotate);
		return;
	}

	// フィールド状況がまずくなったらとりあえず消しに行く
	if (state.field.CountMawashiEmptyPuyos() <= DEADLY_EMPTY && 
		NomiThink::ReactJab(state, temporary_fatal_dose, &kill_index, true)) {
		state.now_kumipuyo.desirable_put = PutType(kill_index);
		Debug::Print("make empty decide. turn:%d, c:%d, rotate:%d\n", state.turn, state.now_kumipuyo.desirable_put.column, state.now_kumipuyo.desirable_put.rotate);
		return;
	}

	state.now_kumipuyo.desirable_put = NomiThink::ChainThink(state, fatal_dose);

	Debug::Print("decide. turn:%d, c:%d, rotate:%d\n", state.turn, state.now_kumipuyo.desirable_put.column, state.now_kumipuyo.desirable_put.rotate);
}

void Nomi::PadDecide() {
	pad_orders = PadSearch::DropOrder(state.now_kumipuyo, state.field, my_pad, false);

	Row row_3 = state.field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN);
	Row row_2 = state.field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN - 1);
	Row row_4 = state.field.GetLowestEmptyRows(Field::PUYO_APPEAR_COLUMN + 1);
	Column dc = state.now_kumipuyo.desirable_put.column;
		Row put_row = state.field.GetLowestEmptyRows(state.now_kumipuyo.desirable_put.column);
		if (
			(put_row < 10
				&& (dc >= Field::PUYO_APPEAR_COLUMN || row_2 < 11)
				&& (dc <= Field::PUYO_APPEAR_COLUMN || row_4 < 11)
				&& row_3 < 12  ) ) {
			pad_orders = PadSearch::DropOrder(state.now_kumipuyo, state.field, my_pad, true);
		}
}

void Nomi::Operate() {
	// この式はframe_ct==1の時にpad_ordersを求めてる場合のみ有効なことに注意
	if (state.hand_frame < pad_orders.size()) {
		my_pad.Press(pad_orders[state.hand_frame]);
		return;
	}
	// 1ターン遅らせる
	if (state.hand_frame == pad_orders.size() + 1){
		cancel_que = PadSearch::CancelDrop(state.now_kumipuyo, state.field, my_pad);
	}
	
	if(cancel_que.empty()){
		my_pad.Press(DOWN);
	}
	else {
		my_pad.Press(cancel_que.front());
		cancel_que.pop();
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