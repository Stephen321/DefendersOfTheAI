#pragma once

#include <iostream> //testing

#include "SFML\Graphics.hpp"


class Screen
{
public:
	virtual int run(sf::RenderWindow &window) = 0;
};