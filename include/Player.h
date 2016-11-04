#ifndef  PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(const sf::Vector2f& startPos, const sf::Texture& texture);
	void update(float dt) override;

private:
	void calcForce() override;
};

#endif 
