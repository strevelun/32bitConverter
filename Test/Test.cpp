// Test.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Test.h"
#include <random>

#define HEIGHT      500
#define WIDTH       500

BITMAPINFO          bmpInfo;
LPDWORD             lpPixel;
HBITMAP             hBitmap;
HDC                 hMemDC;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

    MSG msg;



    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TEST);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}



void SetDib(HWND hWnd)
{
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = WIDTH;
    bmpInfo.bmiHeader.biHeight = HEIGHT;

    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(hWnd);
    //hBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
    //   bmpInfo + pixel data ( width * height * bitCount / 8  )
    //   0, 0 위치  왼쪽 하단

    HANDLE hFile;
    hFile = CreateFile(L"image.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return;

    BITMAPFILEHEADER bmFileHeader;
    DWORD dwReadBytes;

    ReadFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwReadBytes, NULL);

    int iBitmapInfoSize = bmFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
    BITMAPINFO* pBitmapInfo = (BITMAPINFO*) new BYTE[iBitmapInfoSize];
    ReadFile(hFile, (LPVOID)pBitmapInfo, iBitmapInfoSize, &dwReadBytes, NULL);

    LPVOID lpDIBits;
    HBITMAP hBitmap = CreateDIBSection(hdc, pBitmapInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
    ReadFile(hFile, lpPixel, pBitmapInfo->bmiHeader.biSizeImage, &dwReadBytes, NULL);

    CloseHandle(hFile);

    LPDWORD lpDestPixel;
    pBitmapInfo->bmiHeader.biBitCount = 32; 
    HBITMAP hBitmap2 = CreateDIBSection(hdc, pBitmapInfo, DIB_RGB_COLORS, (void**)&lpDestPixel, NULL, 0);

    int cnt = 0;
    int loc = 0;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int temp = 0;
            for (int i = loc + 2; i >= loc; i--)
            {
                if (i >= loc + 2)
                {
                    temp |= 0xff;
                    temp <<= 8;
                }
                temp |= ((LPBYTE)lpPixel)[i];

                if(i != loc)
                    temp <<= 8;
            }
            (lpDestPixel)[cnt++] = temp;
            loc += 3;
        }
    }
    
    hMemDC = CreateCompatibleDC(hdc);
    SelectObject(hMemDC, hBitmap2);
    ReleaseDC(hWnd, hdc);
}

void DestroyDib()
{
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);
}

void DrawDib()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int random;

    for (int y = HEIGHT - 1 ; y >= 0; y--)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            std::uniform_int_distribution<int> d1(0, 0x00ffffff - 1);
            random = d1(gen);
            lpPixel[(y * WIDTH) + x] = random;
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 100, NULL);
        SetDib(hWnd);
        break;

    case WM_TIMER:
       // DrawDib();
        InvalidateRgn(hWnd, NULL, FALSE);
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hMemDC, 0, 0, SRCCOPY);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DestroyDib();
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

