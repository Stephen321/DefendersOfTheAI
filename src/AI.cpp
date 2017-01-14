#include "AI.h"


AI::AI(const sf::Vector2f& startPos)
	: GameObject(Type::AI, startPos)
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;
	m_dir.x = 1.f;

	setOrigin();
}
