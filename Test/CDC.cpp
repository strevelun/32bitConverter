#include "CDC.h"
#include "CBitmap.h"

CDC::CDC(CBitmap* bitmap)
{
	m_bitmap = bitmap;
}

void CDC::Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	BitBlt(hdc, 0, 0, m_bitmap->GetWidth(), m_bitmap->GetHeight(), m_bitmap->GetMemDC(), 0, 0, SRCCOPY);

	EndPaint(hWnd, &ps);
}

void CDC::Render(HWND hWnd, long dx, long dy, float multiple)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	int width = m_bitmap->GetWidth();
	int height = m_bitmap->GetHeight();

	StretchBlt(hdc, dx, dy, width * multiple, height * multiple, m_bitmap->GetMemDC(), 0, 0, width, height, SRCCOPY);

	EndPaint(hWnd, &ps);
}
