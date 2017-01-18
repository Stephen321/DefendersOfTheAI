#pragma once

#include "GameObject.h"

class Laser : public GameObject
{
public:
	Laser(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const sf::Vector2f& direction, const sf::FloatRect& cameraBounds, float maxVelocityScale = 0.f);
	void update(float dt) override;
	void checkWorldBounds() override;
private:
	const float TTL = 2.f;
	float m_liveTimer;
};
