#ifndef INCLUDED_PAD_H_
#define INCLUDED_PAD_H_

#include "command.h"

#include <algorithm>


// Ç±ÇÃÉNÉâÉXÇÕBpuyoPadÇ…åpè≥Ç≥ÇÍÇ‹Ç∑ÅB
struct Pad {

	static constexpr int PAD_SIZE = 6;

	// zero init
	Pad() : pad() {}
	Pad(bool* pad_) { std::copy(pad_, pad_ + PAD_SIZE, pad); }
	Pad(int pad_) {
		for (int i = 0; i < PAD_SIZE; i++)
			pad[i] = (pad_ >> i) & 1;
	}

	bool IsUp()   const { return pad[Command::UP]; }
	bool IsLeft() const { return pad[Command::LEFT]; }
	bool IsDown() const { return pad[Command::DOWN]; }
	bool IsRight() const { return pad[Command::RIGHT]; }
	bool IsRotateRight() const { return pad[Command::ROTATE_RIGHT]; }
	bool IsRotateLeft() const { return pad[Command::ROTATE_LEFT]; }
	bool Is(int index) const { return pad[index]; }

	void SetUp(bool flag) { pad[Command::UP] = flag; }
	void SetLeft(bool flag) { pad[Command::LEFT] = flag; }
	void SetDown(bool flag) { pad[Command::DOWN] = flag; }
	void SetRight(bool flag) { pad[Command::RIGHT] = flag; }
	void SetRotateRight(bool flag) { pad[Command::ROTATE_RIGHT] = flag; }
	void SetRotateLeft(bool flag) { pad[Command::ROTATE_LEFT] = flag; }

	void CopyTo(bool* new_pad) const {
		std::copy(pad, pad + PAD_SIZE, new_pad);
	}

	bool Equals(const Pad& pad_) const {
		bool pad_t[PAD_SIZE];
		pad_.CopyTo(pad_t);
		for (int i = 0; i < PAD_SIZE; i++) if (pad[i] != pad_t[i]) return false;
		return true;
	}

protected:
	bool pad[PAD_SIZE];
};

#endif