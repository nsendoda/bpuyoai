#ifndef BPUYOAI_BPUYOPAD_H_
#define BPUYOAI_BPUYOPAD_H_

#include "../puyo_ai.h"
#include "pad.h"
#include "command.h"

#include <initializer_list>
#include <algorithm>

// Pad���p�����Ă��邽�߁APad��protected:pad[PAD_SIZE]�𒼐ڕҏW�ł���B
class BpuyoPad : public Pad {
public:
	// ���ꂼ��pad_���X�V���āAai_setpad�����s
	// NEUTRAL���n����Ȃ�����͑O�񉟂��ꂽ�{�^���͂��̂܂�
	void Press(const std::initializer_list<Command>& commands);
	void Press(Command c);
	void Press(const Pad& pad);

	// pad��S��false�ɂ���
  // button �͉����Ȃ�
	void Neutral();

	// GIVEUP����
	void GiveUp();
	
	// �����p
	void Dance(int frame_ct);
	void Dance2(int frame_ct);

private:
	void UpdateButton(bool* pad, Command c);
};

#endif
