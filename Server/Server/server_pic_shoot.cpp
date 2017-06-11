#include"stdafx.h"
#include"server_picshoot.h"

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

PicShot::PicShot()
{
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
	KillTimer(NULL, timerID);
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

	char* bitmap = new char[bmpSize];
	ZeroMemory(bitmap, bmpSize);

	GetDIBits(
		hScreenInMem,
		hBitmap,
		0,
		(UINT)height,
		bitmap,
		(BITMAPINFO*)&bmpInfoHeader,
		DIB_RGB_COLORS
	);

	time_t current;
	current = time(NULL);

	wstring name = storePath;
	name += L"picshoot_";
	name += to_wstring(current);
	name += L".bmp";

	ofstream ofs(name, ios_base::binary);
	ofs.write((char*)&bmpfileHeader, sizeof(BITMAPFILEHEADER));
	ofs.write((char*)&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	ofs.write((char*)bitmap, bmpSize);
	ofs.close();
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

	timerID = SetTimer(NULL, 0, clockTick, NULL);
	MSG msg;
	while (GetMessage(
		&msg,
		NULL,
		0,
		0
	))
	{
		if (msg.message == QUIT_PIC)
		{
			break;
		}
		if (msg.message == WM_TIMER)
		{
			szNewName = new wchar_t[MAX_PATH];
			ZeroMemory(szNewName, MAX_PATH);
			HWND hWND = GetForegroundWindow();
			GetWindowText(hWND, szNewName, MAX_PATH);
			wcout << "Current Windows Is : " << szNewName << endl;
			if (wcscmp(currentWND, szNewName))
			{
				wcscpy(currentWND, szNewName);
				CreateThread(
					NULL,
					0,
					ThreadProc,
					this,
					0,
					NULL
				);
				dPicCounter++;
			}
			delete[]szNewName;
		}
	}

	cout << "See U!" << endl;
	return;
}