#include "Render.h"
#include <windows.h>
#include <random> 
#include <iomanip>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>
#include "Simulation.h"
#include "../Utils/Vector2D.h"
#include "../Utils/ConfigSystem.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

using namespace ConfigSystem::Variables;

namespace Simulation
{
    namespace Render
    {
        void RenderText(std::string m_strText, int X, int Y)
        {
            D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);
            RECT rct; //Font
            rct.left = X;
            rct.right = X + 1200;
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

    struct Color
    {
        unsigned char r, g, b;
        Color()
        {
            r = 255;
            g = 255;
            b = 255;
        }
        Color(unsigned char r, unsigned char g, unsigned char b)
        {
            r = r,
            g = g;
            b = b;
        }
    };

    namespace Calls
    {
        void OnInit(LPDIRECT3DDEVICE9& m_pD3DDev, HWND& m_pHWND)
        {
            Globals::m_vecScreenSize = Other::GetWindowSize(m_pHWND);
            D3DXCreateFontA(m_pD3DDev, 30, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, ("Comic Sans MS"), &Simulation::Globals::m_pFont);
        }

        std::vector<Color> GenerateRandomColorPairs(int m_nNumOfCalls)
        {
            std::vector<Color> m_pRetVector;
            std::random_device rd;

            std::mt19937 gen(rd());

            std::uniform_int_distribution<> dis(1, 255);

            for (int i = 0; i < m_nNumOfCalls; i++)
            {
                Color m_pColor = Color(0,0,0);
                m_pColor.r = dis(gen);
                m_pColor.g = dis(gen);
                m_pColor.b = dis(gen);
                m_pRetVector.push_back(m_pColor);
            }
            return m_pRetVector;
        }

        float m_flFindSmallest(std::vector<std::pair<float, float>>& m_pInputs, bool m_bSecond, bool m_bSmallest)
        {
            if (m_bSmallest)
            {
                float m_flMin = FLT_MAX;
                for (int i = 0; i < m_pInputs.size(); i++)
                {
                    if (!m_bSecond)
                    {
                        if (m_pInputs[i].first < m_flMin)
                        {
                            m_flMin = m_pInputs[i].first;
                        }
                    }
                    else
                    {
                        if (m_pInputs[i].second < m_flMin)
                        {
                            m_flMin = m_pInputs[i].second;
                        }
                    }
                }
                return m_flMin;
            }
            else
            {
                float m_flMax = -FLT_MAX;
                for (int i = 0; i < m_pInputs.size(); i++)
                {
                    if (!m_bSecond)
                    {
                        if (m_pInputs[i].first > m_flMax)
                        {
                            m_flMax = m_pInputs[i].first;
                        }
                    }
                    else
                    {
                        if (m_pInputs[i].second > m_flMax)
                        {
                            m_flMax = m_pInputs[i].second;
                        }
                    }
                }
                return m_flMax;
            }
        }

        std::string VectorToString(std::vector<std::pair<float,float>> m_pInputs)
        {
            std::string m_strReturn;

            //m_strReturn += "[";
            
            //for (int i = 0; i < m_flInputs.size(); i++)
            //{
            //    m_strReturn += "[";
            //    std::stringstream ss;
            //    ss << std::fixed << std::setprecision(2) << m_flInputs[i].first;

            //    std::stringstream ss2;
            //    ss2 << std::fixed << std::setprecision(2) << m_flInputs[i].second;

            //    m_strReturn += ss.str() + "m ," + ss2.str() + "s^-2]" + (i != m_flInputs.size() - 1 ? "," : "");
            //}

            std::stringstream ss[4];
            ss[0] << std::fixed << std::setprecision(2) << m_flFindSmallest(m_pInputs, false, true);
            ss[1] << std::fixed << std::setprecision(2) << m_flFindSmallest(m_pInputs, false, false);
            ss[2] << std::fixed << std::setprecision(2) << m_flFindSmallest(m_pInputs, true, true);
            ss[3] << std::fixed << std::setprecision(2) << m_flFindSmallest(m_pInputs, true, false);

            m_strReturn += "[[" + ss[0].str() + "," + ss[1].str() + "]m,[" + ss[2].str() + "," + ss[3].str() + "]s^-2]";

            return m_strReturn;
        }

        void RecursiveSimulastionCall(LPDIRECT3DDEVICE9& m_pD3DDev, const float m_flExtendFactor, int m_nNumOfCalls)
        {
            std::vector<std::pair<float,float>> m_flMaXHeights;
            static std::vector<Color> m_pRandomColors = GenerateRandomColorPairs(m_nNumOfCalls);
             
            int m_nColor = 0;

            for (float i = BETA - K; i < BETA + K; i+=S)
            {
                float acc = BETA;
                BETA = i;

                m_nColor++;

                Render::DrawFillRect(m_pD3DDev, SimulateX2(Globals::s) * 5, Globals::m_vecScreenSize.y - SimulateY(Globals::s) * m_flExtendFactor, 5, 5, m_pRandomColors[m_nColor].r, m_pRandomColors[m_nColor].g, m_pRandomColors[m_nColor].b);
                if (m_nColor == 1)
                {
                    Render::RenderText("s: " + std::to_string(Globals::s) + "m y:" + std::to_string(SimulateY(Globals::s)) + "m", SimulateX2(Globals::s) * 5, Globals::m_vecScreenSize.y - SimulateY(Globals::s) * m_flExtendFactor);
                }


                for (int j = 0; j < Globals::m_pObjects.size(); j++)
                {
                    Render::DrawFillRect(m_pD3DDev, SimulateX2(Globals::m_pObjects[j]) * 5, Globals::m_vecScreenSize.y - SimulateY(Globals::m_pObjects[j]) * m_flExtendFactor, 1, 1, m_pRandomColors[m_nColor].r, m_pRandomColors[m_nColor].g, m_pRandomColors[m_nColor].b);
                }

                Render::DrawFillRect(m_pD3DDev, 0, Globals::m_vecScreenSize.y - STARTHEIGHT * m_flExtendFactor, (MaxY() - STARTHEIGHT) / tan(90 * PI / 180.0 - THETA * PI / 180.0), 5, m_pRandomColors[m_nColor].r, m_pRandomColors[m_nColor].g, m_pRandomColors[m_nColor].b);
                Render::DrawFillRect(m_pD3DDev, 0, Globals::m_vecScreenSize.y - MaxY() * m_flExtendFactor, 2000, 1, m_pRandomColors[m_nColor].r, m_pRandomColors[m_nColor].g, m_pRandomColors[m_nColor].b);

                m_flMaXHeights.push_back(std::make_pair(MaxY() - STARTHEIGHT, BETA));

                BETA = acc;
            }

            Render::RenderText("MAX PREDICTED HEIGHT: " + VectorToString(m_flMaXHeights), 500, Globals::m_vecScreenSize.y - MaxY() * m_flExtendFactor);
          }

        void OnPaint(LPDIRECT3DDEVICE9& m_pD3DDev, float m_flPrevFPS)
        {
            const float m_flExtendFactor = Globals::m_vecScreenSize.y / (MaxY() + MaxY() / 100);
            m_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

            m_pD3DDev->BeginScene();

            OnPhyUpdate(m_flPrevFPS);

            Render::RenderText(std::string("Time: " + std::to_string(Globals::s / v())), 1200, 20);
            Render::RenderText(std::string("Path: " + std::to_string(Globals::s)), 1200, 50);
            Render::RenderText(std::string("Vel: " + std::to_string(v())), 1200, 80);
            Render::RenderText(std::string("MY/Y: " + std::to_string(MaxY() - SimulateY(Globals::s))), 1200, 110);
            Render::RenderText(std::string("Alpha: " + std::to_string(Alpha())), 1200, 140);
            Render::RenderText(std::string("FPS: " + std::to_string(m_flPrevFPS)), 1200, 170);

            RecursiveSimulastionCall(m_pD3DDev, m_flExtendFactor, (K*2) / S + 2);

            m_pD3DDev->EndScene();

            m_pD3DDev->Present(NULL, NULL, NULL, NULL);
        }
    }
}