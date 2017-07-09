#pragma once

typedef struct CMD_HANDLE
{
	HANDLE input;
	HANDLE output;
};

class RemoteCMD
{
public:
	bool CMDMUTEX = false;
	RemoteCMD(int mID);
	RemoteCMD(BYTE *inputbuffer, BYTE *outputbuffer, HANDLE sendEvent);
	~RemoteCMD();
	void remoteContrl(void) throw(...);

private:

	BYTE* readBuffer;
	BYTE* writeBuffer;

	HANDLE sendEvent;
	CMD_HANDLE get, push;
	HANDLE hCMD;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;

	wchar_t* cmd = L"cmd /k cd /d C:";
	DWORD errocode;
};