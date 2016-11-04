#ifndef  GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream> //debug purposes
#include "SFML\Graphics.hpp"

class GameObject : public sf::Drawable
{
public:
	GameObject(const sf::Vector2f& startPos, const sf::Texture& texture);
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
	sf::Vector2f normalise(const sf::Vector2f& v); //put this in a helper class?
	virtual void calcForce();

private:
	const float DRAG_COEFFICIENT;
	const float MAX_VEL;

	float getVectorLength(const sf::Vector2f& v); //put this in a helper class?
};

#endif 
