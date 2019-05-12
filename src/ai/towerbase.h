#ifndef BPUYOAI_TOWERBASE_H_
#define BPUYOAI_TOWERBASE_H_

#include "../types.h"

#include <vector>

// �^���[�̓y�����ۑ�����N���X

struct TowerBase {

	static const FieldIndex NOTEXIST = -1;

	// �⊮���s����
	std::vector<Column> complement_column;

	// �y���Index
	std::vector<FieldIndex> base;

	// @args i, j..�^���[�y��̏c2�A�������B���Ԃ͂ǂ��ł��悢
	// @args c..�c2�A��������Column
	// @args ordered..���^���[�Ȃ�true, �t�^���[�Ȃ�false
	TowerBase(int i, int j, Column c, bool ordered) {
		SetBase(i, j, c, ordered);
	}

	// ���U�[�u��i���^���[�ɂ�����3��ځj���K��[0]�ɗ���悤��Set
// @args r..���U�[�u��
// @args i, j..�^���[�y��̏c2�A�������B���Ԃ͂ǂ��ł��悢
// @args c..�c2�A��������Column
// @args ordered..���^���[�Ȃ�true, �t�^���[�Ȃ�false
	TowerBase(int r, int i, int j, Column c, bool ordered) {
		SetBase(r, i, j, c, ordered);
	}

	inline int GetDirect() const { return direct_incidental_index; }
private:
	// ���^���[�����Ȃ�3��ځ�4��ځ�5��ڂƂȂ�B
// ���̏ꍇ�̒l��1
// �t�^���[��-1
// ��ɕ⊮�̎���INDEX���킹�Ɏg���B
	int direct_incidental_index;
	// ���U�[�u�񂪂Ȃ��ꍇ�A[0]��NOTEXIST��Set
	void SetBase(int i, int j, Column c, bool ordered) {
		if (ordered) SetOrderedTower();
		else SetReverseTower();
		base = { NOTEXIST, i, j };
		complement_column = { NOTEXIST, c, c + direct_incidental_index };
	}

	void SetBase(int r, int i, int j, Column c, bool ordered) {
		if (ordered) SetOrderedTower();
		else SetReverseTower();
		base = { r, i, j };
		complement_column = { c - direct_incidental_index, c, c + direct_incidental_index };
	}

	void SetOrderedTower() { direct_incidental_index = 1; }
	void SetReverseTower() { direct_incidental_index = -1; }
};
#endif