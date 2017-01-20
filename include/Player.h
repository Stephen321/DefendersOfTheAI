#pragma once

#include <memory>
#include "GameObject.h"
#include "Laser.h"

class Player : public GameObject
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Player(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameProjecttiles);
	void update(float dt) override;

private:
	void checkInput();

	//lasers
	void fire(); //TODO: put this into a super class such as Character which AI and the Player both inheirit from
	void smartBomb();
	bool BombAvailable() const;
	void hyperJump();
	void replenishHyperJump();
	bool m_canHyperJump;
	bool m_bombReady;
	const float SMARTBOMB_COOLDOWN = 60.f;
	const float RELOAD_TIME = 0.1f;
	float m_smartBombTimer = 0;
	float m_reloadTimer;
	const float ANGLE_OFFSET = 180.f;
	GameObjectPtrVector& m_gameProjectiles;
};