#pragma once
#include"stdafx.h"
using namespace std;

class ProcFind
{
public:
	ProcFind();
	~ProcFind();

	void PrintCurrentProcesses(void) throw(DWORD);
	void TernimateProcessById(DWORD dwProcessId) throw(DWORD);

private:

};