// Server_DLL.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <Winsock2.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

//�����(�������ƶ�)��������������ӿͻ��˽���socket����Ҫ����ͻ��˵�ip���ɹ�ʱ���ط����socket�ľ��(��ʵ���Ǿ��������������ɾ��)�������쳣ʱ����null
extern "C" __declspec(dllexport) SOCKET connectClinet(char *ip)
{
	int err = 0;

	SOCKET socketServer;
	SOCKADDR_IN addrClient;

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	socketServer = socket(AF_INET, SOCK_STREAM, 0);

	addrClient.sin_addr.S_un.S_addr = inet_addr(ip);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);

	connect(socketServer, (SOCKADDR *)&addrClient, sizeof(SOCKADDR));

	return socketServer;
}

//�������ͻ��˷����ַ��������е�һ����������һ���������صķ����socket���ڶ������ַ�����ָ�룬���������ַ�������
extern "C" __declspec(dllexport) void sendChar(SOCKET socketServer, char *sendData, int length)
{
	send(socketServer, sendData, length, 0);
	//
	//
	//
}

//����˽��տͻ��˷��͵��ַ������յ���������recvData��
extern "C" __declspec(dllexport) void recvChar(SOCKET socketServer, char *recvData, int length)
{
	recv(socketServer, recvData, length, 0);


	//
}

//��ڶ����������ƣ�ֻ�ǵڶ������������ͱ�����ֽ�(���ڷ���ͼƬ�ֽ���)
extern "C" __declspec(dllexport) void sendByte(SOCKET socketServer, BYTE *sendData, int length)
{
	send(socketServer, (char*) sendData, length, 0);
}


