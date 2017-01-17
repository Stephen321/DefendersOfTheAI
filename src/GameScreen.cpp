#include "GameScreen.h"
#include "StateMap.h"
#include "NestStates.h"
#include "Abductor.h"

int GameScreen::run(sf::RenderWindow &window)
{
	GameLoader gameLoader("assets/");
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	sf::View view = window.getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	
	sf::Vector2f worldSize(bounds.width * Constants::WORLD_SCREEN_SIZES, bounds.height);

	sf::RenderTexture leftTexture;
	leftTexture.create(bounds.width, bounds.height);
	leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
	sf::RenderTexture rightTexture;
	rightTexture.create(bounds.width, bounds.height);
	rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));

	std::vector<std::shared_ptr<GameObject>> m_gameObjects;

	std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(sf::Vector2f(500.f, worldSize.y * 0.5f), worldSize));
	m_gameObjects.push_back(player);
	m_gameObjects.push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(100.f, worldSize.y * 0.1f), worldSize, player)));
	//m_gameObjects.push_back(std::make_shared<Nest>(Nest(sf::Vector2f(worldSize.x - 100.f, worldSize.y * 0.5f), worldSize)));
	
	std::vector<std::shared_ptr<Abductor>> testAbductors;
	sf::Vector2f testStart(100, 300);
	for (int i = 0; i < 1; i++)
	{
		testAbductors.push_back(std::shared_ptr<Abductor>(new Abductor(testStart, worldSize, testAbductors)));
	}

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
				testAbductors[rand() % testAbductors.size()]->setVelocity(sf::Vector2f(Helpers::randomNumberF(-0.03, 0.03f), Helpers::randomNumberF(-0.03, 0.03f)));
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
		leftTexture.clear();
		rightTexture.clear();
		//TODOWRAP: only draw section 0 and the last section


		leftTexture.setView(sf::View(sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height)));
		rightTexture.setView(sf::View(sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height)));
		leftTexture.draw(background);
		rightTexture.draw(background);
		leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
		rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));
		leftTexture.draw(background); //draw the surfaces
		rightTexture.draw(background);

		window.draw(background);
		for (const std::shared_ptr<GameObject>& go : m_gameObjects)
		{
			//if (go->getPosition().x - go->getWidth())

			leftTexture.setView(sf::View(sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height)));
			rightTexture.setView(sf::View(sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height)));
			leftTexture.draw(*go);
			rightTexture.draw(*go);
			leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
			rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));
			leftTexture.draw(*go);
			rightTexture.draw(*go);
			window.draw(*go); 
		}

		for (int i = 0; i < testAbductors.size(); i++)
		{
			testAbductors[i]->update(dt);
			leftTexture.setView(sf::View(sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height)));
			rightTexture.setView(sf::View(sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height)));
			leftTexture.draw(*testAbductors[i]);
			rightTexture.draw(*testAbductors[i]);
			leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
			rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));
			leftTexture.draw(*testAbductors[i]);
			rightTexture.draw(*testAbductors[i]);
			window.draw(*testAbductors[i]);
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
		leftTexture.draw(line, 16, sf::Lines);
		rightTexture.draw(line, 16, sf::Lines);
		window.draw(line, 16, sf::Lines);
		leftTexture.draw(boundsRect);
		rightTexture.draw(boundsRect);
		window.draw(boundsRect);

		if (player->getPosition().x <= bounds.width)
		{
			leftTexture.display();
			sf::Sprite s(leftTexture.getTexture());
			s.setPosition(-bounds.width, 0.f);
			window.draw(s);
		}
		else if (player->getPosition().x >= worldSize.x - bounds.width)
		{
			rightTexture.display();
			sf::Sprite s2(rightTexture.getTexture());
			s2.setPosition(worldSize.x, 0.f);
			window.draw(s2);
		}

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}