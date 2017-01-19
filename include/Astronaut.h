#pragma once
#include "AI.h"
#include "AstronautStates.h"
#include "Abductor.h"

class Astronaut : public AI<Astronaut>
{
public:
	Astronaut(float startX, const sf::Vector2f& worldSize, const std::vector<sf::Vector2i>& surfacePathPoints);
	void checkWorldBounds() override;
	void setBeingAbducted(bool value);
	void setBeingChased(bool value);
	bool getBeingAbducted() const;
	bool getBeingChased() const;
	void setTarget(const sf::Vector2f& target);
	float getYAtX(float x);
	void setAbductor(const std::shared_ptr<GameObject>& abductor);
	const std::shared_ptr<GameObject>& getAbductor() const;
	void seekAbductionPos();
	void move(float dt) override;
	void updateAcceleration();
private:
	bool m_beingAbducted; //todo: state for this?
	bool m_beingChased;
	sf::Vector2f m_targetPos;
	std::vector<sf::Vector2i> m_surfacePathPoints;
	const float SURFACE_RIM_THICKNESS = 5.f;
	std::shared_ptr<GameObject> m_abductor;

	sf::Vector2f m_acceleration;
	const float ARRIVE_RADIUS = 50.f;
	const float TARGET_RANGE = 5.f;
	const float TIME_TO_TARGET = 0.3f;
};