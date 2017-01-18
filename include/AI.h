#pragma once

#include "Constants.h"
#include "GameObject.h"
#include "FSM.h"

template<typename T>
class AI : public GameObject
{
public:
	AI(GameObject::Type type, const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const sf::FloatRect& cameraBounds)
		: GameObject(type, startPos, worldSize, cameraBounds)
		, LOWEST_DISTANCE(worldSize.y * 0.6f)
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

	void checkWorldBounds() override
	{
		GameObject::checkWorldBounds();
		float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
		if (m_position.y > LOWEST_DISTANCE - halfHeight)
		{
			m_position.y = LOWEST_DISTANCE - halfHeight;
			m_velocity.y = 0.f;
		}
	}
protected:
	const float LOWEST_DISTANCE;
	FSM<T> m_fsm;
};
