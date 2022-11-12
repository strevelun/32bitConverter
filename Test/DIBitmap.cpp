#include "DIBitmap.h"

DIBitmap::DIBitmap(HDC hdc, LPCWSTR fileName)
{
    if (InitFile(fileName))
    {
        Read24Bitmap(hdc);
    }
}

bool DIBitmap::InitFile(LPCWSTR fileName)
{
    m_hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        m_hFile = NULL;
        return false;
    }
    return true;
}

void DIBitmap::Read24Bitmap(HDC hdc)
{
    BITMAPFILEHEADER    bmFileHeader;
    DWORD               dwReadBytes;

    ReadFile(m_hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwReadBytes, NULL);

    int iBitmapInfoSize = bmFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
    pBitmapInfo = (BITMAPINFO*) new BYTE[iBitmapInfoSize];
    ReadFile(m_hFile, (LPVOID)pBitmapInfo, iBitmapInfoSize, &dwReadBytes, NULL);

    width = pBitmapInfo->bmiHeader.biWidth;
    height = pBitmapInfo->bmiHeader.biHeight;

    m_bitmap = CreateDIBSection(hdc, pBitmapInfo, DIB_RGB_COLORS, (void**)&hBitmap24, NULL, 0);
    ReadFile(m_hFile, hBitmap24, pBitmapInfo->bmiHeader.biSizeImage, &dwReadBytes, NULL);

    CloseHandle(m_hFile);
}

HBITMAP DIBitmap::Convert24to32(HDC hdc)
{
    LPDWORD lpDestPixel;
    HBITMAP bitmap32;
    pBitmapInfo->bmiHeader.biBitCount = 32;
    bitmap32 = CreateDIBSection(hdc, pBitmapInfo, DIB_RGB_COLORS, (void**)&lpDestPixel, NULL, 0);
   
    int originCnt = 0;
    int destCnt = 0;

    BITMAP bitmap;
    GetObject(m_bitmap, sizeof(BITMAP), &bitmap);

    for (int y = 0; y < height; y++)
    {
        originCnt = (y * bitmap.bmWidthBytes);
        for (int x = 0; x < width; x++)
        {
            ((LPBYTE)lpDestPixel)[destCnt++] = ((LPBYTE)hBitmap24)[originCnt++];
            ((LPBYTE)lpDestPixel)[destCnt++] = ((LPBYTE)hBitmap24)[originCnt++];
            ((LPBYTE)lpDestPixel)[destCnt++] = ((LPBYTE)hBitmap24)[originCnt++];
            ((LPBYTE)lpDestPixel)[destCnt++] = 0xff;
        }
    }

    /*
    int cnt = 0;
    int loc = 0;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int temp = 0;
            for (int i = loc + 2; i >= loc; i--)
            {
                if (i >= loc + 2)
                {
                    temp |= 0xff;
                    temp <<= 8;
                }
                temp |= ((LPBYTE)hBitmap24)[i];

                if (i != loc)
                    temp <<= 8;
            }
            lpDestPixel[cnt++] = temp;
            loc += 3;
        }
    }
    */

    return bitmap32;
}