#pragma once
#include <vector>
namespace Simulation
{

#define ALPHA 0.12
#define BETA 0.11
#define THETA 75
#define VAL 0.001
#define STARTHEIGHT 2
#define PI 3.14159265
#define G 9.81
#define EXTENDFACTOR 30
#define STARTY 1000


	namespace Calls
	{
		void OnPhyUpdate();
	}

	namespace Globals
	{
		inline float s = 0.f;
		inline std::vector<float> m_pObjects;
	}
	float v();
	float SimulateX(float s);
	float MaxY();
	float SimulateX2(float s);
	float SimulateY(float s);
}