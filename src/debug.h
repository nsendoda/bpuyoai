#ifndef BPUYOAI_DEBUG_H_
#define BPUYOAI_DEBUG_H_

#include <cstdio>

#define NOMINMAX
#include <windows.h>

class Debug {
public:
  template <typename... Args>
  static void Printf(const char *format, Args const &... args) {
    // int printf(const char *format, ...);
		printf(format, args...);
  }
	static void Print(const char *pFormat, ...) {
		char    szBuff[1024];
		va_list argptr;

		va_start(argptr, pFormat);
		vsprintf(szBuff, pFormat, argptr);
		va_end(argptr);

		OutputDebugString(szBuff);
	}
};

#endif
