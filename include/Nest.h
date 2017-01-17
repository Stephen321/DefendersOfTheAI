#pragma once
#include "AI.h"
#include "NestStates.h"
#include "Missile.h"

class Nest : public AI<Nest>
{
public:
	Nest(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, std::shared_ptr<GameObject> player, std::vector<std::shared_ptr<GameObject>>& gameProjecttiles);
	void setTargetPos(const sf::Vector2f& target);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool checkIfReachedTarget();
	void getWanderTarget();
	bool playerInRange() const;
	void evade();
	void fire();
private:
	const std::shared_ptr<GameObject> m_player;
	float m_wanderOrientation;
	const float PLAYER_IN_RANGE = 400.f;
	const float MIN_TARGET_DIST = 30.f;

	const float ANGLE_CHANGE = 30.f;
	const float WANDER_OFFSET = 150.f;
	const float WANDER_RADIUS = 50.f;

	const float MAX_PREDICTON = 0.3f;
	sf::Vector2f m_targetPos;

	//missiles
	const int MAX_MISSILES_ALIVE = 2;
	int m_missilesAlive;
	const float RELOAD_TIME = 0.8f;
	float m_reloadTimer;
	std::vector<std::shared_ptr<GameObject>>& m_gameProjectiles;

	sf::CircleShape testCircle;
	sf::CircleShape testCircle2;
	sf::CircleShape testCircle3;
	sf::CircleShape testCircle4;
	sf::CircleShape testCircle5;
};