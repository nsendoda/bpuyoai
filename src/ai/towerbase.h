#ifndef BPUYOAI_TOWERBASE_H_
#define BPUYOAI_TOWERBASE_H_

#include "../types.h"

#include <vector>

// �^���[�̓y�����ۑ�����N���X

struct TowerBase {

	static const FieldIndex NOTEXIST = -1;

	Color color;

	// �⊮���s����
	std::vector<Column> complement_column;

	// �y���Index
	std::vector<FieldIndex> base;

	// ���Γ_�ɏo���Ȃ�INDEX
	FieldIndex cannot_fire_index;

	// @args i, j..�^���[�y��̏c2�A�������B���Ԃ͂ǂ��ł��悢
	// @args c..�c2�A��������Column
	// @args ordered..���^���[�Ȃ�true, �t�^���[�Ȃ�false
	TowerBase(Color puyo, int i, int j,FieldIndex fi, Column c, bool ordered) : cannot_fire_index(fi) {
		SetBase(puyo, i, j, c, ordered);
	}

	// ���U�[�u��i���^���[�ɂ�����3��ځj���K��[0]�ɗ���悤��Set
// @args r..���U�[�u��
// @args i, j..�^���[�y��̏c2�A�������B���Ԃ͂ǂ��ł��悢
// @args c..�c2�A��������Column
// @args ordered..���^���[�Ȃ�true, �t�^���[�Ȃ�false
	TowerBase(Color puyo, int r, int i, int j,FieldIndex fi, Column c, bool ordered) : cannot_fire_index(fi) {
		SetBase(puyo, r, i, j, c, ordered);
	}

	inline int GetDirect() const { return direct_incidental_index; }
private:
	// ���^���[�����Ȃ�3��ځ�4��ځ�5��ڂƂȂ�B
// ���̏ꍇ�̒l��1
// �t�^���[��-1
// ��ɕ⊮�̎���INDEX���킹�Ɏg���B
	int direct_incidental_index;
	// ���U�[�u�񂪂Ȃ��ꍇ�A[0]��NOTEXIST��Set
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