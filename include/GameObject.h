#pragma once

#include <iostream> //debug purposes
#include "Helpers.h"

class GameObject : public sf::Drawable
{
public:
	GameObject(const sf::Vector2u& worldSize, const sf::Vector2f& startPos, const sf::Texture& texture);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt);
	sf::Vector2f getPosition() const;
	sf::Vector2f getVelocity() const;

protected:
	sf::Sprite m_sprite;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_dir;
	sf::Vector2f m_force;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	const float FORCE;

	void move(float dt);
	virtual void calcForce();

private:
	sf::Vector2u m_worldSize;
	const float DRAG_COEFFICIENT;
	const float MAX_VEL;
};
