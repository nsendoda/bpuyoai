#ifndef BPUYOAI_BPUYOPAD_H_
#define BPUYOAI_BPUYOPAD_H_

#include "../puyo_ai.h"
#include "pad.h"
#include "command.h"

#include <initializer_list>
#include <algorithm>

// Padを継承しているため、Padのprotected:pad[PAD_SIZE]を直接編集できる。
class BpuyoPad : public Pad {
public:
	// それぞれpad_を更新して、ai_setpadを実行
	// NEUTRALが渡されない限りは前回押されたボタンはそのまま
	void Press(const std::initializer_list<Command>& commands);
	void Press(Command c);
	void Press(const Pad& pad);

	// padを全てfalseにする
  // button は押さない
	void Neutral();

	// GIVEUPする
	void GiveUp();
	
	// 実験用
	void Dance(int frame_ct);
	void Dance2(int frame_ct);

private:
	void UpdateButton(bool* pad, Command c);
};

#endif
