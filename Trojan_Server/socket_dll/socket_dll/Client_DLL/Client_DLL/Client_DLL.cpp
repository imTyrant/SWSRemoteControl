// Client_DLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <Winsock2.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

//客户端(即控制端)调用这个函数监听端口等待socket连接，成功时返回服务端socket的句柄(其实不是句柄，这里可以理解成句柄)，出现异常时返回null
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

//客户端向服务端发送字符串，其中第一个参数是上一个函数返回的服务端socket，第二个是字符串的指针，第三个是字符串长度
extern "C" __declspec(dllexport) void sendChar(SOCKET socketServer, char *sendData, int length)
{
	send(socketServer, sendData, length, 0);
}

//客户端接收服务端发送的字符串，收到的数据在recvData中
extern "C" __declspec(dllexport) void recvChar(SOCKET socketServer, char *recvData, int length)
{
	recv(socketServer, recvData, length, 0);
}

//与第二个函数类似，只是第二个参数的类型变成了字节(用于接收图片字节流)
extern "C" __declspec(dllexport) void recvByte(SOCKET socketServer, BYTE *recvData, int length)
{
	recv(socketServer, (char *)recvData, length, 0);
}


