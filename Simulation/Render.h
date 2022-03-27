#pragma once
#include <windows.h>

#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include "../Utils/Vector2D.h"
#pragma comment (lib, "d3d9.lib")

namespace Simulation
{
    namespace Globals
    {
        inline LPD3DXFONT m_pFont = NULL;
        inline Vector2D m_vecScreenSize(0, 0);
    }
    namespace Calls
    {
        void OnInit(LPDIRECT3DDEVICE9& m_pD3DDev, HWND& m_pHWND);
        void OnPaint(LPDIRECT3DDEVICE9& m_pD3DDev, float m_flPrevFPS);
    }
    namespace Render
    {
        void RenderText(std::string m_strText, int X, int Y);
        void DrawFillRect(IDirect3DDevice9* dev, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
    }
}