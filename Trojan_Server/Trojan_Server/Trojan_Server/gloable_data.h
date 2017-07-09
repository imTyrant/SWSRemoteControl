#pragma once
#include "stdafx.h"
typedef struct RAWDATAPOOL
{
	HANDLE sendEvent; //事件标记
	DWORD PID; //线程id
	BYTE* mainRead; //主线程读取缓冲
	BYTE* mainWrite; // 主线程写缓冲
	int sendSize;//写大小
};

typedef struct TROJAN_THREAD
{
	int mID;
	RAWDATAPOOL exData;
};

#define GLOABLE_EXCHANGE_DATA_BUFFER  std::map<int, RAWDATAPOOL>
#define SEND_DATA_QUEUES std::queue<int>
#define THREAD_POOL std::vector<TROJAN_THREAD>

extern GLOABLE_EXCHANGE_DATA_BUFFER  gExchangeBuffer;

extern SEND_DATA_QUEUES gSendDataQueues;

extern THREAD_POOL gThreadPool;

extern BOOL gQueueAccessLocked;