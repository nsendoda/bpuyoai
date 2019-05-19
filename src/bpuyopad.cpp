#include "bpuyopad.h"

void BpuyoPad::Press(const std::initializer_list<Command>& commands) {
	// 0   1      2     3     4            5
	// up, right, down, left, rotate_left, rotate_right
	Neutral();
	for (Command c : commands) {
		UpdateButton(pad, c);
	}
	ai_SetPad(pad[0], pad[1], pad[2], pad[3], pad[4], pad[5]);
}

void BpuyoPad::Press(Command c) {
	Neutral();
	UpdateButton(pad, c);
	ai_SetPad(pad[0], pad[1], pad[2], pad[3], pad[4], pad[5]);
}

void BpuyoPad::Press(const Pad& src_pad) {
	src_pad.CopyTo(pad);
	ai_SetPad(pad[0], pad[1], pad[2], pad[3], pad[4], pad[5]);
}

void BpuyoPad::Neutral() {
	std::fill(pad, pad + 6, false);
}

void BpuyoPad::GiveUp()
{
	ai_SetPad(1, 0, 0, 0, 1, 1);
}

void BpuyoPad::Dance(int frame_ct) {
	if (frame_ct == 0)
		BpuyoPad::Press(LEFT);
	else if (frame_ct == 2)
		BpuyoPad::Press(LEFT);
	else if (frame_ct % 20 == 0 || frame_ct % 20 == 2 || frame_ct % 20 == 4
		|| frame_ct % 20 == 6 || frame_ct % 20 == 8)
		BpuyoPad::Press(RIGHT);
	else if (frame_ct % 20 == 10 || frame_ct % 20 == 12 || frame_ct % 20 == 14
		|| frame_ct % 20 == 16 || frame_ct % 20 == 18)
		BpuyoPad::Press(LEFT);
	else
		BpuyoPad::Press(NEUTRAL);
}

void BpuyoPad::Dance2(int frame_ct) {
	if (frame_ct == 1)
		BpuyoPad::Press(ROTATE_RIGHT);
	else if (frame_ct == 2)
		BpuyoPad::Press(NEUTRAL);
	else if (frame_ct % 2)
		BpuyoPad::Press(ROTATE_RIGHT);
	else
		BpuyoPad::Press(ROTATE_LEFT);
}


// private below
void BpuyoPad::UpdateButton(bool* pad, Command c) {
	if (c == Command::ROTATE_RIGHT) {
		pad[Command::ROTATE_RIGHT] = true;
	}
	if (c == Command::ROTATE_LEFT) {
		pad[ROTATE_LEFT] = true;
	}
	if (c == Command::DOWN) {
		pad[DOWN] = true;
	}
	if (c == Command::LEFT) {
		pad[LEFT] = true;
	}
	if (c == Command::RIGHT) {
		pad[RIGHT] = true;
	}
	if (c == Command::LEFTDOWN) {
		pad[LEFT] = true;
		pad[DOWN] = true;
	}
	if (c == Command::RIGHTDOWN) {
		pad[RIGHT] = true;
		pad[DOWN] = true;
	}
	if (c == Command::NEUTRAL) {
		std::fill(pad, pad + 6, false);
	}
}
