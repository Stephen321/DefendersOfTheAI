#pragma once

#include "SFML\Graphics.hpp"

namespace Helpers
{
	//testing: inline or seperate definition and declaration so not created multiple times (Graphics.hpp also included multiple times?)


	//clamps a value between min and max-1
	inline int clamp(int value, int min, int max)
	{
		if (value < 0)
		{
			return max - 1;
		}
		if (value >= max)
		{
			return 0;
		}

		return value;
	}

	inline float getLength(const sf::Vector2f & v)
	{
		float length = sqrt(v.x * v.x + v.y * v.y);
		return length;
	}

	inline sf::Vector2f normaliseCopy(const sf::Vector2f & v)
	{
		sf::Vector2f n;
		float length = getLength(v);
		if (length != 0)
		{
			n.x = v.x / length;
			n.y = v.y / length;
		}
		return n;
	}

	inline void normalise(sf::Vector2f & v)
	{
		v = normaliseCopy(v);
	}
	
	inline float randomNumberF(float min, float max)
	{
		return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
	}

	inline int randomNumber(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}
}