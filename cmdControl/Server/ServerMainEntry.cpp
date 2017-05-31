#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"PipeTest_Server.h"

#define PIPETEST

using namespace std;

typedef struct CMD_HANDLE
{
	HANDLE input;
	HANDLE output;
};

void waitCMDQuit(HANDLE waitingProc) {
	CMDMUTEX = true;
	::WaitForSingleObject(waitingProc, INFINITE);
	CMDMUTEX = true;
}

void ServerCMD() throw(DWORD)
{
	CMD_HANDLE get, push;
	HANDLE hCMD;
	
	SECURITY_ATTRIBUTES sa;
	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	wchar_t cmd[] = L"cmd /k cd /d C:";

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

	if (!CreateProcess(
		NULL,
		cmd,
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	)) 
	{
		cout << "Create CMD Process Failed!\n";
		CloseHandle(push.input);
		CloseHandle(push.output);
		CloseHandle(get.input);
		CloseHandle(get.output);
		throw GetLastError();
	}
	HANDLE waitingHandle = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)waitCMDQuit,
		pi.hProcess,
		0,
		NULL
	);
	if (!waitingHandle) 
	{
		cout << "Init Mutex Failed!\n";
		CloseHandle(push.input);
		CloseHandle(push.output);
		CloseHandle(get.input);
		CloseHandle(get.output);
		throw GetLastError();
	}
	while (CMDMUTEX)
	{
		DWORD written;
		DWORD readed;
		DWORD commandLength;
		char* Buffer = NULL;
		try
		{
			commandLength = ReadFromPipe(Buffer);
			Buffer = new char[commandLength + 1];
			if (!ReadFile(serverPipHandle, Buffer, commandLength, &readed, NULL))
			{
				delete[]Buffer;
				cout << "Read Commands Faild!" << endl;
				throw GetLastError();
			}
			strcat(Buffer, "\n");
			cout << "Read From Client Successful" << endl;
			cout << Buffer << endl;
		}
		catch (int e)
		{
			cout << "CMD Read From Pipe Failed!" << hex << GetLastError() << "\n";
			continue;
		}
		if (commandLength <= 0)
		{
			cout << "Nothing Will Write into InPipe!" << hex << GetLastError() << "\n";
			continue;
		}
		if (!WriteFile(get.input, Buffer, commandLength, &written, NULL))
		{
			cout << "InPipe Can't be Wrote!" << hex << GetLastError() << "\n";
			continue;
		}
		Sleep(2000);
		delete[]Buffer;
		DWORD dReader;
		string reply = "";
		Buffer = NULL;
		Buffer = new char[1024];
		while (PeekNamedPipe(push.output, Buffer, 1024, &dReader, NULL, 0) && dReader > 0)
		{
			memset(Buffer, 0, 1024);
			ReadFile(push.output, Buffer, dReader, &dReader, NULL);
			WriteFile(serverPipHandle, Buffer, dReader, &dReader, NULL);
			//reply += Buffer;
		}
		//cout << reply << "\n";
		delete[]Buffer;
		Buffer = NULL;
	}

	CloseHandle(push.input);
	CloseHandle(push.output);
	CloseHandle(get.input);
	CloseHandle(get.output);
	CloseHandle(waitingHandle);
	
	return;
}

void ServerCreatePIPE() throw(DWORD)
{
	serverPipHandle = CreateNamedPipe(
		pipeName,
		PIPE_ACCESS_DUPLEX,
		0,
		1,
		1024,
		1024,
		0,
		NULL
	);
	if (serverPipHandle == INVALID_HANDLE_VALUE)
	{
		serverPipHandle = NULL;
		cout << "Create Pipe Failed!" << endl;
		throw GetLastError();
	}
	cout << "Create Pipe Successful!" << endl;

	cout << "Connecting Clients..." << endl;
	if (!ConnectNamedPipe(serverPipHandle, NULL))
	{
		if (ERROR_IO_PENDING != GetLastError())
		{
			CloseHandle(serverPipHandle);
			//CloseHandle(serevrEvent);
			serverPipHandle = NULL;
			//serevrEvent = NULL;

			cout << "Client Connection Failed!\n" << endl;

			throw GetLastError();
		}
	}
}

DWORD ReadFromPipe(char* inputBuffer) throw(DWORD)
{
	DWORD commandsLength = 0;
	DWORD readedLength = 0;

	if (!ReadFile(serverPipHandle, &commandsLength, 4, &readedLength, NULL))
	{
		cout << "Can't Read From Pipe!" << endl;
		throw GetLastError();
	}
	cout << "commands length is" << commandsLength << endl;
	if (commandsLength <= 0)
	{
		cout << "Commands Invaild!" << endl;
		throw GetLastError();
	}
	//inputBuffer = new char[commandsLength];
	//if (!ReadFile(serverPipHandle, inputBuffer, commandsLength, &readedLength, NULL))
	//{
	//	delete[]inputBuffer;
	//	cout << "Read Commands Faild!" << endl;
	//	throw GetLastError();
	//}
	//cout << "Read From Client Successful" << endl;
	//cout << inputBuffer << endl;
	return commandsLength;
	//system(buffer);
}

BOOL WriteToPipe(char* buffer, int length)
{
	DWORD written;
	DWORD commandsLength = length + 1;

	if (!WriteFile(serverPipHandle, &commandsLength, 4, &written, NULL))
	{
		cout << "Reply To Client Failed" << endl;
		return FALSE;
	}
	if (!WriteFile(serverPipHandle, buffer, commandsLength, &written, NULL))
	{
		cout << "Reply To Client Failed" << endl;
		return FALSE;
	}
	cout << "Reply To Client Successful!" << endl;

	return TRUE;
}

int main(int argc, char* argv[])
{
#ifdef DEBUG

	wchar_t cmd[] = L"cmd /k cd /d C:";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;;//指定wShowWindow成员效
	si.wShowWindow = TRUE;//此成员设为TRUE的话则显示新建进程的主窗口
	BOOL bRet = CreateProcess(
		NULL,//不在此指定可执行文件的文件名
		cmd,//命令行参数
		NULL,//默认进程安全性
		NULL,//默认进程安全性
		FALSE,//指定当前进程内句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		&pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;

#endif

#ifdef PIPETEST

	try
	{
		ServerCreatePIPE();
		ServerCMD();
	}
	catch (DWORD e)
	{
  		cout << "Error! The Code is:" << hex << e << endl;
	}
	//ReadFromPipe();
	system("PAUSE");
 	return 0;

#endif

}