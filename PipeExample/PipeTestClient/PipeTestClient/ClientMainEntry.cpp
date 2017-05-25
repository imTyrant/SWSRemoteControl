#include<iostream>
#include"PipeTest_Client.h"

using namespace std;

int main(int agrc, char* argv[])
{
	try
	{
		OpenNamedPipe();
		WriteToPipe();
	}
	catch (DWORD e)
	{
		cout << "Error! Code is:" << hex << e << endl;
	}
	system("PAUSE");
	return 0;
}

void OpenNamedPipe() throw(DWORD)
{
	cout << "Connecting Existed Pipe..." << endl;
	if (!WaitNamedPipe(pipeName, NMPWAIT_WAIT_FOREVER))
	{
		cout << "Client Connect Pipe Failed!" << endl;
		throw GetLastError();
	}
	clientPipHandle = CreateFile(
		pipeName,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == clientPipHandle)
	{
		cout << "Open Pipe Failed!" << endl;
		throw GetLastError();
	}
}

void WriteToPipe() throw(DWORD)
{
	DWORD writer;
	char* message = "Hello World\n";

	if (!WriteFile(clientPipHandle, message, strlen(message), &writer, NULL))
	{
		cout << "Writing Message Failed!" << endl;
		throw GetLastError();
	}
	cout << "Write Message Successful!" << endl;
}
