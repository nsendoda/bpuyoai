#ifndef BPUYOAI_AI_H_
#define BPUYOAI_AI_H_

// �eAI�Ɍp�������N���X
// shared_ptr�Ŏg������(destructor��virtual����Ȃ����߁j
class AI {
public:
	// �����J�n���ɌĂ΂��֐�
	virtual void Init() {}
	// ����ۂɊւ�炸���t���[���Ă΂��֐�
	virtual void PreProcess() {}
	// �g�݂Ղ悪�o���`�ݒu�������鎞�܂ŁA���t���[���Ă΂��֐�
	virtual void Main() {}

};

#endif