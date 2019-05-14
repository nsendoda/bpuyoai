#ifndef BPUYOAI_NOMITHINK_H_
#define BPUYOAI_NOMITHINK_H_

#include "../state.h"
#include "../field.h"
#include "../simulator.h"
#include "../types.h"

#include "towerrate.h"
#include "towerbase.h"
#include "nomimemory.h"

#include <vector>
#include <queue>
#include <functional>


class NomiThink {
public:
	static std::vector<PutType> ConstantPut(const State & state_, const Kumipuyo & pre);
	static Score CalculateFatalDose(const State & state_);
	static bool KillThink(const State & state, Score fatal_dose, FieldIndex * fi);
	static PutType NomiThink::Think(const State& state, Score fatal_dose);
private:

	static std::vector<PutType> FirstConstants(const std::string & p);


	static bool CanFireTower(const Field & f, FieldIndex i, FieldIndex pre);

	static TowerRate Waruagaki(const Field& second_field, Score fatal_dose, PutIndex pi, Frame puts_frame);

	static TowerRate RateTower(const Field & f_, const TowerBase & t_base, Score fatal_dose, PutIndex pi, Frame puts_frame);

	static Chain ActualChain(const Field & f_, const TowerBase& t_base);

	static TowerRate VirtualChain(const Field & f_, const TowerBase & t_base, Score fatal_dose);

	static TowerBase BaseDecide(const Field & f);

	static void BaseDelete(Field * f, const TowerBase & t_base);

	static int LinkCount(const Field & field, FieldIndex i);

	static int ComplementTower3And2(Field * f, bool* used, const TowerBase & t_base);

	static int ComplementTower3(Field * f, bool * used, const TowerBase & t_base);

	static bool Complement3Connection(Field * f, bool * used, FieldIndex i, FieldIndex j, FieldIndex k, int c_i, int shape_i, int direct, bool base_conflict);

	static bool Complement2Connection(Field * f, bool * used, FieldIndex base, FieldIndex j, int c_i, int shape_i, int direct, bool base_conflict);

	static const int dt[4];
};


#endif