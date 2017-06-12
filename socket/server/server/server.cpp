// server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Winsock2.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

void main()
{
	int err;                       // ������Ϣ
	int len = sizeof(SOCKADDR);

	char  cmdStr[100] = { 0 };     // ���͵��ͻ��˵�����
	char recvBuf[1024] = { 0 };    // ���ܿͻ��˷��ص��ַ���
	char *ip;

	SOCKET sockServer;            // ����� Socket
	SOCKET sockClient;            // �ͻ��� Scoket
	SOCKADDR_IN addrServer;       // ����˵�ַ
	SOCKADDR_IN addrClient;       // �ͻ��˵�ַ

	WSADATA wsaData;              // winsock �ṹ��
	WORD wVersionRequested;       // winsock �İ汾

    // ���� Windows Socket�汾
	wVersionRequested = MAKEWORD(2, 2);

	// ��ʼ�� Windows Socket
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		// �������󣬳������
		return;
	}

	/*
	* ȷ��WinSock DLL֧��2.2
	* ��ע�����DLL֧�ֵİ汾����2.2��2.2
	* ����Ȼ�᷵��wVersion2.2�İ汾
	*/

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		// �������󣬳������
		WSACleanup(); // ��ֹWinsock 2 DLL (Ws2_32.dll) ��ʹ��
		return;
	}

	// �����������socket
	sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//  ���÷���� socket
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);       // ����IP
	addrServer.sin_family = AF_INET;                           // Э��������INET
	addrServer.sin_port = htons(6000);                         // �󶨶˿�6000
	
	// ����������socket���ڱ��ض˿�
	bind(sockServer, (SOCKADDR *)&addrServer, len);

	// Listen �����˿�
	listen(sockServer, 5);                                     // 5 Ϊ�ȴ�������Ŀ

	while (1)
	{
		printf("������������:\n������...\n");
		// accept ���������̣�ֱ���пͻ�����������Ϊֹ
		sockClient = accept(sockServer, (SOCKADDR *)&addrClient, &len);
		// ��ȡip��ַ
		ip = inet_ntoa(addrClient.sin_addr);
		// ���������ʾ
		printf("-- IP %s ���ӵ������\n", ip);

		while (1)
		{
			printf(">>");
			gets_s(cmdStr);
			// ������͵��ͻ���
			send(sockClient, cmdStr, strlen(cmdStr) + 1, 0);
			if (strcmp(cmdStr, "exit") == 0)
				break; 
			recv(sockClient, recvBuf, 1024, 0);
			printf("-- %s: %s\n", ip, recvBuf);
		}
		// �ر�socket
		closesocket(sockClient);
		WSACleanup();
	}
}
	

