#pragma once
#include<Windows.h>

HANDLE serverPipHandle;
const wchar_t* pipeName = L"\\\\.\\pipe\\myPipe";
bool CMDMUTEX = true;

DWORD ReadFromPipe(char* inputBuffer) throw(DWORD);
BOOL WriteToPipe(char* buffer, int length);