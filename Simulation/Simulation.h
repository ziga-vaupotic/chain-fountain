#pragma once
#include <vector>
namespace Simulation
{

#define PI 3.14159265
#define EXTENDFACTOR 30
#define STARTY 1000


	namespace Calls
	{
		void OnPhyUpdate(float m_flFPS);
	}

	namespace Globals
	{
		inline float s = 0.f;
		inline std::vector<float> m_pObjects;
	}
	float v();
	float SimulateX(float s);
	float Alpha();
	float MaxY();
	float SimulateX2(float s);
	float SimulateY(float s);
	float Interpolate(float y0, float y, float t);
}