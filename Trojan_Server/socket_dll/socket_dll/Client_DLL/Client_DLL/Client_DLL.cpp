// Client_DLL.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <Winsock2.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

//�ͻ���(�����ƶ�)����������������˿ڵȴ�socket���ӣ��ɹ�ʱ���ط����socket�ľ��(��ʵ���Ǿ��������������ɾ��)�������쳣ʱ����null
extern "C" __declspec(dllexport) SOCKET waitConnection()
{
	int error;
	int len = sizeof(SOCKADDR);

	SOCKET socketServer;
	SOCKET socketClient;
	SOCKADDR_IN addrServer;
	SOCKADDR_IN addrClient;

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	error = WSAStartup(wVersionRequested, &wsaData);

	if (error != 0)
		return -1;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	socketClient = socket(AF_INET, SOCK_STREAM, 0);

	addrClient.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);

	bind(socketClient, (SOCKADDR *)&addrClient, len);

	listen(socketClient, 5);

	socketServer = accept(socketClient, (SOCKADDR *)&addrServer, &len);

	return socketServer;
}

//�ͻ��������˷����ַ��������е�һ����������һ���������صķ����socket���ڶ������ַ�����ָ�룬���������ַ�������
extern "C" __declspec(dllexport) void sendChar(SOCKET socketServer, char *sendData, int length)
{
	send(socketServer, sendData, length, 0);
}

//�ͻ��˽��շ���˷��͵��ַ������յ���������recvData��
extern "C" __declspec(dllexport) void recvChar(SOCKET socketServer, char *recvData, int length)
{
	recv(socketServer, recvData, length, 0);
}

//��ڶ����������ƣ�ֻ�ǵڶ������������ͱ�����ֽ�(���ڽ���ͼƬ�ֽ���)
extern "C" __declspec(dllexport) void recvByte(SOCKET socketServer, BYTE *recvData, int length)
{
	recv(socketServer, (char *)recvData, length, 0);
}


