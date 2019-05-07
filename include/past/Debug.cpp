#include "Debug.h"

void Debug::Print(const char *pFormat, ...)
{
	char    szBuff[1024];
	va_list argptr;

	va_start(argptr, pFormat);
	vsprintf(szBuff, pFormat, argptr);
	va_end(argptr);

	OutputDebugString(szBuff);

	return;
}
