#include "puyo_ai.h"

#include "src/ai/ai.h"

#include "src/ai/researcher.h"
#include "src/ai/nomi.h"
#include "src/ai/sea.h"

#include <memory>

std::unique_ptr<AI> ai;


//------------------------------------------------------------------------------
//  �ŏ���1�񂾂��Ă΂��֐�
//------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall ai_init()
{
	ai.reset(new Nomi());
	ai->Init();
}

//------------------------------------------------------------------------------
//  ����ۂɊւ�炸��ɌĂ΂��֐�
//------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall ai_bmain()
{
	ai->PreProcess();
}

//------------------------------------------------------------------------------
//  �g�Ղ悪�o�������u�ԂɌĂ΂��֐�
//------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall ai_main()
{
	ai->Main();
}

