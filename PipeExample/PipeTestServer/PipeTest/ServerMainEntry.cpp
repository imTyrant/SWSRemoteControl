#include<iostream>
#include"PipeTest_Server.h"

using namespace std;



int main(int argc, char* argv[])
{
	try
	{
		ServerCreatePIPE();
	}
	catch (DWORD e)
	{
		cout << "Error! The Code is:" << hex << e << endl;
	}
	ReadFromPipe();
	system("PAUSE");
	return 0;
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
	HANDLE serevrEvent = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		NULL
	);
	if (!serevrEvent)
	{
		cout << "Create Event Failed!" << endl;
		throw GetLastError();
	}
	cout << "Connecting Clients..." << endl;
	if (!ConnectNamedPipe(serverPipHandle, NULL))
	{
		if (ERROR_IO_PENDING != GetLastError())
		{
			CloseHandle(serverPipHandle);
			CloseHandle(serevrEvent);
			serverPipHandle = NULL;
			serevrEvent = NULL;

			cout << "Client Connection Failed!\n" << endl;

			throw GetLastError();
		}
	}
	cout << "Wait For Client Connect to Pipe..." << endl;
	if (WAIT_FAILED == WaitForSingleObject(serevrEvent, 10))
	{
		CloseHandle(serverPipHandle);
		CloseHandle(serevrEvent);
		serverPipHandle = NULL;
		serevrEvent = NULL;

		cout << "Object Failed!\n" << endl;
		return;
	}
	CloseHandle(serevrEvent);
	serevrEvent = NULL;
}

void ReadFromPipe()
{
	DWORD blen;
	char* buffer = new char[255];
	if (!ReadFile(serverPipHandle, buffer, 255, &blen, NULL))
	{
		delete[]buffer;
		cout << "Read Failed!" << endl;
		return;
	}
	int i = 0;
	cout << "Read From Client Successful" << endl;
	cout << buffer << endl;
}