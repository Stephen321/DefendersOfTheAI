#include "MenuScreen.h"

int MenuScreen::run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	sf::View view = window.getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	sf::Texture startTexture;
	startTexture.loadFromFile("assets/sprites/start.png");
	sf::Sprite start(startTexture);
	start.setPosition(0.f, 0.f);
	start.setScale(bounds.width / start.getLocalBounds().width, bounds.height / start.getLocalBounds().height);

	while (Running)
	{
		float dt = frameClock.restart().asSeconds();

		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}

			if (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Space)
			{
				std::cout << "Going to screen: " << 1 << std::endl;
				return (1);
			}
		}

		window.clear();
		window.draw(start);
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}