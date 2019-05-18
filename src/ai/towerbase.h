#ifndef BPUYOAI_TOWERBASE_H_
#define BPUYOAI_TOWERBASE_H_

#include "../types.h"

#include <vector>

// タワーの土台情報を保存するクラス

struct TowerBase {

	static const FieldIndex NOTEXIST = -1;

	Color color;

	// 補完を行う列
	std::vector<Column> complement_column;

	// 土台のIndex
	std::vector<FieldIndex> base;

	// 発火点に出来ないINDEX
	FieldIndex cannot_fire_index;

	// @args i, j..タワー土台の縦2連結部分。順番はどうでもよい
	// @args c..縦2連結部分のColumn
	// @args ordered..順タワーならtrue, 逆タワーならfalse
	TowerBase(Color puyo, int i, int j,FieldIndex fi, Column c, bool ordered) : cannot_fire_index(fi) {
		SetBase(puyo, i, j, c, ordered);
	}

	// リザーブ列（順タワーにおける3列目）が必ず[0]に来るようにSet
// @args r..リザーブ列
// @args i, j..タワー土台の縦2連結部分。順番はどうでもよい
// @args c..縦2連結部分のColumn
// @args ordered..順タワーならtrue, 逆タワーならfalse
	TowerBase(Color puyo, int r, int i, int j,FieldIndex fi, Column c, bool ordered) : cannot_fire_index(fi) {
		SetBase(puyo, r, i, j, c, ordered);
	}

	inline int GetDirect() const { return direct_incidental_index; }
private:
	// 順タワー方向なら3列目→4列目→5列目となる。
// その場合の値は1
// 逆タワーは-1
// 主に補完の時のINDEX合わせに使う。
	int direct_incidental_index;
	// リザーブ列がない場合、[0]はNOTEXISTにSet
	void SetBase(Color puyo, int i, int j, Column c, bool ordered) {
		if (ordered) SetOrderedTower();
		else SetReverseTower();
		color = puyo;
		base = { NOTEXIST, i, j };
		complement_column = { NOTEXIST, c, c + direct_incidental_index };
	}

	void SetBase(Color puyo, int r, int i, int j, Column c, bool ordered) {
		if (ordered) SetOrderedTower();
		else SetReverseTower();
		base = { r, i, j };
		color = puyo;
		complement_column = { c - direct_incidental_index, c, c + direct_incidental_index };
	}

	void SetOrderedTower() { direct_incidental_index = 1; }
	void SetReverseTower() { direct_incidental_index = -1; }
};
#endif