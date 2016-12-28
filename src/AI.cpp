#include "AI.h"


AI::AI(const sf::Vector2f& startPos, const sf::Texture& texture)
	: GameObject(startPos, texture, PhysicsProperties(800.f, 0.9f, 1000.f))
{
}
