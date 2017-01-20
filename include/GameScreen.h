#pragma once

#include "Screen.h"
#include "Player.h"
#include "Nest.h"
#include "Background.h"
#include "GameLoader.h"
#include "Meteor.h"
#include "Radar.h"
#include "Pickup.h"

class GameScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
private: 
	sf::FloatRect getRectFromView(const sf::View& view);
	void drawGameObject(sf::RenderTarget& target, std::shared_ptr<GameObject>& gameObject, const sf::FloatRect& viewBounds);
};