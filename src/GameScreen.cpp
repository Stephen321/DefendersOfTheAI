#include "GameScreen.h"
#include "StateMap.h"
#include "NestStates.h"
#include "Abductor.h"

int GameScreen::run(sf::RenderWindow &window)
{
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	typedef std::unordered_map<std::string, GameObjectPtrVector> GameObjectMap;
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
	sf::RenderTexture preTeleportTexture;
	preTeleportTexture.create(bounds.width, bounds.height);
	preTeleportTexture.setView(sf::View());

	GameObjectMap gameObjectsMap; //TODO: instead of 4 vectors?
	gameObjectsMap[Constants::ABDUCTOR_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::MUTANT_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::PROJECTILE_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::MISC_KEY] = GameObjectPtrVector();

	std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(sf::Vector2f(500.f, worldSize.y * 0.5f),
															 worldSize, gameObjectsMap[Constants::PROJECTILE_KEY]));
	gameObjectsMap[Constants::MISC_KEY].push_back(player);
	gameObjectsMap[Constants::MISC_KEY].push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(100.f, worldSize.y * 0.1f),
												  worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY], gameObjectsMap[Constants::ABDUCTOR_KEY])));
	//gameObjectsMap[Constants::MISC_KEY].push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(worldSize.x - 100.f, worldSize.y * 0.1f), worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY])));
	//m_gameObjects.push_back(std::make_shared<Nest>(Nest(sf::Vector2f(worldSize.x - 100.f, worldSize.y * 0.5f), worldSize)));
	gameObjectsMap[Constants::MISC_KEY].push_back(std::shared_ptr<Meteor>(new Meteor(worldSize, Helpers::randomNumber(10, 5) * bounds.width / 128)));
	Background background(bounds, player);

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
				//testAbductors[rand() % testAbductors.size()]->setVelocity(sf::Vector2f(Helpers::randomNumberF(-0.03, 0.03f), Helpers::randomNumberF(-0.03, 0.03f)));
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

		//get dt
		float dt = frameClock.restart().asSeconds();
		if (dt > 0.3f || pause)//debug test for moving window/losing focus
			dt = 0.f;

		//update background
		background.update(dt, bounds);// getRectFromView(window.getView())); //TODO: add back in after testing 

		//update camera
		view.setCenter(player->getPosition().x , view.getCenter().y);
		bounds.left = view.getCenter().x - (bounds.width * 0.5f);
		bounds.top = view.getCenter().y - (bounds.height * 0.5f);
		window.setView(view);

		window.clear(sf::Color(96, 23, 54));
		leftTexture.clear();
		rightTexture.clear();
		preTeleportTexture.clear(sf::Color::Transparent);
		//TODOWRAP: only draw section 0 and the last section

		//draw background
		leftTexture.setView(sf::View(sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height)));
		rightTexture.setView(sf::View(sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height)));
		leftTexture.draw(background);
		rightTexture.draw(background);
		leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
		rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));
		leftTexture.draw(background); //draw the surfaces
		rightTexture.draw(background);

		window.draw(background);

		for (GameObjectMap::iterator it = gameObjectsMap.begin(); it != gameObjectsMap.end(); ++it)
		{
			GameObjectPtrVector& v = it->second;
			for (GameObjectPtrVector::iterator itV = v.begin(); itV != v.end();)
			{				
				std::shared_ptr<GameObject>& gameObject = (*itV);
				//update 
				gameObject->update(dt); //TODO: another loop before .clear

										//draw
				bool intersects = false;

				leftTexture.setView(sf::View(sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height)));
				rightTexture.setView(sf::View(sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height)));
				if (gameObject->getRect().intersects(getRectFromView(leftTexture.getView())))
				{
					leftTexture.draw(*gameObject);
				}
				if (gameObject->getRect().intersects(getRectFromView(rightTexture.getView())))
				{
					rightTexture.draw(*gameObject);
				}

				leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
				rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));
				if (gameObject->getRect().intersects(getRectFromView(leftTexture.getView())))
				{
					leftTexture.draw(*gameObject);
				}
				if (gameObject->getRect().intersects(getRectFromView(rightTexture.getView())))
				{
					rightTexture.draw(*gameObject);
				}


				
				if (gameObject->getRect().intersects(getRectFromView(window.getView())))//bounds)) test test test 
				{
					window.draw(*gameObject);
				}
				if (player->getPosition().x < bounds.width)
				{
					preTeleportTexture.setView(sf::View(sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height)));
				}
				else if (player->getPosition().x > worldSize.x - bounds.width)
				{
					preTeleportTexture.setView(sf::View(sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height)));
				}
				else
				{
					preTeleportTexture.setView(sf::View());;
				}
				//if (gameObject->getRect().intersects(getRectFromView(preTeleportTexture.getView())))
				//{
					preTeleportTexture.draw(*gameObject);
				//}


				//remove if not active
				if ((*itV)->getActive() == false)
				{
					itV = v.erase(itV);
					if (v.empty())
					{
						break;
					}
				}
				else
				{
					++itV;
				}
			}
		}	
			
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

		if (player->getPosition().x <= bounds.width || player->getPosition().x >= worldSize.x - bounds.width)
		{
			leftTexture.display();
			sf::Sprite s(leftTexture.getTexture());
			s.setPosition(-bounds.width, 0.f);
			window.draw(s);
			rightTexture.display();
			sf::Sprite s2(rightTexture.getTexture());
			s2.setPosition(worldSize.x, 0.f);
			window.draw(s2);
		}
		if (player->getPosition().x <= bounds.width)
		{
			preTeleportTexture.display();
			sf::Sprite s(preTeleportTexture.getTexture());
			s.setPosition(0.f, 0.f);
			window.draw(s);
		}
		else if(player->getPosition().x >= worldSize.x - bounds.width)
		{
			preTeleportTexture.display();
			sf::Sprite s(preTeleportTexture.getTexture());
			s.setPosition(worldSize.x - bounds.width, 0.f);
			window.draw(s);
		}

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}

sf::FloatRect GameScreen::getRectFromView(const sf::View & view)
{
	return sf::FloatRect(view.getCenter().x - view.getSize().x * 0.5f,
						 view.getCenter().y- view.getSize().y * 0.5f,
						 view.getSize().x,
						 view.getSize().y);
}
