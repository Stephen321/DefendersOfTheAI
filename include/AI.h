#pragma once

#include "Constants.h"
#include "GameObject.h"
#include "FSM.h"
#include "HealthBar.h"
#include "Laser.h"

template<typename T>
class AI : public GameObject
{
public:
	AI(GameObject::Type type, const sf::Vector2f& startPos, const sf::Vector2f& worldSize)
		: GameObject(type, startPos, worldSize)
		, LOWEST_DISTANCE(worldSize.y * 0.65f)
	{
		GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
		m_sprite.setTexture(props.texture);
		m_forceAmount = props.forceAmount;
		m_dragCoefficent = props.dragCoefficent;
		m_maxVelocity = props.maxVelocity;
		m_healthBar = HealthBar(props.maxHealth);
		m_dir.x = (rand() % 2 == 0) ? 1.f : -1.f;
		setOrigin();
	}

	void update(float dt) override
	{
		if (m_active == false)
		{
			return;
		}
		m_healthBar.update(m_position);
		m_fsm.update(dt);
		GameObject::update(dt);
	}
	void changeState(std::shared_ptr<State<T>> state)
	{
		m_fsm.changeState(state);
	}

	void setMoving(bool moving)
	{
		m_moving = moving;
	}

	virtual bool collision(const std::shared_ptr<GameObject>& collidor) override
	{
		bool collided = false;
		if (GameObject::collision(collidor))
		{
			collided = true;

			float damage = 0.f;
			if (collidor->getType() == Type::Laser)
			{
				std::shared_ptr<Laser> laser = std::static_pointer_cast<Laser>(collidor);
				if (laser->ownerType() == Type::Player)
				{
					damage = laser->getDamage();
					collidor->setActive(false);
				}
			}

			if (m_healthBar.changeHealth(-damage) == false)
			{
				m_active = false;
			}
		}
		return collided;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		GameObject::draw(target, states);
		target.draw(m_healthBar);
	}

protected:
	const float LOWEST_DISTANCE;
	HealthBar m_healthBar;
	FSM<T> m_fsm;
	float HEALTH_Y_OFFSET = 1.f;
};
