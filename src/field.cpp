#include "field.h"

/// 壁埋め, rows_heightの初期化
Field::Field() {
	std::fill_n(field_, FIELD_SIZE, Color::EMPTY);
  std::fill_n(field_, COLUMN, Color::WALL);
  for (int i = FIELD_START - 1; i <= FIELD_END + 1; i += COLUMN)
    field_[i] = Color::WALL;
  InitLowestEmptyRows();
}

Field::Field(const Field &field) {
  std::copy(field.field_, field.field_ + FIELD_SIZE, field_);
  std::copy(field.lowest_empty_rows_, field.lowest_empty_rows_ + COLUMN,
            lowest_empty_rows_);
}

Field::~Field() {}

// 13段目までの空いているマスの数を数える
int Field::CountEmptyPuyos() const {
	int ans = 0;
	for(int c = 1; c < COLUMN; c++){
		ans += 14 - GetLowestEmptyRows(c);
	}
	return ans;
}

// 12列目までで3列目12段から到達できる空いているマスの数を数える
int Field::CountMawashiEmptyPuyos() const {
	int ans = 0;
	for (int c = 1; c < COLUMN; c++) {
		ans += std::max(0, 13 - GetLowestEmptyRows(c));
	}
	return ans;
}

// 13段目までの各列の空いているぷよの最低段数を数える
void Field::ModifyLowestEmptyRows() {
  for (int column = 1; column < COLUMN; column++) {
    int empty_row = 1;
    while (field_[Field::COLUMN * empty_row + column] != Color::EMPTY &&
           Field::COLUMN * empty_row + column < FIELD_END) {
      empty_row++;
    }
    lowest_empty_rows_[column] = empty_row;
  }
}
void Field::AddLowestEmptyRows(Column column, int ct) {
  if (column <= 0 || column >= Field::COLUMN) {
    Debug::Printf("addlowestemptyrows error. column is invalid.");
  }
  Row row = lowest_empty_rows_[column];
  lowest_empty_rows_[column] = std::max(
      std::min(row + ct, LOWEST_EMPTY_ROWS_MAX), LOWEST_EMPTY_ROWS_MIN);
}

// @TODO 0アクセスの時例外を投げる
int Field::GetLowestEmptyRows(int column) const {
  if (column <= 0 || column >= Field::COLUMN) {
    Debug::Printf("addlowestemptyrows error. column is invalid.");
  }
  return lowest_empty_rows_[column];
}

void Field::OutputIncludeWall() const {
  std::map<Color, char> colormap = {{Color::EMPTY, 'O'},  {Color::RED, 'R'},
                                    {Color::YELLOW, 'Y'}, {Color::PURPLE, 'P'},
                                    {Color::GREEN, 'G'},  {Color::BLUE, 'B'},
                                    {Color::OJAMA, 'X'},  {Color::WALL, 'W'}};
  for (int i = FIELD_SIZE - COLUMN;; i++) {
    Debug::Printf(" %d", field_[i]);
    if (i == COLUMN - 1)
      break;
    else if (i % COLUMN == (COLUMN - 1)) {
      i -= 2 * COLUMN;
      Debug::Printf("\n");
    }
  }
  Debug::Printf("\nrows_height:");
  for (int i = 1; i < Field::COLUMN; ++i) {
    Debug::Printf(" %d", lowest_empty_rows_[i]);
  }
  Debug::Printf("\n");
}

void Field::Output() const {
  for (int i = FIELD_END - 5; i >= FIELD_START; i++) {
    if (i % COLUMN == 0) {
      i -= 2 * COLUMN;
      Debug::Printf("\n");
    } else
      Debug::Printf(" %d", field_[i]);
  }
  Debug::Printf("\nrows_height:");
  for (int i = 1; i < Field::COLUMN; ++i) {
    Debug::Printf(" %d", lowest_empty_rows_[i]);
  }
  Debug::Printf("\n");
}

bool Field::Equals(const Field &target) {
  for (int i = 0; i < FIELD_SIZE; i++) {
    if (field_[i] != target[i])
      return false;
  }
  return true;
}
