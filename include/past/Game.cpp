#include "Game.h"

Game::Game():
	myself(Parameter::MYSELF),
	enemy(Parameter::ENEMY)
{
}


Game::~Game() {
}


/// 毎フレームのぷよの位置をアップデートする
/// ターンが切り替わった時は、それを変数に格納する
/// また、敵の連鎖を検知する
void Game::Update() {
	myself_turn_changed = myself.UpdateWhenDetectTurnChange();
	enemy.UpdateWhenDetectTurnChange();
	if (DetectedEnemyChain()) {
		enemy.field.LoadFromBpuyo();
		enemy.field.Output();
	}
}


//
/// 思考ルーチン,ai_mainから呼び出される
void Game::Think() {
	if (myself.decided == false) {
		if (myself.turn <= 2) myself.now_kumipuyo.desirable_put = PutType::GetPattern(8);
		else myself.now_kumipuyo.desirable_put = ai.Decide(myself);
		myself.decided = true;
	}
	BpuyoPad::Drop(myself.now_kumipuyo);
}


//相手が連鎖中ならtrueを返す
bool Game::DetectedEnemyChain() {
	static AI_SCORE* enemychain = ai_GetScore2(Parameter::ENEMY, -1, 1);
	if (enemychain->score == -2)return true;
	else return false;
}
