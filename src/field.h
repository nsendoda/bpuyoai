#ifndef BPUYOAI_FIELD_H_
#define BPUYOAI_FIELD_H_

#include <algorithm>
#include <cstring>
#include <map>
#include <queue>

#include "debug.h"
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
   *  0  WWWWWWW
   *
   *  W は壁を表し, 番兵の役割をする.
   * 配列でのインデックスは次のようになっている.
   * 14  98 99 100 101 102 103 104
   * 13  91 92 93  94  95  96  97
   * 12  84 85 86  87  88  89  90
   *  .
   *  .
   *  1  789....
   *  0  0123456
   *
   *  8方向は次で表せる.
   *   +6     +7     +8
   *   -1  Position
   *   -6    -7      -8
   */
public:
  static const int COLUMN = 7;
  static const int ROW = 15;
  static const int FIELD_SIZE = COLUMN * ROW; // 105
  static const int FIELD_START = 8;           // 1段目の1列目
  static const int FIELD_END = 97;            // 13段目の6列目
  static const int FIELD_DEATH = 87;

  static const int PUYO_APPEAR_COLUMN = 3;
	static const int VISIBLE_COLUMN = 6;
	static const int VISIBLE_ROW = 12;

	static constexpr int LOWEST_EMPTY_ROWS_MAX = 14; // 13段目までぷよが埋まってる
	static constexpr int LOWEST_EMPTY_ROWS_MIN = 1;  // 1段目に何もぷよがない

  Field();
  Field(const Field &field);
  ~Field();

	const Color &operator[](int index) const { return field_[index]; }

	Color &operator[](int index) { return field_[index]; }

  void Output() const;
  void OutputIncludeWall() const;
  bool Equals(const Field &target);

	// 13段目までの空いているマスの数を数える
	int CountEmptyPuyos() const;

	// 12列目までで3列目12段から到達できる空いているマスの数を数える
	int CountMawashiEmptyPuyos() const;

  void ModifyLowestEmptyRows();

  /// inlineなのでヘッダに書きます.
  /// rows_heightに加算
  /// [1, 13]に沿わない値になる時は最大又は最小値で収める
  void AddLowestEmptyRows(Column column, int ct);

	// ある列で空いているマスの内最も低いマスの高さを返す.
	// [1, 13]の範囲
  /// @TODO 0アクセスの時例外を投げる
  int GetLowestEmptyRows(int column) const;

	// ある列の一番低い空いているマスのfieldにおけるindexを返す
	inline FieldIndex GetEmptyIndex(Column column) const {
		return GetLowestEmptyRows(column) * COLUMN + column;
	}

  inline void InitLowestEmptyRows() {
    std::fill_n(lowest_empty_rows_, COLUMN, 1);
  }

private:
  // 各列の空ぷよのrowindex. 初期値は1
  // range = [1, 14]
	// 14の時はもう置けない
  int lowest_empty_rows_[COLUMN];
  // ぷよぷよの6*13における盤面
  // 0は使わない, [1-6]
  Color field_[FIELD_SIZE];
};

#endif // BPUYO_FIELD_H_
