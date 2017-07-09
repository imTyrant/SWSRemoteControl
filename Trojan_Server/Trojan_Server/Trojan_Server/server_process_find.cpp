#include "stdafx.h"
#include"server_process_find.h"

using namespace std;


ProcFind::ProcFind(int mid)
{
	this->mID = mid;
	this->hEvent = gExchangeBuffer[mid].sendEvent;
	this->readBuffer = gExchangeBuffer[mid].mainWrite;
	this->writeBuffer = gExchangeBuffer[mid].mainRead;
}

ProcFind::~ProcFind()
{
	cout << ":(" << endl;
}

void ProcFind::PrintCurrentProcesses(void) throw(DWORD)
{
	wstring result;

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
		
		result.append(to_wstring(pe32.th32ProcessID));
		result.append(L"\t\t\t\t\t");
		result.append(pe32.szExeFile);
		result.append(L"\r\n");

		//wcout << pe32.szExeFile << "\t\t" << pe32.th32ProcessID << endl;
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	int size = result.size() * sizeof(wchar_t);

	char *rtnData = new char[size];

	ZeroMemory(rtnData, size);
	memcpy(rtnData, result.c_str(), size);

	int packs = size;

	memcpy(writeBuffer, (BYTE*)&size, 4);
	gExchangeBuffer[mID].sendSize = 1024;
	ResetEvent(hEvent);
	while (!gQueueAccessLocked)
	{
		gQueueAccessLocked = TRUE;
		gSendDataQueues.push(mID);
		gQueueAccessLocked = FALSE;
		break;
	}
	WaitForSingleObject(hEvent, INFINITE);

	if ((packs % 1024) > 0)
	{
		packs = packs / 1024 + 1;
	}
	else
	{
		packs = packs / 1024;
	}

	for (int i = 0; i < packs; i++)
	{

		ZeroMemory(writeBuffer, 1024);
		if (i != packs - 1)
		{
			memcpy(writeBuffer, &(rtnData[i * 1024]), 1024);
			gExchangeBuffer[mID].sendSize = 1024;
		}
		else
		{
			memcpy(writeBuffer, &(rtnData[i * 1024]), (size - i * 1024));
			gExchangeBuffer[mID].sendSize = (size - i * 1024);
		}

		ResetEvent(hEvent);
		while (!gQueueAccessLocked)
		{
			gQueueAccessLocked = TRUE;
			gSendDataQueues.push(mID);
			gQueueAccessLocked = FALSE;
			break;
		}
		WaitForSingleObject(hEvent, INFINITE);
	}
	delete[] rtnData;
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
	BOOL bRet;
	if (!(bRet = TerminateProcess(hProcess, 0)))
	{
		cout << "Can't Close Process : " << dwProcessId << endl;
		throw GetLastError();
	}
	CloseHandle(hProcess);
	return;
}