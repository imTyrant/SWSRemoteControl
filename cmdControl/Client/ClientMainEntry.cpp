#include<iostream>
#include<string>
#include"PipeTest_Client.h"

using namespace std;

int main(int agrc, char* argv[])
{
	char Buffer[1024];
	DWORD dReader;
	string reply;
	try
	{
		OpenNamedPipe();
		while (true)
		{
			WriteToPipe();
			while (PeekNamedPipe(clientPipHandle, Buffer, 1024, &dReader, NULL, 0) && dReader > 0)
			{
				memset(Buffer, 0, 1024);
				ReadFile(clientPipHandle, Buffer, dReader, &dReader, NULL);
				reply += Buffer;
			}
			cout << reply << endl;
		}
		
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
	string commands;
	getline(cin, commands);

	DWORD commandsLength = commands.length() + 1;

	if (!WriteFile(clientPipHandle, &commandsLength, 4, &writer, NULL))
	{
		cout << "Writing Message Failed!" << endl;
		throw GetLastError();
	}
	if (!WriteFile(clientPipHandle, commands.c_str(), commandsLength, &writer, NULL))
	{
		cout << "Writing Message Failed!" << endl;
		throw GetLastError();
	}
	cout << "Write Message Successful!" << endl;
}
