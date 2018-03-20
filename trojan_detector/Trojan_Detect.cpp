#include <iostream>
#include <Windows.h>
#include <string>
#include "ProcFind.h"

using namespace std;

int main()
{
	ProcFind pf = ProcFind();
	pf.PrintCurrentProcesses();
	if (pf.HACKED)
	{
		char a;
		cout << "Infected!\nStop?(y/n): ";
		cin >> a;
		if (a == 'y');
		{
			pf.DoKill();
			Sleep(200);
		}
	}
	else 
	{
		cout << "Safe!\n" << endl;
	}
	
	
	return 0;
}