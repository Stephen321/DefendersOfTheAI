#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "Screen.h"

class GameScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
};

#endif