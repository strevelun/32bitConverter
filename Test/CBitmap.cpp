#include "CBitmap.h"
#include "framework.h"

CBitmap::CBitmap(HWND hWnd, LPCWSTR fileName)
{
    m_hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        HDC hdc = GetDC(hWnd);
        BITMAPFILEHEADER    bmFileHeader;
        DWORD               dwReadBytes;

        ReadFile(m_hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwReadBytes, NULL);

        int iBitmapInfoSize = bmFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
        BITMAPINFO* pBitmapInfo = (BITMAPINFO*) new BYTE[iBitmapInfoSize];
        ReadFile(m_hFile, (LPVOID)pBitmapInfo, iBitmapInfoSize, &dwReadBytes, NULL);

        m_width = pBitmapInfo->bmiHeader.biWidth;
        m_height = pBitmapInfo->bmiHeader.biHeight;

        m_bitmap = CreateDIBSection(hdc, pBitmapInfo, DIB_RGB_COLORS, (void**)&m_pixels, NULL, 0);
        ReadFile(m_hFile, m_pixels, pBitmapInfo->bmiHeader.biSizeImage, &dwReadBytes, NULL);

        m_hMemDC = CreateCompatibleDC(hdc);
        SelectObject(m_hMemDC, m_bitmap);

        ReleaseDC(hWnd, hdc);
    }

    CloseHandle(m_hFile);
}

void CBitmap::Render(HWND hWnd)
{
    PAINTSTRUCT ps;

    HDC hdc2 = BeginPaint(hWnd, &ps);
    
	BitBlt(hdc2, 0, 0, m_width, m_height, m_hMemDC, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
}

void CBitmap::Render(HDC hdc, long dx, long dy, long dw, long dh)
{
	// StretchBlt();
}
