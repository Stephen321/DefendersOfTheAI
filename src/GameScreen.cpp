#include "GameScreen.h"

int GameScreen::run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	sf::Vector2f windowSize = (sf::Vector2f)window.getSize();

	Background background(windowSize);

	sf::Vector2u worldSize = background.getWorldSize(); //put this somewhere else and use world units?

	sf::Texture playerTex;
	playerTex.loadFromFile("assets/sprites/player/square.png");
	Player player(worldSize, windowSize * 0.5f, playerTex);

	

	while (Running)
	{
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}

			if (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Space)
			{
				std::cout << "Going to screen: " << 2 << std::endl;
				return (2);
			}
		}
		float dt = frameClock.restart().asSeconds();

		player.update(dt);
		sf::Vector2f worldVelocity = -player.getVelocity() * dt;
		background.update(worldVelocity);

		window.clear();
		window.draw(background);
		window.draw(player);
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}