#pragma once

#include <cstdio>
#include <windows.h>

class Debug{
public:
	static void Print(const char *pFormat, ...);
private:
	Debug();
};

