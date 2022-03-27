#pragma once
#include <vector>
namespace ConfigSystem
{
	namespace Variables
	{
		inline float ALPHA = 0.12;
		inline float BETA = 0.11;
		inline float THETA = 75;
		inline float VAL = 0.01;
		inline float STARTHEIGHT = 59;
		inline float G = 9.81;
		inline float ACCELERATION = 0;
		inline float MASS = 0;
		inline float BHALFS = 0.2;
		inline float VELOCITY = 2;
		inline float MU = 0.0008;
		inline float K = 0.01;
		inline float S = 0.01;
	}


	template< typename T >
	class ConfigValue
	{
	public:
		ConfigValue(std::string category_, std::string name_, T* value_)
		{
			category = category_;
			name = name_;
			value = value_;
		}

		std::string category, name;
		T* value;
	};

	inline std::vector< ConfigValue< float >* > m_pFloats;
	void Init();
	void Load();
}