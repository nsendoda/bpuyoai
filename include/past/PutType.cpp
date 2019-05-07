#include "PutType.h"


PutType::PutType()
{
}

PutType::PutType(int index, RotateType rotate) :
	index(index),
	rotate(rotate)
{}

PutType	PutType::GetPattern(int index) {
	return PutType(1 + (index + 1) / Parameter::ROTATE_NUMBER, Parameter::ALL_INDEXED_ROTATE_TYPE[index]);
}

void PutType::SetPattern(int index) {
	this->index = 1 + (index + 1) / Parameter::ROTATE_NUMBER;
	rotate = Parameter::ALL_INDEXED_ROTATE_TYPE[index];
}

/// PutType通りに置いた時に、親ぷよと子ぷよがFieldクラスのどのインデックスに置かれるかを計算してpairで返す.
/// firstはparent, secondはchildを返す.
std::pair<int, int> PutType::CaluculatePutIndex(const Field& field)const {
	/// 親、子の列インデックス
	int parent_column_index = index;
	int child_column_index = index + (rotate == ROTATE_90 ? 1 : 0) + (rotate == ROTATE_270 ? -1 : 0);
	/// 親、子の行インデックス
	int parent_row_index = field.GetLowestEmptyRowsIndex(parent_column_index) + (rotate == ROTATE_180 ? 1 : 0);
	int child_row_index = field.GetLowestEmptyRowsIndex(child_column_index) + (rotate == ROTATE_0 ? 1 : 0);
	return std::make_pair(parent_row_index * Field::COLUMN + parent_column_index, child_row_index * Field::COLUMN + child_column_index);
}

/// fieldにdesirable_putでぷよを置けるか調べる関数.
/// @pre 色、場所、回転がセットされている
bool PutType::CanPut(const Field& field)const{
  /* 初めに合法手かどうか確かめる. */
  if (index < 1 || index > 6) {
    return false;
  }
  /* parentを置く列の12段目にすでにぷよがある場合は置けない */
  if (field[12 * Field::COLUMN + index] != Parameter::EMPTY) {
    return false;
  }

  /// 13段目には置けない
  if (field.GetLowestEmptyRowsIndex(index) >= Field::ROW - 2)return false;
  /* 回しはできないという仮定のもとで, 置くのに回しが必要なものに注意する. */
  /* なお, 11段目まで積みあがっているときの, 縦置き, 逆置きはできるものとする. */ 
  if (index == 1 && field[12 * Field::COLUMN + 1] != Parameter::EMPTY) { /* 1列目に置こうとするときに, 1列目に積みあがっている. */
    return false;
  }
  if (index == 1 && field[12 * Field::COLUMN + 2] != Parameter::EMPTY) { /* 1列目に置こうとするときに, 2列目に積みあがっている. */
      return false;
  }
  if (index == 2 && field[12 * Field::COLUMN + 2] != Parameter::EMPTY) { 
    return false;
  }
  if (index == 4 && field[12 * Field::COLUMN + 4] != Parameter::EMPTY) {
    return false;
  }
  if (index == 5 && field[12 * Field::COLUMN + 5] != Parameter::EMPTY) {
    return false;
  }
  if (index == 5 && field[12 * Field::COLUMN + 4] != Parameter::EMPTY) {
    return false;
  }
  if (index == 6 && field[12 * Field::COLUMN + 6] != Parameter::EMPTY) {
    return false;
  }
  if (index == 6 && (field[12 * Field::COLUMN + 4] != Parameter::EMPTY || field[12 * Field::COLUMN + 5] != Parameter::EMPTY)) {
    return false;
  }
  if (index == 2 && rotate == ROTATE_270 && field[12 * Field::COLUMN + 1] != Parameter::EMPTY) {
    return false;
  }
  if (index == 3 && rotate == ROTATE_90  && field[12 * Field::COLUMN + 4] != Parameter::EMPTY) {
    return false;
  }
  if (index == 3 && rotate == ROTATE_270 && field[12 * Field::COLUMN + 2] != Parameter::EMPTY) {
    return false;
  }
  if (index == 4 && rotate == ROTATE_90  && field[12 * Field::COLUMN + 5] != Parameter::EMPTY) {
    return false;
  }
  if (index == 5 && rotate == ROTATE_90  && field[12 * Field::COLUMN + 6] != Parameter::EMPTY) {
    return false;
  }
	return true;
}


PutType::~PutType()
{
}
