#pragma once

#include <iostream>
#include <string>
#include <map>
#include <windows.h>
#include <TlHelp32.h>

using namespace std;

class ProcFind
{
public:
	BOOL HACKED = FALSE;
	ProcFind();
	~ProcFind();

	void PrintCurrentProcesses(void) throw(DWORD);
	void TernimateProcessById(DWORD dwProcessId) throw(DWORD);
	void DoKill();
private:

	DWORD KILL;
};