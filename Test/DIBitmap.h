#pragma once

#include "framework.h"

class DIBitmap
{
    HBITMAP         m_bitmap;
    HANDLE          m_hFile;
    BITMAPINFO*     m_pBitmapInfo;
    LPDWORD         m_hBitmap24;

    int             m_width, m_height;

private:
    bool InitFile(LPCWSTR fileName);
    void Read24Bitmap(HDC hdc);

public:
    DIBitmap(HDC hdc, LPCWSTR fileName);
    ~DIBitmap() { DeleteObject(m_pBitmapInfo); }

    HBITMAP Convert24to32(HDC hdc);
    HBITMAP GetOriginalBitmap() { return m_bitmap; }
};

