#pragma once

#include <iostream> //debug purposes
#include "Helpers.h"
#include "GameData.h"

class GameObject : public sf::Drawable
{
public:
	enum class Type {
		Player,
		AI,
		Laser
	};
	GameObject(Type type, const sf::Vector2f& startPos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt);
	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& v);
	sf::Vector2f getVelocity() const;
	void moveBy(float dx, float dy);
	virtual void teleport(float offset, int section, float width);
	Type getType() const;
	bool getActive() const;

protected:
	void setOrigin();
	void move(float dt);

	bool m_active;
	bool m_moving;
	const float MIN_VEL = 2.5f;
	Type m_type;
	sf::Sprite m_sprite;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_dir;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_forceAmount;
	float m_dragCoefficent;
	float m_maxVelocity;
};
