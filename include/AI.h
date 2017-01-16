#pragma once

#include "GameObject.h"
#include "FSM.h"

template<typename T>
class AI : public GameObject
{
public:
	AI(GameObject::Type type, const sf::Vector2f& startPos, const sf::Vector2f& worldSize)
		: GameObject(type, startPos, worldSize)
	{
		GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
		m_sprite.setTexture(props.texture);
		m_forceAmount = props.forceAmount;
		m_dragCoefficent = props.dragCoefficent;
		m_maxVelocity = props.maxVelocity;
		m_dir.x = 1.f;
		setOrigin();
	}

	void update(float dt) override
	{
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
protected:
	FSM<T> m_fsm;
};
