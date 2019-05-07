#include "nomi.h"

Nomi::Nomi() :
	state(MYSELF),
	enemy(ENEMY),
	mawashi_state(ai_GetDropSpeed(MYSELF)){}



void Nomi::Init() {

	ai_SetBMainMode(1);
	ai_SetMainMode(1);
//	ai_SetTable(1, 0, "GR GR GG YG YY YP PP");
	ai_SetTable(1, 0, "GG GR GG YG YY YP PP");
	for (int i = 0; i < DATABASE_SIZE; i++)
		RawData::SetDatabase(i, &database[i]);



}

void Nomi::PreProcess() {
	enemy.DetectTurnChange();
	enemy.UpdateFrame();

	if (enemy.VisibleStateFirstTurn()) {
		enemy.Update();
	}
}

void Nomi::Main() {

	state.UpdateFrame();

	if (state.DetectTurnChange()) {
		mawashi_state.Init(ai_GetDropSpeed(MYSELF));

		my_pad.Press(NEUTRAL);

	}
	else if (state.VisibleStateFirstTurn()) {
		state.Update();
		Decide();
		PadDecide();
		Operate();
	}
	else {
		Operate();
	}

}

void Nomi::Decide() {

	if (KillThink(100)) {
		return;
	}

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

	Think();

}

bool Nomi::KillThink(Score fatal_dose) {
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {

	}
	return false;
}

void Nomi::Think() {
	for (PutIndex pi = 0; pi < PUTTYPE_PATTERN; pi++) {

	}
	state.now_kumipuyo.desirable_put = PutType::GetPutType(2);
}

void Nomi::PadDecide() {
		pad_orders = PadSearch::DropOrder(state.now_kumipuyo, state.field, my_pad);
		if (pad_orders.empty()) {
//			ai_SetName("KABEGOE");
			pad_orders = PadSearch::CarefulOrder(state.now_kumipuyo, state.field, my_pad);
		}
}

void Nomi::Operate() {
	// ‚±‚ÌŽ®‚Íframe_ct==1‚ÌŽž‚Épad_orders‚ð‹‚ß‚Ä‚éê‡‚Ì‚Ý—LŒø‚È‚±‚Æ‚É’ˆÓ
	if (state.hand_frame < pad_orders.size()) {
		my_pad.Press(pad_orders[state.hand_frame]);
	}
	else {
		my_pad.Neutral();
		my_pad.Press(DOWN);
	}
}

void Nomi::MakeDatabase(int puyocount) {
	std::string filename = "C:/Users/nosi/Dropbox/project/BpuyoAI_sea/src/ai/data/puyocount_bit_"
		+ std::to_string(puyocount) + ".toml";
	std::ifstream ifs(filename.c_str());
	toml::ParseResult pr = toml::parse(ifs);

	if (!pr.valid()) {
		char cdir[255];
		GetCurrentDirectory(255, cdir);
		Debug::Print("Current Directory : %s", cdir);

		Debug::Print("%s/n", pr.errorReason);
		ai_SetName("DAME");
		return;
	}

	toml::Value p = pr.value;
	ai_SetName("NOMI");
	toml::Array turns = p.get< toml::Array>("turn");
	for (toml::Value turn : turns) {
		__int64 f1 = turn.get<__int64>("field1");
		__int64 f2 = turn.get<__int64>("field2");
		__int64 f3 = turn.get<__int64>("field3");
		__int64 f4 = turn.get<__int64>("field4");
		__int64 now = turn.get<__int64>("now");
		__int64 next = turn.get<__int64>("next");
		std::vector <PutIndex> putcts = turn.get<std::vector<PutIndex> >("index");
		database[puyocount].Insert(f1, f2, f3, f4, now, next, putcts);
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