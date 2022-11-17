#pragma once

#include "framework.h"

class CBitmap;

class CDC
{
	CBitmap* m_bitmap;

public:
	CDC(CBitmap* bitmap);

	void Render(HWND hWnd);
	void Render(HWND hWnd, long dx, long dy, float multiple);
};