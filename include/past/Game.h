#pragma once


#include "types.h"
#include "puyo_ai.h"
#include "Parameter.hpp"
#include "Field.h"
#include "Simulator.h"
#include "BpuyoPad.h"
#include "State.h"
#include "Nomi.h"



class Game
{
public:
	State myself, enemy;
	bool myself_turn_changed;
	Nomi ai;
	Game();
	~Game();
	//ゲームの状態を更新,ai_mainbから呼び出される
	void Update();
	//思考ルーチン,ai_mainから呼び出される
	void Think();
	//相手が連鎖中ならtrueを返す
	bool DetectedEnemyChain();
private:
};

