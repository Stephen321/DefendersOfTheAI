#pragma once
#include <vector>
#include "AI.h"
#include "MutantStates.h"
#include "Laser.h"
#include "Astronaut.h"

class Mutant : public AI<Mutant>
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Mutant(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const std::shared_ptr<GameObject>& player,
		GameObjectPtrVector& gameProjectiles, GameObjectPtrVector& gameMutants);

	sf::Vector2f separation();
	sf::Vector2f swarm();
	void setAcceleration(const sf::Vector2f& acceleration);
	void move(float dt) override;
	int getNeighbourCount() const;
	float getForceAmount() const;
	void setDirection(const sf::Vector2f& dir);
	sf::Vector2f getDirection() const;
	void fire(float dt); 

private:
	//lasers
	const std::shared_ptr<GameObject> m_player;
	const float LASER_VEL_SCALE = 0.4f; 
	const float PLAYER_LASER_RANGE = 300.f;
	const float RELOAD_TIME = 0.75; 
	float m_reloadTimer;

	GameObjectPtrVector& m_gameProjectiles;
	GameObjectPtrVector& m_gameMutants;

	sf::Vector2f m_acceleration;

	const float NEIGHBOUR_RADIUS = 250.f;
	const float DESIRED_SEPARATION = 50.f;
	const float PLAYER_DESIRED_SEPARATION = 100.f;
	const float PLAYER_SEPERATION_FORCE_SCALE = 0.15f;
	const float PLAYER_FORCE_SCALER = 1.5f;

	const float SEPERATION_WEIGHT = 6.f;
	const float ALIGNMENT_WEIGHT = 1.1f;
	const float COHESION_WEIGHT = 1.2f;
};