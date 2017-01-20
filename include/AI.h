#pragma once

#include "Constants.h"
#include "GameObject.h"
#include "FSM.h"

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
		m_dir.x = (rand() % 2 == 0) ? 1 : -1;
		setOrigin();
	}

	void update(float dt) override
	{
		if (m_active == false)
		{
			return;
		}
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
	/*	sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, collidor->getPosition(), m_position);
		float distance = Helpers::getLength(vectorBetween);

		if (collidor->getType() == Type::Meteor)
		{
			if (distance - collidor->getHeight() < 200.f )
			{
				sf::Vector2f repulsion = Helpers::normaliseCopy(vectorBetween);
				repulsion /= (distance - collidor->getHeight());
				repulsion *= m_maxVelocity;
				repulsion -= m_velocity;
				Helpers::limit(repulsion, m_forceAmount);
				m_repulsionForce = repulsion * .f;
			}
			else
			{
				m_repulsionForce.x = 0.f;
				m_repulsionForce.y = 0.f;
			}
		}*/
		return GameObject::collision(collidor);
	}

protected:
	const float LOWEST_DISTANCE;
	FSM<T> m_fsm;
};
