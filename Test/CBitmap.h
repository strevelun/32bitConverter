#pragma once

#include "CDC.h"

class CBitmap
{
    HBITMAP m_bitmap;
    HANDLE m_hFile;
	long m_width = 0, m_height = 0;
	LPDWORD m_pixels = NULL;
	HDC m_hMemDC;

public:
	CBitmap(HWND hWnd, LPCWSTR fileName);

	void Render(HWND hWnd);
	void Render(HDC hdc, long dx, long dy, long dw, long dh);

};

