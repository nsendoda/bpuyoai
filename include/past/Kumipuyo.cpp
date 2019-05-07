#include "Kumipuyo.h"



Kumipuyo::Kumipuyo(int player):
	player_(player)
{}


Kumipuyo::~Kumipuyo()
{
}

/// @pre 予めプレイヤーがセットされている
void Kumipuyo::SetPositionAndRotate() {
	AI_NOWPOS* nowpos = ai_GetNowPos(player_);
	x = nowpos->x;
	y = nowpos->y;
	rotate = static_cast<RotateType>(nowpos->rot);
}

/// @pre 予めプレイヤーがセットされている
void Kumipuyo::SetNowColor() {
	AI_NOW* nowpuyo = ai_GetNow2(player_);
	parent = nowpuyo->a;
	child  = nowpuyo->b;
}

void Kumipuyo::SetNextColor() {
	AI_NOW* nextpuyo = ai_GetNext2(player_);
	parent = nextpuyo->a;
	child  = nextpuyo->b;
}





int Kumipuyo::PutAndCaluculateFrame(Field* field, const PutType& put)const{
	//ぷよを置く
	/// 親、子の列インデックス
	int parent_column_index = put.index;
	int child_column_index = put.index + (put.rotate == ROTATE_90 ? 1 : 0) + (put.rotate == ROTATE_270 ? -1 : 0);
	/// 親、子の行インデックス
	int parent_row_index = field->GetLowestEmptyRowsIndex(parent_column_index) + (put.rotate == ROTATE_180 ? 1 : 0);
	int child_row_index = field->GetLowestEmptyRowsIndex(child_column_index) + (put.rotate == ROTATE_0 ? 1 : 0);
	/// ぷよを置く
	(*field)[parent_row_index * Field::COLUMN + parent_column_index] = parent;
	(*field)[child_row_index  * Field::COLUMN + child_column_index ] = child;
    /// 置いた列の高さを＋１
	field->AddLowestEmptyRowsIndex(parent_column_index, 1);
	field->AddLowestEmptyRowsIndex(child_column_index, 1);

	// フレーム計算
	/// ちぎり段数
	int num_chigiri = child_column_index != parent_column_index ? std::abs(parent_row_index - child_row_index) : 0;
	/// 設置の実質段数（1段目は1,13段目は13）
	/// ちぎった時は高い方が設置段数、ちぎらない時は低い方が設置段数になる
	int num_row = num_chigiri > 0 ? max(parent_row_index, child_row_index) : min(parent_row_index, child_row_index);
	/// 設置にかかる時間を求める
	int drop_time = Parameter::kTimeChigiriRowDifference[num_chigiri];
	if (rotate == ROTATE_0) drop_time += Parameter::kTimeNoRotateDrop[num_row];
	else if (rotate == ROTATE_180) drop_time += Parameter::kTimeReverseDrop[num_row];
	else drop_time += Parameter::kTimeFlatDrop[num_row];
	return drop_time;
}


/// 引数のフィールドの13,14段目に空中浮遊させる.
/// 置けるかどうかの判定は行わない.
/// childはparentに隣接するように置かれる. その後ぷよを落下することにより設置している.
/// 回転が90,270時は全て14段目に置く.
/// 0,180時は14段目と13段目に分けて置く.
/// @pre CanPutで置けるかどうかの判定をfieldに行っている
void Kumipuyo::FloatInField(Field* field)const{
	int childFileIndex, parentFileIndex;
	int childRowIndex  = Field::ROW - 1;
	int parentRowIndex = Field::ROW - 1;

	childFileIndex = parentFileIndex = desirable_put.index;

	switch (desirable_put.rotate) {
		case ROTATE_0:   { 
			// 親が13段目
			parentRowIndex--;
			break; 
		}
		case ROTATE_90:  { 
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
	(*field)[parentRowIndex * Field::COLUMN + parentFileIndex] = parent;
	(*field)[childRowIndex * Field::COLUMN + childFileIndex] = child;
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
int Kumipuyo::FallKumipuyo(Field* field_,int parentFileIndex, int rotate)const{
	Field& field = *field_;
	int childFileIndex = parentFileIndex;/**/
	int parentRowIndex = 0, childRowIndex = 0;
	if (rotate == RotateType::ROTATE_90)++childFileIndex;
	else if (rotate == RotateType::ROTATE_270) --childFileIndex;
	else if (rotate == RotateType::ROTATE_0) ++parentRowIndex;
	else if (rotate == RotateType::ROTATE_180) ++childRowIndex;
	/* 親、子が置く場所 */
	int parentDropIndex = (field.GetLowestEmptyRowsIndex(parentFileIndex) - childRowIndex) * Field::COLUMN + parentFileIndex;
	int childDropIndex = (field.GetLowestEmptyRowsIndex(childFileIndex) - parentRowIndex) * Field::COLUMN + childFileIndex;
	/* 組みぷよが置いてある場所*/
	int parentPlacedIndex =  parentRowIndex*Field::COLUMN + parentFileIndex;
	int childPlacedIndex =  childRowIndex*Field::COLUMN + childFileIndex;
	/* ぷよを置く */
	field[parentDropIndex] = field[parentPlacedIndex];
	field[childDropIndex] = field[childPlacedIndex];
	/* 
	 * 13,14のぷよを消す
	 * 12段目が置かれている状態で、13段目に縦置き（0,180）、又は11段目が置かれている状態で、12段目に縦置き（0,180）
	 * を考えた時は被りが発生するのでその部分は消してはいけない.
	 */
	if (parentDropIndex == parentPlacedIndex && childDropIndex == childPlacedIndex) {/* 13段目に縦置きの時は14段目のぷよが一個消えるので辻褄を合わせる. */
		field.AddLowestEmptyRowsIndex(parentFileIndex, 1);
	}
	else if (rotate == ROTATE_0 && parentPlacedIndex == childDropIndex) {/* 12段目に無回転で設置時 */
		field[childPlacedIndex] = Parameter::EMPTY;
	}
	else if (rotate == ROTATE_180 && childPlacedIndex == parentDropIndex) {/* 12段目に180度回転で設置時*/
		field[parentPlacedIndex] = Parameter::EMPTY;
	}
	else {
		field[childPlacedIndex] = Parameter::EMPTY;
		field[parentPlacedIndex] = Parameter::EMPTY;
	}
	return 0;
}

