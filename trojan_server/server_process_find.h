#pragma once

#define KILL_PROC  WM_USER + 400 + 2

class ProcFind
{
public:
	ProcFind(int mid);
	~ProcFind();

	void PrintCurrentProcesses(void) throw(...);
	void TernimateProcessById(DWORD dwProcessId) throw(...);

private:

	int mID;

	HANDLE hEvent;

	BYTE* readBuffer;
	BYTE* writeBuffer;

	MSG msg;

};