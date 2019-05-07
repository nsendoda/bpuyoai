//------------------------------------------------------------------------------
//  puyo_ai.h
//------------------------------------------------------------------------------
#pragma once



#ifdef __cplusplus
extern "C"{
#endif

#define PUYO_AI_VERSION		4

typedef struct{
	int score;
	int rensa;
	int tunagi_a, tunagi_b;
	int x_a, y_a;
	int x_b, y_b;
	int quick;
} AI_SCORE;

typedef struct{
	int a;
	int b;
} AI_NOW;

typedef struct{
	float x, y;
	int rot;
} AI_NOWPOS;

typedef struct{
	int connect;
	int color;
	int lx, rx;
	int dy, uy;
} AI_CONNECT_ONE;

typedef struct{
	int num;
	AI_CONNECT_ONE *info;
} AI_CONNECT;

//------------------------------------------------------------------------------
void ai_SetTumi( int x, int rot );
AI_SCORE* ai_GetScore( int x, int rot );
AI_SCORE* ai_GetScore2( int player, int x, int rot );
AI_NOW* ai_GetNow();
AI_NOW* ai_GetNow2( int player );
void ai_SetNow2( int player, int a, int b ); // ※使用不可
AI_SCORE* ai_GetScoreE( int x, int rot );
void ai_Reset();
AI_NOW* ai_GetNext();
AI_NOW* ai_GetNext2( int player );
void ai_SetNext();
void ai_SetTumiNum( int n );
int ai_GetSpace( int player );
unsigned char* ai_GetField( int player ); // ※Lua版に合わせて、[1]からスタートする6x18の配列へのポインタが返される
void ai_Save( int n );
void ai_Load( int n );
int ai_GetCount();
int ai_GetCount2( int player );
void ai_SetTable( int type, int reverse, char *tbl );
void ai_SetDefSpeed( int speed );
int ai_GetBattleCount();
unsigned char* ai_GetMemory();
void ai_SetName( char *name );
void ai_SetMainMode( int mode );
void ai_SetBMainMode( int mode );
int ai_IsTumiReserved();
void ai_SetField( int player, unsigned char *field ); // ※[1]からスタートする6x18の配列を渡す
AI_CONNECT* ai_GetConnectInfo( int player, int a, int b );
void ai_Delete( int player, int x, int y );
void ai_SetPad( int up, int right, int down, int left, int a, int b );
int ai_GetSettingSpeed();
int ai_GetSettingMawasi();
int ai_GetSettingGiveup();
int ai_GetDropSpeed( int player );
int ai_GetYokokuOjama( int player );
AI_NOWPOS* ai_GetNowPos( int player );
int ai_GetPlayerMode( int player );
int ai_GetTotalScore( int player );


#ifdef __cplusplus
} // extern "C"
#endif

