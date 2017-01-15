#pragma once

#include "GameObject.h"
#include "FSM.h"

template<typename T>
class AI : public GameObject
{
public:
	AI(const sf::Vector2f& startPos, GameObject::Type type)
		: GameObject(type, startPos)
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
		m_fsm.update();
		GameObject::update(dt);
	}

	void changeState(std::shared_ptr<State<T>> state)
	{
		m_fsm.changeState(state);
	}
protected:
	FSM<T> m_fsm;
};
