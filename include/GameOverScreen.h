#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include "Screen.h"

class GameOverScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
};

#endif