#pragma once

#include <iostream> //debug purposes
#include "Helpers.h"

class GameObject : public sf::Drawable
{
public:
	struct PhysicsProperties
	{
		PhysicsProperties(float maxForce, float dragCoefficient, float maxVel)
			: MAX_FORCE(maxForce)
			, DRAG_COEFFICIENT(dragCoefficient)
			, MAX_VEL(maxVel) {}
		const float MAX_FORCE;
		const float DRAG_COEFFICIENT;
		const float MAX_VEL;
	};

	GameObject(const sf::Vector2f& startPos, const sf::Texture& texture, const PhysicsProperties& physicProperties);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt);
	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& v);
	sf::Vector2f getVelocity() const;
	void moveBy(float dx, float dy);

protected:
	void move(float dt);

	sf::Sprite m_sprite;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_dir;
	sf::Vector2f m_force;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	PhysicsProperties m_physicsProperties;
};
