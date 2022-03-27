#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <Shlwapi.h>
#include <string>
#include <io.h>
#include <ShlObj.h>
#include "ConfigSystem.h"
#include <tchar.h>

namespace ConfigSystem
{

	void SetupValue(float& value, float def, std::string category, std::string name)
	{
		value = def;
		m_pFloats.push_back(new ConfigValue< float >(category, name, &value));
	}

	static TCHAR m_pPath[260];
	void Init()
	{

		GetModuleFileName(NULL, m_pPath, 260);

		SetupValue(Variables::ALPHA, 0.12, "Simulation", "ALPHA");
		SetupValue(Variables::ALPHA, 0.12, "Simulation", "ALPHA");
		SetupValue(Variables::BETA, 0.11, "Simulation", "BETA");
		SetupValue(Variables::G, 9.81, "Simulation", "G");
		SetupValue(Variables::STARTHEIGHT, 2, "Simulation", "STARTHEIGHT");
		SetupValue(Variables::THETA, 90, "Simulation", "THETA");
		SetupValue(Variables::VAL, 0.01, "Simulation", "VAL");
		SetupValue(Variables::ACCELERATION, 2, "Simulation", "ACCELERATION");
		SetupValue(Variables::MASS, 2, "Simulation", "MASS");
		SetupValue(Variables::BHALFS, 0.2, "Simulation", "BHALFS");
		SetupValue(Variables::VELOCITY, 0.2, "Simulation", "VELOCITY");
		SetupValue(Variables::MU, 0.0002, "Simulation", "MU");
		SetupValue(Variables::K, 0.0002, "Simulation", "K");
		SetupValue(Variables::S, 0.0002, "Simulation", "S");

		Load();
	}

	bool FileExists(const std::string& name) {
		std::ifstream f(name.c_str());
		return f.good();
	}
	void Save()
	{
		std::string m_strFile = std::string(m_pPath) + "\param.ini";

		for (auto value : m_pFloats)
		{
			WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), m_strFile.c_str());
		}

		std::cout << GetLastError() << std::endl;

	}

	void Load()
	{
		std::string m_strFile = std::string(m_pPath) + "\param.ini";

		if (!FileExists(m_strFile))
		{
			Save();
			return;
		}

		char value_l[32] = { '\0' };

		for (auto value : m_pFloats)
		{
			GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, m_strFile.c_str());
			*value->value = atof(value_l);
		}
	}
}