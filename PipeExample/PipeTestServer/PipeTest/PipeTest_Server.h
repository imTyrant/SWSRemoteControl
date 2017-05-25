#pragma once
#include<Windows.h>

HANDLE serverPipHandle;
const wchar_t* pipeName = L"\\\\.\\pipe\\myPipe";

void ServerCreatePIPE() throw(DWORD);
void ReadFromPipe();