#pragma once

#include "GameObject.h"

class Missile : public GameObject 
{
public: 
	Missile(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, sf::Vector2f& target, int& ownerMissileCount, float damage);
	void update(float dt) override;
	void checkWorldBounds() override;
	float getDamage() const;
private:
	const float VEL_DROP_STOP_SCALE = 0.4f;
	const float DROP_DIST = 50.f;
	const float TTL = 10.f;
	const float MIN_Y_VEL = 10.f;
	float m_angle;
	bool m_finishedDropping;
	sf::Vector2f& m_target;
	float m_startY;
	float m_liveTimer;
	int& m_ownerMissileCount;
	float m_damage;
};
