#pragma once
#include "stdafx.h"
typedef struct RAWDATAPOOL
{
	HANDLE sendEvent; //�¼����
	DWORD PID; //�߳�id
	BYTE* mainRead; //���̶߳�ȡ����
	BYTE* mainWrite; // ���߳�д����
	int sendSize;//д��С
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