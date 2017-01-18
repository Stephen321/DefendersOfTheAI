#pragma once
#include <vector>
#include "AI.h"
#include "AbductorStates.h"

class Abductor : public AI<Abductor>
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameAbductors);

	sf::Vector2f separation();
	sf::Vector2f alignment();
	sf::Vector2f cohesion();
	sf::Vector2f seek(const sf::Vector2f&  v);
	void update(float dt);
	void setAcceleration(const sf::Vector2f& acceleration);
	void move(float dt) override;
	int getNeighbourCount() const;
	void setYPosToWave();
	float getForceAmount() const;
	void setDirection(const sf::Vector2f& dir);
	sf::Vector2f getDirection() const;
	void resetMaxVelocity();
	void setMaxPatrolVelocity();
	void setReachedPatrolY(bool value);

	void checkWorldBounds() override;

private:
	float getWaveY();
	const float PATROL_MAX_VEL_SCALE = 0.2f;
	const float START_MAX_VEL;
	sf::Vector2f m_acceleration;
	GameObjectPtrVector& m_gameAbductors;
	const float PATROL_WAVE_LENGTH = 400.f;
	const float PATROL_TIMER_SCALE = 1000.f;
	float m_patrolWaveTimer;
	const float MATCHED_Y_THRESHOLD = 5.f;
	bool m_reachedPatrolY;
	const float NEIGHBOUR_RADIUS = 400.f;
	const float DESIRED_SEPARATION = 200.f;

	const float SEPERATION_WEIGHT = 2.f;
	const float ALIGNMENT_WEIGHT = 1.f; 
	const float COHESION_WEIGHT= 1.f;
};