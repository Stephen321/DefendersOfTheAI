#ifndef  GAMEOBJECT_H
#define GAMEOBJECT_H

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
	void move(float dt);

private:
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_dir;
	float m_speed;
};

#endif 
