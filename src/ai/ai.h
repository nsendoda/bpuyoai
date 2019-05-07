#ifndef BPUYOAI_AI_H_
#define BPUYOAI_AI_H_

// 各AIに継承されるクラス
// shared_ptrで使うこと(destructorがvirtualじゃないため）
class AI {
public:
	// 試合開始時に呼ばれる関数
	virtual void Init() {}
	// 操作可否に関わらず毎フレーム呼ばれる関数
	virtual void PreProcess() {}
	// 組みぷよが出現～設置完了する時まで、毎フレーム呼ばれる関数
	virtual void Main() {}

};

#endif