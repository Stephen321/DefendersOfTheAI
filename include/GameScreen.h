#pragma once

#include "Screen.h"
#include "Player.h"
#include "Background.h"

class GameScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
};