#include "simulator.h"

/// よって, 連結数4以上のぷよが一つもない時には, score,
/// chainNum,time_chainともに0となる.
/// 発見した連鎖数が０の時は組みぷよ周りだけ、
/// 　　　　　　　　１以上の時は全部を見ていく
/// @note chain_numはデフォルト引数0. 1だと既に1連鎖が行われた状態で始まる
/// @pre 全てのぷよが落下済み
Chain Simulator::Simulate(Field *field_, int parent_index, int child_index,
                          int chain_num) {
  Field &field = *field_;
  int score = 0;
  int chain_frame = 0;
  while (true) {
    bool isDisappeared = false;

    bool color[Color::WALL] = {
        false}; /* 連結していたぷよの色のindexにtrueが格納される配列 */
    int colorNum = 0;      /* 連結していたぷよの色の種類 */
    int disappeardNum = 0; /* この1連鎖で消えたぷよの量の和 */
    int bonus_chain, bonus_link, bonus_color; /* 各ボーナスの値が格納される */

    bonus_link = 0; /* 連結ボーナス
                     * 4つ以上連結しているぷよを探す.
                     * 見つけたら, そのぷよたちを消す.
                     */
    for (int i, ct = 0;;) {
      if (chain_num == 0) {
        if (ct == 0)
          i = parent_index;
        else if (ct == 1)
          i = child_index;
        else
          break;
        ct++;
        if (i < Field::FIELD_START || i > Field::FIELD_END) {
          continue;
        }
      } else {
        if (ct == 0) {
          ct++;
          i = Field::FIELD_START;
        } else {
          i++;
          if (i > Field::FIELD_END)
            break;
        }
      }
      bool linked[Field::FIELD_SIZE] = {
          false}; /* 連結している場所にtrueが格納される配列 */
      /*  連鎖数が1以上で, 下か左が同じ色のぷよの場合,
       * 連結数を再探索する必要はない */
      if (chain_num > 0 &&
          (field[i] == field[i - Field::COLUMN] || field[i] == field[i - 1])) {
        continue;
      }

      if (field[i] == Color::EMPTY || field[i] == Color::OJAMA) {
        continue;
      }

      /* 連結数を取得する */
      int linkNum = CaluculateLinkCount(field, i, linked);
      /* 4つ以上連結していたら */
      if (linkNum >= 4) {
        isDisappeared = true;
        disappeardNum += linkNum;
        bonus_link += kBonusLink[linkNum];
        /* 連結しているぷよの色を保存 */
        if (!color[field[i]]) {
          color[field[i]] = true;
          colorNum++;
        }
        /* 連結しているぷよを消す */
        /// @todo 関数に書き出す deletelink
        DeleteLink(&field, i, chain_num, linked);
      }
    }
    if (isDisappeared) {
      /* ぷよが消えているので, ぷよたちを落下させる. 同時に連鎖時間も取得. */
      chain_frame += kTimeOneChainAndDrop[GetDropRowAndFallAll(field_)];
      /* 連鎖数を増やす */
      chain_num++;
      /* 得点の計算 */
      bonus_chain = kBonusChain[chain_num];
      bonus_color = kBonusColor[colorNum];
      /* ボーナスの和が0なら, ボーナス全体を1として計算するため,
       * ここでは連鎖ボーナスを1として代える. */
      if (bonus_chain + bonus_link + bonus_color == 0) {
        bonus_chain = 1;
      }
      score += 10 * disappeardNum * (bonus_chain + bonus_link + bonus_color);
    } else {
      break;
    }
  }
  // scoreがfatal_doseを超えてる場合矯正
  if (score > UPPER_FATAL_DOSE)
    score = UPPER_FATAL_DOSE;
  return Chain(chain_num, score, chain_frame);
}

/*
 * 浮いているぷよをすべて落下させる関数.
 * 落下段数を返す.
 * 連鎖シュミレート時のぷよの落下にのみ使う.
 */
int Simulator::GetDropRowAndFallAll(Field *field_) {
  Field &field = *field_;
  int num_max_drop_row = 0;
  /// 右の列から各ぷよの落ちる段数を計算する
  for (int x = Field::FIELD_START; x < Field::FIELD_START + Field::COLUMN - 1;
       ++x) {
    bool is_local_fall = false;
    int f[Field::ROW] = {0};
    int emptyCount = 0;
    int num_drop_row = 0;
    for (int y = 0; y < Field::ROW - 2; y++) {
      if (field[x + Field::COLUMN * y] == Color::EMPTY) {
        emptyCount++;
      } else {
        f[y] = emptyCount;
        if (emptyCount > 0) {
          num_drop_row = emptyCount;
          is_local_fall = true;
        }
      }
    }
    /// 実際にぷよを落下させる
    if (is_local_fall) {
      for (int y = 0; y < Field::ROW - 2; y++) {
        if (f[y] > 0) {
          field[x + Field::COLUMN * (y - f[y])] = field[x + Field::COLUMN * y];
          field[x + Field::COLUMN * y] = Color::EMPTY;
        }
      }
      field.AddLowestEmptyRowsIndex(x % Field::COLUMN, -num_drop_row);
      num_max_drop_row = std::max(num_drop_row, num_max_drop_row);
    }
  }
  return num_max_drop_row;
}

/// 組ぷよを引数のfieldに置く
/// @ret 置くのにかかったフレームを返す
int Simulator::PutAndCaluculateFrame(const Kumipuyo &kumipuyo, Field *field,
                                     const PutType &put) {
  //ぷよを置く
  /// 親、子の列インデックス
  int parent_column_index = put.index;
  int child_column_index = put.index + (put.rotate == ROTATE_90 ? 1 : 0) +
                           (put.rotate == ROTATE_270 ? -1 : 0);
  /// 親、子の行インデックス
  int parent_row_index = field->GetLowestEmptyRowsIndex(parent_column_index) +
                         (put.rotate == ROTATE_180 ? 1 : 0);
  int child_row_index = field->GetLowestEmptyRowsIndex(child_column_index) +
                        (put.rotate == ROTATE_0 ? 1 : 0);
  /// ぷよを置く
  (*field)[parent_row_index * Field::COLUMN + parent_column_index] =
      kumipuyo.parent;
  (*field)[child_row_index * Field::COLUMN + child_column_index] =
      kumipuyo.child;
  /// 置いた列の高さを＋１
  field->AddLowestEmptyRowsIndex(parent_column_index, 1);
  field->AddLowestEmptyRowsIndex(child_column_index, 1);

  /// フレーム計算
  /// ちぎり段数
  int num_chigiri = child_column_index != parent_column_index
                        ? std::abs(parent_row_index - child_row_index)
                        : 0;
  /// 設置の実質段数（1段目は1, 13段目は13）
  /// ちぎった時は高い方が設置段数、ちぎらない時は低い方が設置段数になる
  int num_row = num_chigiri > 0 ? std::max(parent_row_index, child_row_index)
                                : std::min(parent_row_index, child_row_index);
  /// 設置にかかる時間を求める
  int drop_time = kTimeChigiriRowDifference[num_chigiri];
  if (kumipuyo.rotate == ROTATE_0)
    drop_time += kTimeNoRotateDrop[num_row];
  else if (kumipuyo.rotate == ROTATE_180)
    drop_time += kTimeReverseDrop[num_row];
  else
    drop_time += kTimeFlatDrop[num_row];
  return drop_time;
}

/* --------------below private------------------- */

void Simulator::DeleteLink(Field *field_, int i, int chain_num, bool linked[]) {
  Field &field = *field_;
  if (chain_num == 0) {
    std::queue<int> que;
    que.push(i);
    bool used[Field::FIELD_SIZE];
    std::fill_n(used, Field::FIELD_SIZE, false);
    while (!que.empty()) {
      int j = que.front();
      que.pop();
      field[j] = Color::EMPTY;
      used[j] = true;
      for (int d : {-Field::COLUMN, +1, +Field::COLUMN, -1}) {
        if (used[j + d] || !linked[j + d])
          continue;
        que.push(j + d);
      }
    }
  } else {
    for (int j = i; j <= Field::FIELD_END; j++) {
      if (linked[j]) {
        field[j] = Color::EMPTY;
        /* お邪魔ぷよを消す */
        int d[4] = {-Field::COLUMN, +1, +Field::COLUMN, -1};
        for (int k = 0; k < 4; k++) {
          if (field[j + d[k]] == Color::OJAMA) {
            field[j + d[k]] = Color::EMPTY;
          }
        }
      }
    }
  }
}

/* 連結数を計算する関数.*linkedに,
 * 連結しているぷよの場所にtrueが立った配列が代入される.*返り値は連結数.
 * */
int Simulator::CaluculateLinkCount(const Field &field, int index,
                                   bool *linked) {
  // indexにぷよがない、又は13段目にいるなら0を返す.
  if (field[index] == Color::EMPTY || field[index] == Color::WALL ||
      field[index] == Color::OJAMA || index >= 13 * Field::COLUMN) {

    return 0;
  }
  linked[index] = true;
  int link_ct = 1;
  static const int dx[4] = {-Field::COLUMN, +1, +Field::COLUMN, -1};
  for (int d = 0; d < 4; ++d) {
    if (field[index] == field[index + dx[d]]) {
      if (linked[index + dx[d]]) {
        continue;
      }
      link_ct += CaluculateLinkCount(field, index + dx[d], linked);
    }
  }
  return link_ct;
}

/// 引数のフィールドの13,14段目に空中浮遊させる.
/// 置けるかどうかの判定は行わない.
/// childはparentに隣接するように置かれる.
/// その後ぷよを落下することにより設置している.
/// 回転が90,270時は全て14段目に置く.
/// 0,180時は14段目と13段目に分けて置く.
/// @pre CanPutで置けるかどうかの判定をfieldに行っている
void Simulator::FloatKumipuyo(const Kumipuyo &kumipuyo, Field *field) {
  int childFileIndex, parentFileIndex;
  int childRowIndex = Field::ROW - 1;
  int parentRowIndex = Field::ROW - 1;

  childFileIndex = parentFileIndex = kumipuyo.desirable_put.index;

  switch (kumipuyo.desirable_put.rotate) {
  case ROTATE_0: { // 親が13段目
    parentRowIndex--;
    break;
  }
  case ROTATE_90: {
    childFileIndex++;
    break;
  }
  case ROTATE_180: {
    // 子が13段目
    childRowIndex--;
    break;
  }
  case ROTATE_270: {
    childFileIndex--;
    break;
  }
  }
  (*field)[parentRowIndex * Field::COLUMN + parentFileIndex] = kumipuyo.parent;
  (*field)[childRowIndex * Field::COLUMN + childFileIndex] = kumipuyo.child;
}

/*
 * 浮いている組みぷよを落下させる関数.
 * 組ぷよはそれぞれMove::putにより、
 * 回転が90,270時は全て14段目,0,180時は14段目と13段目に分けて置かれている.
 * その情報とcan_drop_rowを用いてO(1)で落とす.
 * 置くのにかかった時間（設置までの時間＋ちぎり時間）を返す
 * ちぎりが発生しない場合は段数は0.
 * 引数はフィールド, 親ぷよを列インデックス, 回転
 */
int Simulator::FallKumipuyo(const Kumipuyo &kumipuyo, Field *field_,
                            int parentFileIndex, int rotate) {
  Field &field = *field_;
  int childFileIndex = parentFileIndex; /**/
  int parentRowIndex = 0, childRowIndex = 0;
  if (rotate == RotateType::ROTATE_90)
    ++childFileIndex;
  else if (rotate == RotateType::ROTATE_270)
    --childFileIndex;
  else if (rotate == RotateType::ROTATE_0)
    ++parentRowIndex;
  else if (rotate == RotateType::ROTATE_180)
    ++childRowIndex;
  /* 親、子が置く場所 */
  int parentDropIndex =
      (field.GetLowestEmptyRowsIndex(parentFileIndex) - childRowIndex) *
          Field::COLUMN +
      parentFileIndex;
  int childDropIndex =
      (field.GetLowestEmptyRowsIndex(childFileIndex) - parentRowIndex) *
          Field::COLUMN +
      childFileIndex;
  /* 組みぷよが置いてある場所*/
  int parentPlacedIndex = parentRowIndex * Field::COLUMN + parentFileIndex;
  int childPlacedIndex = childRowIndex * Field::COLUMN + childFileIndex;
  /* ぷよを置く */
  field[parentDropIndex] = field[parentPlacedIndex];
  field[childDropIndex] = field[childPlacedIndex];
  /*
   * 13,14のぷよを消す
   * 12段目が置かれている状態で、13段目に縦置き（0,180）、又は11段目が置かれている状態で、12段目に縦置き（0,180）
   * を考えた時は被りが発生するのでその部分は消してはいけない.
   */
  if (parentDropIndex == parentPlacedIndex &&
      childDropIndex ==
          childPlacedIndex) { /* 13段目に縦置きの時は14段目のぷよが一個消えるので辻褄を合わせる.
                                 */
    field.AddLowestEmptyRowsIndex(parentFileIndex, 1);
  } else if (rotate == ROTATE_0 &&
             parentPlacedIndex == childDropIndex) { /* 12段目に無回転で設置時 */
    field[childPlacedIndex] = Color::EMPTY;
  } else if (rotate == ROTATE_180 &&
             childPlacedIndex ==
                 parentDropIndex) { /* 12段目に180度回転で設置時*/
    field[parentPlacedIndex] = Color::EMPTY;
  } else {
    field[childPlacedIndex] = Color::EMPTY;
    field[parentPlacedIndex] = Color::EMPTY;
  }
  return 0;
}
