#pragma once

#include "GameObject.h"
#include "Laser.h"

class Player : public GameObject
{
public:
	Player(const sf::Vector2f& startPos, const sf::Texture& texture, const sf::Texture& tempLaserTex);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void checkInput();
	void fire(); //TODO: put this into a super class such as Character which AI and the Player both inheirit from

	const sf::Texture& m_tempLaserTex;
	std::vector<Laser> m_lasers;
};