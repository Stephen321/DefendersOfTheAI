#pragma once

#include "GameObject.h"
#include "Laser.h"

class Player : public GameObject
{
public:
	Player(const sf::Vector2f& startPos);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void teleport(float offset, int section, float width) override;

private:
	void checkInput();
	void fire(); //TODO: put this into a super class such as Character which AI and the Player both inheirit from
	const float RELOAD_TIME = 0.1f;
	float m_reloadTimer;
	std::vector<Laser> m_lasers;
};