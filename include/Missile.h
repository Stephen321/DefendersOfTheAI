#pragma once

#include "GameObject.h"

class Missile : public GameObject //TODO: use projectile superclass
{
public: //TODO: use pursue steering to aim at player (make Pursue/Flee/Seek/Evade classes instead of repeating code)
	Missile(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const sf::Vector2f& target, int& ownerMissileCount);
	void update(float dt) override;
	void checkWorldBounds() override;
private:
	const float DROP_DIST = 50.f;
	const float TTL = 2.f;
	const float MIN_Y_VEL = 10.f;
	float m_angle;
	bool m_finishedDropping;
	sf::Vector2f m_target;
	float m_startY;
	float m_liveTimer;
	int& m_ownerMissileCount;
};
