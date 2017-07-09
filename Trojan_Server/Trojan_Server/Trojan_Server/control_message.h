#pragma once

#define SOCKET_SENDED			WM_USER + 100 + 1
#define SOCKET_DISSCONECED		WM_USER + 100 + 2
#define SCOKET_NEW_MESSAGE		WM_USER + 100 + 3

#define NEW_COMMAND				WM_USER + 200 + 1

#define NEW_CMD_COMMAND			WM_USER + 300 + 1
#define TERMINATE_CMD			WM_USER + 300 + 2



#define SYS_SHUTDOWN_MID			'0'
#define SYS_NEW_CMD					'1'
#define SYS_NEW_PIC_SHOOT			'2'
#define SYS_NEW_PROC_FIND			'3'

#define FOR_CMD						'A'
#define FOR_PIC_SHOOT				'B'
#define FOR_PROC_FIND				'C'

typedef struct trojan_head
{
	BYTE CommandType;//命令类型
	int mID;//mID
	int Length;//长度
};
