#pragma once
#include "AI.h"
#include "NestStates.h"
#include "Missile.h"
#include "Abductor.h"

class Nest : public AI<Nest>
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	typedef std::unordered_map<std::string, GameObjectPtrVector> GameObjectMap;
	Nest(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, std::shared_ptr<GameObject> player, GameObjectMap& gameObjectsRef);
	void setTargetPos(const sf::Vector2f& target);
	bool checkIfReachedTarget();
	void getWanderTarget();
	bool playerInRange() const;
	void evade();
	void produceAbductors(float dt);
	void fire(float dt);
	void setPlayerPos();
	void setProduceTimer(float value);
	void checkWorldBounds() override;
private:
	const std::shared_ptr<GameObject> m_player;
	float m_wanderOrientation;
	const float PLAYER_EVADE_RANGE = 400.f;
	const float PLAYER_MISSILE_RANGE = 700.f;
	const float MIN_TARGET_DIST = 30.f;

	const float ANGLE_CHANGE = 30.f;
	const float WANDER_OFFSET = 150.f;
	const float WANDER_RADIUS = 50.f;

	const float MAX_PREDICTON = 0.3f;

	const float OFFSET_Y;

	const int MAX_ABDUCTORS_PRODUCED = 20;;
	int m_abductorsProduced;
	const int TIME_TO_PRODUCE =  6;
	const int PRODUCE_TIME_OFFSET = 3;
	int m_timeToProduceAbductor;
	float m_produceAbductorTimer;
	sf::Vector2f m_targetPos;
	sf::Vector2f m_playerPos;

	GameObjectMap& m_gameObjectsRef;

	//missiles
	const int MAX_MISSILES_ALIVE = 2;
	int m_missilesAlive;
	const float RELOAD_TIME = 2.5f;
	float m_reloadTimer;
	
	const float DAMAGE = 20.f;

};