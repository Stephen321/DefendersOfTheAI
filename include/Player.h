#pragma once

#include <memory>
#include "GameObject.h"
#include "Laser.h"
#include "Missile.h"
#include "HealthBar.h"
#include "Constants.h"

class Player : public GameObject
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Player(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameProjecttiles);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool collision(const std::shared_ptr<GameObject>& collidor);

private:
	void checkInput();

	//lasers
	void fire(); 
	const float RELOAD_TIME = 0.1f;
	float m_reloadTimer;
	const float ANGLE_OFFSET = 180.f;
	GameObjectPtrVector& m_gameProjectiles;
	HealthBar m_healthBar;
	const float HEALTH_Y_OFFSET = 30.f;


	const float DAMAGE = 10.f;
};