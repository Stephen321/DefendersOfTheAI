#include "GameScreen.h"
#include "StateMap.h"
#include "NestStates.h"

int GameScreen::run(sf::RenderWindow &window)
{
	GameLoader gameLoader("assets/");
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	sf::Vector2u windowSize = window.getSize();
	sf::View view = window.getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);


	sf::Vector2f worldSize(windowSize.x * 9.f, windowSize.y); //TODO: 9 is 9 screen widths, put in constant

	std::vector<std::shared_ptr<GameObject>> m_gameObjects;

	std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(sf::Vector2f(500.f, worldSize.y * 0.5f), worldSize));
	m_gameObjects.push_back(std::shared_ptr<Meteor>(new Meteor(worldSize)));
	m_gameObjects.push_back(player);
	m_gameObjects.push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(100.f, worldSize.y * 0.1f), worldSize, player)));	
	//m_gameObjects.push_back(std::make_shared<Nest>(Nest(sf::Vector2f(worldSize.x - 100.f, worldSize.y * 0.5f), worldSize)));
	

	Background background(bounds, player, m_gameObjects);

	//debug
	bool pause = false;
	float zoom = 1.f;
	bool zoomed = false;
	sf::RectangleShape boundsRect;
	boundsRect.setFillColor(sf::Color::Transparent);
	boundsRect.setOutlineColor(sf::Color::Yellow);
	boundsRect.setOutlineThickness(4.f);
	boundsRect.setPosition(bounds.left, bounds.top);
	boundsRect.setSize(sf::Vector2f(bounds.width, bounds.height));
	
	while (Running)
	{
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed || (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape))
			{
				return (-1);
			}

			if (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Return)
			{
				std::cout << "Going to screen: " << 2 << std::endl;
				return (2);
			}
			if (Event.type == sf::Event::MouseWheelScrolled)
			{
				if (Event.mouseWheelScroll.delta < 0 && zoom > 1)
					zoom = 1;
				else if (Event.mouseWheelScroll.delta > 0 && zoom < 1)
					zoom = 1;
				zoom += Event.mouseWheelScroll.delta / 100.f;
				zoomed = true;
			}

			if (Event.type == sf::Event::MouseButtonReleased)
			{
				if (Event.mouseButton.button == sf::Mouse::Button::Right)
					view.reset(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
			}
			if (Event.type == sf::Event::LostFocus)
			{
				pause = true;
			}
			if (Event.type == sf::Event::GainedFocus)
			{
				pause = false;
			}
		}

		float dt = frameClock.restart().asSeconds();
		if (dt > 0.3f || pause)//debug test for moving window/losing focus
			dt = 0.f;

		for (std::shared_ptr<GameObject>& go : m_gameObjects)
		{
			go->update(dt);
		}
		background.update(dt);

		view.setCenter(player->getPosition().x , view.getCenter().y);
		bounds.left = view.getCenter().x - (bounds.width * 0.5f);
		bounds.top = view.getCenter().y - (bounds.height * 0.5f);
		window.setView(view);

		window.clear(sf::Color(96, 23, 54));
		window.draw(background);
		for (const std::shared_ptr<GameObject>& go : m_gameObjects)
		{
			window.draw(*go);
		}
		
		//debug
		if (zoomed) {
			view.zoom(zoom);
			zoomed = false;
		}
		boundsRect.setPosition(bounds.left, bounds.top);
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Red),
			sf::Vertex(sf::Vector2f(350, 0.f), sf::Color::Red),
			sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Green),
			sf::Vertex(sf::Vector2f(0.f, 350), sf::Color::Green),
			sf::Vertex(sf::Vector2f(0.f, (float)window.getSize().y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(350.f,  (float)window.getSize().y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(0.f,  (float)window.getSize().y), sf::Color::Green),
			sf::Vertex(sf::Vector2f(0.f,  (float)window.getSize().y - 350.f), sf::Color::Green),
			sf::Vertex(sf::Vector2f((float)worldSize.x, 0.f), sf::Color::Magenta),
			sf::Vertex(sf::Vector2f((float)worldSize.x - 350.f, 0.f), sf::Color::Magenta),
			sf::Vertex(sf::Vector2f((float)worldSize.x, 0.f), sf::Color::Yellow),
			sf::Vertex(sf::Vector2f((float)worldSize.x, 350.f), sf::Color::Yellow),
			sf::Vertex(sf::Vector2f((float)worldSize.x, (float)window.getSize().y), sf::Color::Magenta),
			sf::Vertex(sf::Vector2f((float)worldSize.x - 350.f,  (float)window.getSize().y), sf::Color::Magenta),
			sf::Vertex(sf::Vector2f((float)worldSize.x,  (float)window.getSize().y), sf::Color::Yellow),
			sf::Vertex(sf::Vector2f((float)worldSize.x,  (float)window.getSize().y - 350.f), sf::Color::Yellow)
		};
		window.draw(line, 16, sf::Lines);
		window.draw(boundsRect);

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}