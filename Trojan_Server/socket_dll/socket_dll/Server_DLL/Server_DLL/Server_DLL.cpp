// Server_DLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <Winsock2.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

//服务端(即被控制端)调用这个主动连接客户端建立socket，需要传入客户端的ip，成功时返回服务端socket的句柄(其实不是句柄，这里可以理解成句柄)，出现异常时返回null
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

//服务端向客户端发送字符串，其中第一个参数是上一个函数返回的服务端socket，第二个是字符串的指针，第三个是字符串长度
extern "C" __declspec(dllexport) void sendChar(SOCKET socketServer, char *sendData, int length)
{
	send(socketServer, sendData, length, 0);
	//
	//
	//
}

//服务端接收客户端发送的字符串，收到的数据在recvData中
extern "C" __declspec(dllexport) void recvChar(SOCKET socketServer, char *recvData, int length)
{
	recv(socketServer, recvData, length, 0);


	//
}

//与第二个函数类似，只是第二个参数的类型变成了字节(用于发送图片字节流)
extern "C" __declspec(dllexport) void sendByte(SOCKET socketServer, BYTE *sendData, int length)
{
	send(socketServer, (char*) sendData, length, 0);
}


