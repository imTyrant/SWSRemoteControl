#include "stdafx.h"
#include "server_pic_shoot.h"
#include "server_process_find.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;
/************************DATA***********************/

WSADATA wsaData;
SOCKET socketClient;
SOCKADDR_IN socketServer;

char* ServerIP = "127.0.0.1";
u_short ServerPort = 23333;

DWORD MainThreadID;

int customer = -1;


/***********************THREAD*********************/

BOOL socketInit();

DWORD dataSend(LPVOID lpParameter);

DWORD dataRecv(LPVOID lpParameter);

int main(int argc, char** argv)
{
	MSG msg;
	MainThreadID = GetCurrentThreadId();
	int netSend = 0;
	int netRecv = 1;

	RAWDATAPOOL sendDataPool;
	RAWDATAPOOL recvDataPool;

	gExchangeBuffer.insert(pair<int, RAWDATAPOOL>(netSend, sendDataPool));
	gExchangeBuffer.insert(pair<int, RAWDATAPOOL>(netRecv, recvDataPool));

	if (NULL == (gExchangeBuffer[0].sendEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		cout << "Create event failed!" << GetLastError() << endl;
		gExchangeBuffer.clear();
		return -1;
	}

	gExchangeBuffer[netRecv].mainRead = new BYTE[1024];

	if (NULL == CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)dataSend, (LPVOID)&sendDataPool, NULL, &(gExchangeBuffer[0].PID)))
	{
		CloseHandle(gExchangeBuffer[netSend].sendEvent);
		delete[] gExchangeBuffer[netRecv].mainRead;
		gExchangeBuffer.clear();
		return -1;
	}
	if (NULL == CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)dataRecv, (LPVOID)&recvDataPool, NULL, &(gExchangeBuffer[1].PID)))
	{
		CloseHandle(gExchangeBuffer[netSend].sendEvent);
		delete[] gExchangeBuffer[netRecv].mainRead;
		gExchangeBuffer.clear();
		return -1;
	}
_SocketStart:
	while (!socketInit())
	{
		Sleep(5000);
	}
	while (1)
	{
		if (gSendDataQueues.empty())
		{
			continue;
		}
	}

	CloseHandle(gExchangeBuffer[netSend].sendEvent);
	delete[] gExchangeBuffer[netRecv].mainRead;
	gExchangeBuffer.clear();
	return 0;
}

BOOL socketInit()
{
	WORD winVersion = MAKEWORD(2, 2);

	if (WSAStartup(winVersion, &wsaData))
	{
		//cout << "Init socket failed!" << endl;
		return FALSE;
	}

	if ((LOBYTE(wsaData.wVersion) != 2) || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return FALSE;
	}

	socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == socketClient)
	{
		WSACleanup();
		return FALSE;
	}

	socketServer.sin_addr.S_un.S_addr = inet_addr(ServerIP);
	socketServer.sin_family = AF_INET;
	socketServer.sin_port = htons(ServerPort);
	int result = connect(socketClient, (SOCKADDR*)&socketServer, sizeof(SOCKADDR));
	if (result == SOCKET_ERROR)
	{
		cout << hex << WSAGetLastError() << endl;
		return FALSE;
	}

	return TRUE;
}

DWORD dataSend(LPVOID lpParameter)
{
	int mID = *(int*)lpParameter;
	while (1)
	{
		WaitForSingleObject(gExchangeBuffer[mID].sendEvent, INFINITE);
		if (customer < 1)
		{
			ResetEvent(gExchangeBuffer[mID].sendEvent);
			continue;
		}
		BYTE* buffer = gExchangeBuffer[customer].mainRead;
		int result = send(socketClient, (char*)buffer, 1024, 0);
		if (result == SOCKET_ERROR)
		{
			PostThreadMessage(MainThreadID, SOCKET_DISSCONECED, NULL, NULL);
			return -1;
		}

		PostThreadMessage(MainThreadID, SOCKET_SENDED, NULL, NULL);

		ResetEvent(gExchangeBuffer[mID].sendEvent);
	}

	return 0;
}

DWORD dataRecv(LPVOID lpParameter)
{
	int mID = *(int*)lpParameter;
	BYTE* writeBuffer = gExchangeBuffer[mID].mainRead;
	int result;
	while (1)
	{
		result = recv(socketClient, (char*)writeBuffer, 1024, 0);
		if (result == SOCKET_ERROR)
		{
			PostThreadMessage(MainThreadID, SOCKET_DISSCONECED, NULL, NULL);
			return -1;
		}
		PostThreadMessage(MainThreadID, SCOKET_NEW_MESSAGE, NULL, NULL);
	}
	return 0;
}