#include "GameScreen.h"

int GameScreen::run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	sf::Texture playerTex;
	playerTex.loadFromFile("assets/sprites/player/square.png");
	Player player(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), playerTex);

	Background background(window.getView().getSize());
	

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
		sf::Vector2f worldVelocity = -player.getVelocity();
		background.update(worldVelocity);

		window.clear();
		window.draw(background);
		window.draw(player);
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}