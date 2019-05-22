#ifndef BPUYOAI_MODE_H_
#define BPUYOAI_MODE_H_

enum Mode : int {
	OPERATION = 1,
	CHAIN_CHECK = 3,
	CHAIN = 4,
	WAIT_5 = 5,
	WAIT_6 = 6,
	OJAMA_DROP = 7,
	LOSE = 8,
	WIN = 9,
	LEVEL = 10,
	LEVEL_AFTER = 11,
	// クイック後の短縮されたWAIT_6
	QUICK_WAIT = 12
};

#endif