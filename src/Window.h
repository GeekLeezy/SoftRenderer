#pragma once
#include <windows.h>
#include <commctrl.h>

class Window
{
public:
	HWND hWnd;
	int width;
	int height;
	HDC hDC;
	HDC hMem;
	void* bmpBuffer;

	Window(const int &w, const int &h);
	~Window();


};

