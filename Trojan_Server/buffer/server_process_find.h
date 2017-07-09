#pragma once

class ProcFind
{
public:
	ProcFind();
	~ProcFind();

	void PrintCurrentProcesses(void) throw(...);
	void TernimateProcessById(DWORD dwProcessId) throw(...);

private:

};