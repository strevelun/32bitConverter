#pragma once

#include "framework.h"

class DIBitmap
{
    HBITMAP         m_bitmap;
    HANDLE          m_hFile;
    BITMAPINFO* pBitmapInfo;
    LPDWORD hBitmap24;

    int width, height;

private:
    bool InitFile(LPCWSTR fileName);
    void Read24Bitmap(HDC hdc);

public:
    DIBitmap(HDC hdc, LPCWSTR fileName);
    ~DIBitmap() { DeleteObject(pBitmapInfo); }

    HBITMAP Convert24to32(HDC hdc);
    HBITMAP GetOriginalBitmap() { return m_bitmap; }
};

