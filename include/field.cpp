#include "field.h"

/// 壁埋め, rows_heightの初期化
Field::Field() {
  std::fill_n(field_, COLUMN, Color::WALL);
  for (int i = FIELD_START - 1; i <= FIELD_END + 1; i += COLUMN)
    field_[i] = Color::WALL;
  std::fill_n(lowest_empty_rows_index_, COLUMN, 1);
}

Field::Field(const Field &field) {
  std::copy(field.field_, field.field_ + FIELD_SIZE, field_);
  std::copy(field.lowest_empty_rows_index_,
            field.lowest_empty_rows_index_ + COLUMN, lowest_empty_rows_index_);
}

Field::~Field() {}

/*
 * void Field::Output() const {
  for (int i = FIELD_END - 5; i >= FIELD_START; i++) {
    if (i % COLUMN == 0) {
      i -= 2 * COLUMN;
      Debug::Print("\n");
    } else
      Debug::Print(" %d", field_[i]);
  }
  Debug::Print("\nrows_height:");
  for (int i = 1; i < Field::COLUMN; ++i) {
    Debug::Print(" %d", lowest_empty_rows_index_[i]);
  }
  Debug::Print("\n");
}
*/
