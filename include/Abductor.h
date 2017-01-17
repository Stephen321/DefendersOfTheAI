#pragma once
#include <vector>
#include "AI.h"
#include "AbductorStates.h"

class Abductor : public AI<Abductor>
{
public:
	Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, std::vector<std::shared_ptr<Abductor>>& abductors);

	sf::Vector2f separation();
	sf::Vector2f alignment();
	sf::Vector2f cohesion();
	sf::Vector2f seek(const sf::Vector2f&  v);
	void update(float dt);
	float angle(const sf::Vector2f& v);
	void setAcceleration(const sf::Vector2f& acceleration);
	void checkBounds();
	void move(float dt) override;
	int getNeighbourCount() const;
	void setYPosWave();
	float getForceAmount() const;
	void setDirection(const sf::Vector2f& dir);
	sf::Vector2f getDirection() const;
	void resetMaxVelocity();
	void setMaxPatrolVelocity();

private:
	const float PATROL_MAX_VEL_SCALE = 0.2f;
	const float START_MAX_VEL;
	sf::Vector2f m_acceleration;
	std::vector<std::shared_ptr<Abductor>>& m_abductors;
	float m_patrolWaveTimer;
	const float LOWEST_DISTANCE;
	const float NEIGHBOUR_RADIUS = 400.f;
	const float DESIRED_SEPARATION = 200.f;

	const float SEPERATION_WEIGHT = 2.f;
	const float ALIGNMENT_WEIGHT = 1.f; 
	const float COHESION_WEIGHT= 1.f;
};