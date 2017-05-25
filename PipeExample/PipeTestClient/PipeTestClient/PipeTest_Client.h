#pragma once
#include<Windows.h>

HANDLE clientPipHandle;
const wchar_t* pipeName = L"\\\\.\\pipe\\myPipe";

void OpenNamedPipe() throw(DWORD);
void WriteToPipe() throw(DWORD);
