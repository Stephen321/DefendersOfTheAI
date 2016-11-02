#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "Screen.h"

class MenuScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
};

#endif