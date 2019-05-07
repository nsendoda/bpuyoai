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
  static Chain Simulate(Field *field_, int parent_index, int child_index,
                        int chain_num = 0);
  static int GetDropRowAndFallAll(Field *field_);

  /// ぷよを引数のfieldに置く
  /// @ret 置くのにかかったフレームを返す
  static int PutAndCaluculateFrame(const Kumipuyo &kumipuyo, Field *field,
                                   const PutType &put);

  /// 指定した位置にぷよを置く
  static inline int PutOnePuyoAndGetIndex(Field *field, int i, Color c) {
    // ASSERT_MESSAGE(i != 0, "Simulator put error : i = 0")
    int index = field->GetLowestEmptyRowsIndex(i) * Field::COLUMN + i;
    (*field)[index] = c;
    field->AddLowestEmptyRowsIndex(i, 1);
    return index;
  }

  static inline void EliminateOnePuyo(Field *field, int i) {
    // ASSERT_MESSAGE(i != 0, "Simulator eliminate error : i = 0")
    field->AddLowestEmptyRowsIndex(i, -1);
    (*field)[field->GetLowestEmptyRowsIndex(i) * Field::COLUMN + i] =
        Color::EMPTY;
  }

private:
  static void DeleteLink(Field *field_, int i, int chain_num, bool linked[]);
  static int CaluculateLinkCount(const Field &field, int index, bool *linked);
  /// 14段目と14段目にぷよを浮かせる
  static void FloatKumipuyo(const Kumipuyo &kumipuyo, Field *field);
  /// 組みぷよを指定した方法で落とす
  static int FallKumipuyo(const Kumipuyo &kumipuyo, Field *field_,
                          int parentFileIndex, int rotate);
};

#endif
