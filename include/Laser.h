#pragma once

#include "GameObject.h"

class Laser : public GameObject
{
public:
	Laser(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const sf::Vector2f& direction);
	void update(float dt) override;
	void checkWorldBounds() override;
private:
	const float TTL = 2.f;
	float m_liveTimer;
};
