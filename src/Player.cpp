#include "Player.h"

Player::Player(const sf::Vector2f& startPos, const sf::Texture& texture)
	: GameObject(startPos, texture)
{
}

void Player::update(float dt)
{
	move(dt);
}
