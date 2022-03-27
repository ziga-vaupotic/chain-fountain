#include <iostream>
#include <math.h>
#include <cstdint>
#include <algorithm>
#include <algorithm>
#include <iomanip>
#include "Simulation.h"
#include "../Utils/ConfigSystem.h"
#include "../Utils/Vector2D.h"


using namespace ConfigSystem::Variables;

namespace Simulation
{
    namespace Calls
    {
        void OnPhyUpdate(float m_flFPS)
        {
            if (m_flFPS == 0.f)
            {
                return;
            }

            Globals::m_pObjects.push_back(Globals::s);
            Globals::s += VAL * (1.0f / m_flFPS);
            std::cout << std::max(0.f, SimulateY(Globals::s)) << " : " << SimulateX(Globals::s) << " t: " << Globals::s / v() << std::endl;
        }
    }



    float ky()
    {
        return cos(90 * PI / 180.0 - THETA * PI / 180.0) * ((ALPHA * STARTHEIGHT) / (1 - ALPHA - BETA));
    }

    float kx()
    {
        return sqrt(pow((ALPHA * STARTHEIGHT) / (1 - ALPHA - BETA), 2) - pow(ky(), 2));
    }


    float v()
    {
        return sqrt((G * STARTHEIGHT) / (1 - ALPHA - BETA));
    }

    float SimulateX(float s)
    {
        float k_X = kx();
        float k_Y = ky();
        return std::max((double)0, k_X * log((sqrt(pow(k_X, 2) + pow(k_Y - s, 2)) - k_Y + 2) / (sqrt(pow(k_X, 2) + pow(k_Y, 2)) - k_Y)));
    }


    float SimulateX2(float s)
    {
        return abs(s * 50);
    }

    float MomentofInertia()
    {
        float mass = (MASS / 3);
        float powfbhalf = powf(BHALFS, 2);
        return (mass * powfbhalf) / 6;
    }

    float a()
    {
        return (BHALFS / 2) * ACCELERATION;
    }

    float Fc()
    {
        return 0.5 * MASS * a() * 0.3f;
    }

    float Alpha()
    {
        return 1.f/6.f;
    }

    float MaxY()
    {
        return ((STARTHEIGHT * (1 - BETA)) / (1 - Alpha() - BETA));
    }

    float SimulateY(float s)
    {
        return ((STARTHEIGHT * (1 - BETA)) / (1 - Alpha() - BETA) - abs(cos(90 * PI / 180.0 - THETA * PI / 180.0) * ((Alpha() * STARTHEIGHT) / (1 - Alpha() - BETA)) - s));
    }


    float Interpolate(float y0, float y, float t)
    {
        return y0 + t * (y - y0);
    }

}