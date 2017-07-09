#include "stdafx.h"
#include"server_process_find.h"

using namespace std;


ProcFind::ProcFind()
{
	cout << ":)" << endl;
}

ProcFind::~ProcFind()
{
	cout << ":(" << endl;
}

void ProcFind::PrintCurrentProcesses(void) throw(DWORD)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << "Create Snapshot Failed!" << endl;
		throw GetLastError();
	}
	if (!Process32First(hProcessSnap, &pe32))
	{
		cout << "Get First Process Failed!" << endl;
		throw GetLastError();
	}
	do
	{
		wcout << pe32.szExeFile << "\t\t" << pe32.th32ProcessID << endl;
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return;
}

void ProcFind::TernimateProcessById(DWORD dwProcessId) throw(DWORD)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
	if (hProcess == NULL)
	{
		cout << "Close Process Failed! Can't Get Process Handle!" << endl;
		throw GetLastError();
	}
	if (BOOL bRet = TerminateProcess(hProcess, 0))
	{
		cout << "Can't Close Process : " << dwProcessId << endl;
		throw GetLastError();
	}
	CloseHandle(hProcess);
	return;
}