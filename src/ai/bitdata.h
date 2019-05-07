#ifndef BPUYOAI_BITDATA_AI_
#define BPUYOAI_BITDATA_AI_

struct BitData {
	__int64 f1, f2, f3, f4, now, next;
	BitData(__int64 f1_, __int64 f2_, __int64 f3_, __int64 f4_, __int64 now_, __int64 next_)
		: f1(f1_), f2(f2_), f3(f3_), f4(f4_), now(now_), next(next_) {}

};

struct CompareBitDataOnlyNow {
	bool operator() (const BitData& a, const BitData& b) const {
		if (a.f1 != b.f1) return a.f1 < b.f1;
		if (a.f2 != b.f2) return a.f2 < b.f2;
		if (a.f3 != b.f3) return a.f3 < b.f3;
		if (a.f4 != b.f4) return a.f4 < b.f4;
		return a.now < b.now;
	}
};

struct CompareBitData {
	bool operator() (const BitData& a, const BitData& b) const {
		if (a.f1 != b.f1) return a.f1 < b.f1;
		if (a.f2 != b.f2) return a.f2 < b.f2;
		if (a.f3 != b.f3) return a.f3 < b.f3;
		if (a.f4 != b.f4) return a.f4 < b.f4;

		if (a.now != b.now) return a.now < b.now;
		return a.next < b.next;
	}
};

#endif