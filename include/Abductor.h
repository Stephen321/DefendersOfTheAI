#pragma once
#include <vector>
#include "AI.h"
#include "AbductorStates.h"
#include "Laser.h"

class Abductor : public AI<Abductor>
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameAbductors,
			 std::shared_ptr<GameObject> player, GameObjectPtrVector& gameProjectiles);

	sf::Vector2f separation();
	sf::Vector2f alignment();
	sf::Vector2f cohesion();
	sf::Vector2f seek(const sf::Vector2f&  v);
	void setAcceleration(const sf::Vector2f& acceleration);
	void move(float dt) override;
	int getNeighbourCount() const;
	float getForceAmount() const;
	void setDirection(const sf::Vector2f& dir);
	sf::Vector2f getDirection() const;
	void updateDropAcceleration();
	void updatePatrolAcceleration();
	void fire(float dt); //TODO: and put this into that fire capable interface
	bool reachedTarget() const;

	void checkWorldBounds() override;

private:
	//lasers
	const std::shared_ptr<GameObject> m_player;
	const float LASER_VEL_SCALE = 0.4f; //abductor lasers travel slower than players so the player gets a chance to avoid them
	const float PLAYER_LASER_RANGE = 500.f;
	const float RELOAD_TIME = 1.5f; //TODO put this reload time and timer in an interface
	float m_reloadTimer;

	GameObjectPtrVector& m_gameProjectiles;
	GameObjectPtrVector& m_gameAbductors;

	sf::Vector2f m_acceleration;
	float m_targetInZone;
	const float ARRIVE_RADIUS = 400.f;
	const float TARGET_RANGE = 25.f;
	const float TIME_TO_TARGET = 0.005f;
	const float HIGHEST_DISTANCE;
	bool m_reachedTarget;

	const float NEIGHBOUR_RADIUS = 400.f;
	const float DESIRED_SEPARATION = 70.f;
	const float PLAYER_DESIRED_SEPARATION = 100.f;
	const float PLAYER_SEPERATION_FORCE_SCALE = 0.25f;

	const float SEPERATION_WEIGHT = 5.f;
	const float ALIGNMENT_WEIGHT = 1.5f; 
	const float COHESION_WEIGHT= 1.5f;
};