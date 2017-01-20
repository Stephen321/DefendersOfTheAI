#pragma once

#include "GameObject.h"

class Laser : public GameObject
{
public:
	Laser(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const sf::Vector2f& direction, float damage, GameObject::Type ownerType, float maxVelocityScale = 0.f);
	void update(float dt) override;
	void checkWorldBounds() override;
	float getDamage() const;
	GameObject::Type ownerType() const;
private:
	Type m_ownerType;
	const float TTL = 2.f;
	float m_liveTimer;
	float m_damage;
};
