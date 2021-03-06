#include "puyo_ai.h"

#include "src/ai/ai.h"

#include "src/ai/researcher.h"
#include "src/ai/nomi.h"
#include "src/ai/sea.h"

#include <memory>

std::unique_ptr<AI> ai;


//------------------------------------------------------------------------------
//  最初に1回だけ呼ばれる関数
//------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall ai_init()
{
	ai.reset(new Nomi());
	ai->Init();
}

//------------------------------------------------------------------------------
//  操作可否に関わらず常に呼ばれる関数
//------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall ai_bmain()
{
	ai->PreProcess();
}

//------------------------------------------------------------------------------
//  組ぷよが出現した瞬間に呼ばれる関数
//------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall ai_main()
{
	ai->Main();
}

