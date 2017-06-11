#pragma once
#include"stdafx.h"

#define QUIT_PIC 0x9000

#define BMP 0x1
#define PNG 0x2
#define JPEG 0x3

using namespace std;

class PicShot
{
public:

	PicShot();
	PicShot(DWORD frequency);
	PicShot(DWORD frequency, wchar_t* path);
	~PicShot();

	void setClockTick(DWORD frequency);
	void setStorePath(wchar_t* path);

	void Shoot(void);
	void PicCircule(void);
	//void KillMe(void);

private:
	wchar_t currentWND[MAX_PATH] = { 0 };
	UINT clockTick = 5000;
	UINT_PTR timerID;
	DWORD dPicCounter = 0;
	wchar_t storePath[MAX_PATH] = { 0 };
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);

};
/*
class PicShot
{
public:
	PicShot();
	~PicShot();
	void setClockTick(DWORD frequency);
	void Shoot(void);
	void PicCircule(void);
	friend std::istream& operator>>(std::istream& is, PicShot& t);

private:
	UINT picQuality;
	UINT clockTick;
	UINT_PTR timerID;
	DWORD dPicCounter = 1;

	static DWORD WINAPI ThreadProc(LPVOID lpParameter);

};
*/