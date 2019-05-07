#ifndef BPUYOAI_FIELD_H_
#define BPUYOAI_FIELD_H_

#include <algorithm>
#include <cstring>

#include "types.h"

class Field {
  /*
   * 盤面は次のように構成されている.
   * 14  W******   --回し専用スペース.
   * ここにぷよが置かれても連鎖に関わることはない.
   * 13  W******
   * 12  W******
   * 11  W******
   * 10  W******
   *  9  W******
   *  8  W******
   *  7  W******
   *  6  W******
   *  5  W******
   *  4  W******
   *  3  W******
   *  2  W******
   *  1  W******
   *  0  WWWWWWWW
   *
   *  W は壁を表し, 番兵の役割をする.
   * 配列でのインデックスは次のようになっている.
   * 14  98 99 100 101 102 103 104 105
   * 13  91 92 93 94 95 96 97
   *  .
   *  .
   *  .
   *  1  789....
   *  0  0123456
   *
   *  8方向は次で表せる.
   *   +6     +7     +8
   *   -1  Position  +1
   *   -6    -7      -8
   */
public:
  static const int COLUMN = 7;
  static const int ROW = 15;
  static const int FIELD_SIZE = COLUMN * ROW + 1; /// 106
  static const int FIELD_START = 8;               /// 1段目の1列目
  static const int FIELD_END = 97;                /// 13段目の6列目
  static const int FIELD_DEATH = 87;
  static const int PUYO_APPEAR_COLUMN = 3;

  Field();
  Field(const Field &field);
  ~Field();

  const Color &operator[](int index) const { return field_[index]; }

  Color &operator[](int index) { return field_[index]; }

  /// inlineなのでヘッダに書きます.
  /// rows_heightに加算
  inline void AddLowestEmptyRowsIndex(int i, int ct) {
    /*		if (i <= 0 || Field::COLUMN <= i) {
                            ASSERT_MESSAGE(false, "Field addrowsheihgt error : i
       = 0")
                    }
    */
    lowest_empty_rows_index_[i] += ct;
  }

  inline int GetLowestEmptyRowsIndex(int i) const {
    /*		if (i <= 0 || Field::COLUMN <= i) {
                            ASSERT_MESSAGE(false, "Field getrowsheihgt error : i
       = 0")
                    }
    */
    return lowest_empty_rows_index_[i];
  }

  inline int InitLowestEmptyRowsIndex() {
    std::fill_n(lowest_empty_rows_index_, COLUMN, 1);
  }

private:
  // 各列の空ぷよのrowindex. 初期値は1
  int lowest_empty_rows_index_[COLUMN];
  // ぷよぷよの6*13における盤面
  Color field_[FIELD_SIZE];
};

#endif // BPUYO_FIELD_H_
