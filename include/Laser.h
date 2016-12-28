#pragma once

#include "GameObject.h"

class Laser : public GameObject
{
public:
	Laser(const sf::Vector2f& startPos, const sf::Texture& texture, const sf::Vector2f& startVelocity);
};
