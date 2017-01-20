#pragma once
#include <vector>
#include "AI.h"
#include "AbductorStates.h"
#include "Astronaut.h"
#include "Mutant.h"

class Abductor : public AI<Abductor>
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	typedef std::unordered_map<std::string, GameObjectPtrVector> GameObjectMap;
	Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, std::shared_ptr<GameObject> player, GameObjectMap& gameObjectsRef);

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
	bool getAbducting() const;
	void setAbducting(bool value);
	void setAbductingVictim(const std::shared_ptr<Astronaut>& abductionVictim);
	void updateAbduction(float dt);
	void checkAbductionBounds();
	bool checkIfVictim(const std::shared_ptr<GameObject>& astroObject);

	const float DAMAGE = 5.f;
	//testing
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool collision(const std::shared_ptr<GameObject>& collidor) override;

private:
	//test
	sf::RectangleShape m_beamRect;

	//lasers
	const std::shared_ptr<GameObject> m_player;
	const float LASER_VEL_SCALE = 0.4f; //abductor lasers travel slower than players so the player gets a chance to avoid them
	const float PLAYER_LASER_RANGE = 500.f;
	const float RELOAD_TIME = 1.5f; //TODO put this reload time and timer in an interface
	float m_reloadTimer;

	GameObjectMap& m_gameObjectsRef;

	const float ABDUCT_TIME_TO_ARRIVE = 0.9f;
	const float ABDUCT_ARRIVE_RADIUS = 400.f;
	const float ABDUCT_SEEK_RANGE = 30.f;
	const int BEAM_SIZE = 5;
	const float ABDUCTION_X_DIFF = 20.f;
	const float ABDUCTION_RANGE;
	bool m_abducting;
	std::shared_ptr<Astronaut> m_abductionVictim;
	sf::Vector2f m_abductionTargetPos;
	bool m_reachedAbductionTarget;
	sf::Vector2f m_abductionOffset;
	const float ASCEND_SCALE = 0.1f;

	const float MAX_PREDICTION = 0.1f; //seek to predicted position
	sf::Vector2f m_acceleration;
	float m_targetInZone;
	const float ARRIVE_RADIUS = 200.f;
	const float TARGET_RANGE = 15.f;
	const float TIME_TO_TARGET = 0.5f;
	const float HIGHEST_DISTANCE;
	bool m_reachedTarget;

	const float NEIGHBOUR_RADIUS = 250.f;
	const float DESIRED_SEPARATION = 50.f;
	const float PLAYER_DESIRED_SEPARATION = 100.f;
	const float PLAYER_SEPERATION_FORCE_SCALE = 0.05f;
	const float PLAYER_FORCE_SCALER = 1.5f;

	const float SEPERATION_WEIGHT = 6.f;
	const float ALIGNMENT_WEIGHT = 1.1f; 
	const float COHESION_WEIGHT= 1.2f;
};