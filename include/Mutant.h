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
	typedef std::unordered_map<std::string, GameObjectPtrVector> GameObjectMap;
	Mutant(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, std::shared_ptr<GameObject> player, GameObjectMap& gameObjectsRef);

	sf::Vector2f separation();
	sf::Vector2f swarm();
	sf::Vector2f seek();
	void checkWorldBounds() override;
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

	GameObjectMap& m_gameObjectsRef;

	sf::Vector2f m_acceleration;

	const float NEIGHBOUR_RADIUS = 250.f;
	const float DESIRED_SEPARATION = 50.f;
	const float PLAYER_DESIRED_SEPARATION = 100.f;
	const float PLAYER_SEPERATION_FORCE_SCALE = 0.05f;
	const float PLAYER_FORCE_SCALER = 3.f;

	const float PLAYER_SEEK_FORCE_SCALE = 0.5f;

	const float SEPERATION_WEIGHT = 6.f;
	const float SWARM_WEIGHT = 1.f;
	const float SEEK_WEIGHT = 1.f;

	const float DAMAGE = 10.f;
};