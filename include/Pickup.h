#pragma once
#include "GameObject.h"

class Pickup : public GameObject
{
public:
	Pickup(GameObject::Type type, const sf::Vector2f& worldSize);
	void update(float dt) override;
private:
	float m_fallingSpeed = 3;
};