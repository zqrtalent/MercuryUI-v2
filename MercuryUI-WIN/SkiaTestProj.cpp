// SkiaTestProj.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SkiaTestProj.h"

#include "SkPaint.h"
#include "SkString.h"

#include "SkData.h"
#include "SkImage.h"
#include "SkStream.h"
#include "SkSurface.h"
#include "SkCanvas.h"

using namespace std;
#include <vector>

#include "PokerLobbyDlg.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SKIATESTPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	PokerLobbyDlg dlg;
	dlg.Create();

	/**/
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SKIATESTPROJ));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SKIATESTPROJ));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SKIATESTPROJ);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void drawCanvas(SkCanvas* canvas)
{
	canvas->clear(SK_ColorWHITE);

	SkPaint paint1, paint2, paint3;

	paint1.setTextSize(64.0f);
	paint1.setAntiAlias(true);
	paint1.setColor(SkColorSetRGB(255, 0, 0));
	paint1.setStyle(SkPaint::kFill_Style);

	paint2.setTextSize(64.f);
	paint2.setAntiAlias(true);
	paint2.setColor(SkColorSetRGB(0, 136, 0));
	paint2.setStyle(SkPaint::kStroke_Style);
	paint2.setStrokeWidth(SkIntToScalar(3));

	paint3.setTextSize(64.0f);
	paint3.setAntiAlias(true);
	paint3.setColor(SkColorSetRGB(136, 136, 136));
	paint3.setTextScaleX(SkFloatToScalar(1.5f));

	const char text[] = "Skia!";
	
	canvas->save();
	SkMatrix m;
	m.setIDiv(1.0f, -1.0f);
	canvas->setMatrix(m);
	canvas->drawText(text, strlen(text), 20.0f, -100.0f, paint1);
	canvas->restore();

	canvas->drawText(text, strlen(text), 20.0f, 144.0f, paint2);
	canvas->drawText(text, strlen(text), 20.0f, 224.0f, paint3);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			
			int width = (ps.rcPaint.right - ps.rcPaint.left), height = (ps.rcPaint.bottom - ps.rcPaint.top);
			
			HDC memDC = ::CreateCompatibleDC(NULL);
			BITMAPINFOHEADER bmpInfo32;
			memset(&bmpInfo32, 0, sizeof(BITMAPINFOHEADER));
			bmpInfo32.biBitCount = 32;
			bmpInfo32.biCompression = BI_RGB;
			bmpInfo32.biPlanes = 1;
			bmpInfo32.biHeight = height;
			bmpInfo32.biSize = sizeof(BITMAPINFOHEADER);
			bmpInfo32.biWidth = width;

			void*   lpMap = NULL;
			HBITMAP hDib = ::CreateDIBSection(memDC, (BITMAPINFO*)&bmpInfo32, DIB_RGB_COLORS, &lpMap, NULL, 0);
			if (hDib) {
				::SelectObject(memDC, hDib);
				SkImageInfo imageInfo;
				imageInfo.fAlphaType = SkAlphaType::kOpaque_SkAlphaType;
				imageInfo.fColorType = SkColorType::kN32_SkColorType;
				imageInfo.fHeight = height;
				imageInfo.fWidth = width;

				SkSurface* rasterSurface = SkSurface::NewRasterDirect(imageInfo, lpMap, width* 4);
				SkCanvas* rasterCanvas = rasterSurface->getCanvas();
				drawCanvas(rasterCanvas);

				//rasterCanvas->unref();
				rasterSurface->unref();

				::BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY|NOMIRRORBITMAP);
				::DeleteDC(memDC);
			}
			/*
			SkImageInfo info = SkImageInfo::MakeN32(width, height, SkAlphaType::kOpaque_SkAlphaType);
			size_t rowBytes = info.minRowBytes();
			size_t size = info.getSafeSize(rowBytes);

			std::vector<char> pixelMemory(size);  // allocate memory
			SkSurface* surface = SkSurface::NewRasterDirect(info, &pixelMemory[0], rowBytes);
			SkCanvas* canvas = surface->getCanvas();

			int sizess = width*height * 4;

			drawCanvas(canvas);

			SkImage* img = surface->newImageSnapshot();
			if (img) {
				SkData* png = img->encode();
				if (png) {

					SkFILEWStream out("C:\\Users\\ZqrTalent\\Desktop\\Dev\\11.png");
					(void)out.write(png->data(), png->size());
				}
				
			}
			

			surface->unref();
			*/
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
