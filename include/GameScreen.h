#pragma once

#include "Screen.h"
#include "Player.h"
#include "AI.h"
#include "Background.h"
#include "GameLoader.h"

class GameScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
};