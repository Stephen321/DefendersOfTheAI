#pragma once

#include <memory>
#include "GameObject.h"
#include "Laser.h"

class Player : public GameObject
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Player(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameProjecttiles, const sf::FloatRect& cameraBounds);
	void update(float dt) override;

private:
	void checkInput();

	//lasers
	void fire(); //TODO: put this into a super class such as Character which AI and the Player both inheirit from
	const float RELOAD_TIME = 0.1f;
	float m_reloadTimer;
	GameObjectPtrVector& m_gameProjectiles;
};