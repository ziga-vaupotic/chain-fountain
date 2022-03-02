#include "Render.h"
#include <windows.h>

#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Simulation.h"
#include "../Utils/Vector2D.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

namespace Simulation
{
    namespace Render
    {
        void RenderText(std::string m_strText, int X, int Y)
        {
            D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);
            RECT rct; //Font
            rct.left = X;
            rct.right = X + 700;
            rct.top = Y;
            rct.bottom = rct.top + 200;
            Simulation::Globals::m_pFont->DrawTextA(NULL, m_strText.c_str(), -1, &rct, 0, fontColor);
        }
        void DrawFillRect(IDirect3DDevice9* dev, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
        {
            D3DCOLOR rectColor = D3DCOLOR_XRGB(r, g, b);
            D3DRECT BarRect = { x, y, x + w, y + h };

            dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, rectColor, 0, 0);
        }
    }

    namespace Other
    {
        Vector2D GetWindowSize(HWND& m_pHWND)
        {
            RECT rect;
            if (GetWindowRect(m_pHWND, &rect))
            {
                return Vector2D(rect.right - rect.left, rect.bottom - rect.top);
            }
            return Vector2D(0, 0);
        }

    }

    namespace Calls
    {
        void OnInit(LPDIRECT3DDEVICE9& m_pD3DDev, HWND& m_pHWND)
        {
            Globals::m_vecScreenSize = Other::GetWindowSize(m_pHWND);
            D3DXCreateFontA(m_pD3DDev, 30, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, ("Arial"), &Simulation::Globals::m_pFont);
        }

        void OnPaint(LPDIRECT3DDEVICE9& m_pD3DDev)
        {
            const float m_flExtendFactor = Globals::m_vecScreenSize.y / (MaxY() + MaxY() / 100);
            m_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

            m_pD3DDev->BeginScene();

            OnPhyUpdate();

            Render::RenderText(std::string("Time: " + std::to_string(Globals::s / v())), 1200, 20);
            Render::RenderText(std::string("Path: " + std::to_string(Globals::s)), 1200, 50);
            Render::RenderText(std::string("Vel: " + std::to_string(v())), 1200, 80);
            Render::RenderText(std::string("MY/Y: " + std::to_string(MaxY() - SimulateY(Globals::s))), 1200, 110);

            Render::DrawFillRect(m_pD3DDev, SimulateX2(Globals::s) * 5 , Globals::m_vecScreenSize.y - SimulateY(Globals::s) * m_flExtendFactor, 5, 5, 255, 0, 0);

            for (int i = 0; i < Globals::m_pObjects.size(); i++)
            {
                Render::DrawFillRect(m_pD3DDev, SimulateX2(Globals::m_pObjects[i]) * 5, Globals::m_vecScreenSize.y - SimulateY(Globals::m_pObjects[i]) * m_flExtendFactor, 1, 1, 255 - i / Globals::m_pObjects.size(), 0, 0);
            }

            Render::DrawFillRect(m_pD3DDev, 0, Globals::m_vecScreenSize.y - STARTHEIGHT * m_flExtendFactor, (MaxY() - STARTHEIGHT) / tan(90 * PI / 180.0 - THETA * PI / 180.0), 5, 60,60,60);
            Render::DrawFillRect(m_pD3DDev, 0, Globals::m_vecScreenSize.y - MaxY() * m_flExtendFactor, 2000, 1, 60, 60, 60);
            Render::RenderText("MAX PREDICTED HEIGHT: " + std::to_string(MaxY() - STARTHEIGHT), 500, Globals::m_vecScreenSize.y - MaxY() * m_flExtendFactor);

            m_pD3DDev->EndScene();

            m_pD3DDev->Present(NULL, NULL, NULL, NULL);
        }
    }
}