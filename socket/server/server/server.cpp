// server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Winsock2.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

void main()
{
	int err;                       // 错误信息
	int len = sizeof(SOCKADDR);

	char  cmdStr[100] = { 0 };     // 发送到客户端的命令
	char recvBuf[1024] = { 0 };    // 接受客户端返回的字符串
	char *ip;

	SOCKET sockServer;            // 服务端 Socket
	SOCKET sockClient;            // 客户端 Scoket
	SOCKADDR_IN addrServer;       // 服务端地址
	SOCKADDR_IN addrClient;       // 客户端地址

	WSADATA wsaData;              // winsock 结构体
	WORD wVersionRequested;       // winsock 的版本

    // 配置 Windows Socket版本
	wVersionRequested = MAKEWORD(2, 2);

	// 初始化 Windows Socket
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		// 启动错误，程序结束
		return;
	}

	/*
	* 确认WinSock DLL支持2.2
	* 请注意如果DLL支持的版本大于2.2至2.2
	* 它仍然会返回wVersion2.2的版本
	*/

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		// 启动错误，程序结束
		WSACleanup(); // 终止Winsock 2 DLL (Ws2_32.dll) 的使用
		return;
	}

	// 定义服务器端socket
	sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//  设置服务端 socket
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);       // 本机IP
	addrServer.sin_family = AF_INET;                           // 协议类型是INET
	addrServer.sin_port = htons(6000);                         // 绑定端口6000
	
	// 将服务器端socket绑定在本地端口
	bind(sockServer, (SOCKADDR *)&addrServer, len);

	// Listen 监听端口
	listen(sockServer, 5);                                     // 5 为等待连接数目

	while (1)
	{
		printf("服务器已启动:\n监听中...\n");
		// accept 会阻塞进程，直到有客户端连接上来为止
		sockClient = accept(sockServer, (SOCKADDR *)&addrClient, &len);
		// 获取ip地址
		ip = inet_ntoa(addrClient.sin_addr);
		// 输出连接提示
		printf("-- IP %s 连接到服务端\n", ip);

		while (1)
		{
			printf(">>");
			gets_s(cmdStr);
			// 将命令发送到客户端
			send(sockClient, cmdStr, strlen(cmdStr) + 1, 0);
			if (strcmp(cmdStr, "exit") == 0)
				break; 
			recv(sockClient, recvBuf, 1024, 0);
			printf("-- %s: %s\n", ip, recvBuf);
		}
		// 关闭socket
		closesocket(sockClient);
		WSACleanup();
	}
}
	

