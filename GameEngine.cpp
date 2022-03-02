
#include <windows.h>

#include <iostream>
#include <d3d9.h>
#include "Simulation/Render.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

LPDIRECT3D9 m_pD3D;
LPDIRECT3DDEVICE9 m_pD3DDev;


void initD3D(HWND hWnd)
{
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;   

    ZeroMemory(&d3dpp, sizeof(d3dpp));   
    d3dpp.Windowed = TRUE; 
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;   
    d3dpp.hDeviceWindow = hWnd;  


    m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &m_pD3DDev);
}

void cleanD3D(void)
{
    m_pD3DDev->Release();
    m_pD3D->Release();
}





LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("Nigga");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("Nigga"),   // window class name
        TEXT("Nigga Simulation"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    initD3D(hWnd);
    Simulation::Calls::OnInit(m_pD3DDev, hWnd);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_CREATE:
        AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
        return 0;
    case WM_PAINT:
        Simulation::Calls::OnPaint(m_pD3DDev);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc