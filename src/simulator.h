#ifndef BPUYOAI_SIMULATOR_H_
#define BPUYOAI_SIMULATOR_H_
#include <queue>

#include "chain.h"
#include "field.h"
#include "kumipuyo.h"
#include "puttype.h"
#include "types.h"

class Simulator {
public:
  // 消えなくなるぷよが出来るまで連鎖を行う
  // 初めはぷよを落とす処理を呼ばないことに注意
  static Chain Simulate(Field *field_, int parent_index = -1,
                        int child_index = -1, int chain_num = 0);

  // 浮いているぷよを全て落とす
  // @ret 最大落下段数
  static Row FallAll(Field *field_);

  /// ぷよを引数のfieldに置く
  /// @ret 置くのにかかったフレームを返す
  static Frame Put(const Kumipuyo &kumipuyo, Field *field, const PutType &put);

  /// 指定した位置にぷよを置く
  static inline FieldIndex PutOnePuyo(Field *field, Column column, Color c) {
    // ASSERT_MESSAGE(i != 0, "Simulator put error : i = 0")
    Row empty_row = field->GetLowestEmptyRows(column);
    FieldIndex index = empty_row * Field::COLUMN + column;
    // 14段目以上に置かれたぷよは消滅する
    if (empty_row < Field::ROW - 1) {
      (*field)[index] = c;
      field->AddLowestEmptyRows(column, 1);
    }
    return index;
  }

  /// 指定した列のぷよの一番上のぷよを消す
  static inline void EliminateOnePuyo(Field *field, Column i) {
    // ASSERT_MESSAGE(i != 0, "Simulator eliminate error : i = 0")
    field->AddLowestEmptyRows(i, -1);
    (*field)[field->GetLowestEmptyRows(i) * Field::COLUMN + i] = Color::EMPTY;
  }

  // parent, childに代入
  static std::pair<FieldIndex, FieldIndex>
  CalculatePutIndex(const PutType &puttype, const Field &field);

  // フィールドにこの置き方で置けるかどうか
  static bool CanPut(const PutType &puttype, const Field &field);

	static bool ValidPosition(const Kumipuyo &kumipuyo, const Field &field);

private:
  static void DeleteLink(Field *field_, int i, int chain_num, bool linked[]);
  static int CalculateLinkCount(const Field &field, int index, bool *linked);
  /// 14段目と14段目にぷよを浮かせる
  static void FloatKumipuyo(const Kumipuyo &kumipuyo, Field *field);
  /// 組みぷよを指定した方法で落とす
  static int FallKumipuyo(const Kumipuyo &kumipuyo, Field *field_,
                          int parentFileIndex, int rotate);
};

#endif
