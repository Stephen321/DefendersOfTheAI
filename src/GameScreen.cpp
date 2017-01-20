#include "GameScreen.h"
#include "StateMap.h"
#include "NestStates.h"
#include "Abductor.h"
#include "Astronaut.h"
#include "Mutant.h"

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
	leftTexture.create((unsigned int)bounds.width, (unsigned int)bounds.height);
	leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
	sf::RenderTexture rightTexture;
	rightTexture.create((unsigned int)bounds.width, (unsigned int)bounds.height);
	rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));
	sf::RenderTexture preTeleportTexture;
	preTeleportTexture.create((unsigned int)bounds.width, (unsigned int)bounds.height);
	preTeleportTexture.setView(sf::View());

	GameObjectMap gameObjectsMap; //TODO: instead of 4 vectors?

	std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(sf::Vector2f(500.f, worldSize.y * 0.5f),
															 worldSize, gameObjectsMap[Constants::PROJECTILE_KEY]));
	gameObjectsMap[Constants::MISC_KEY].push_back(player);
	/*gameObjectsMap[Constants::MISC_KEY].push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(100.f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY], gameObjectsMap[Constants::ABDUCTOR_KEY])));*/
	//gameObjectsMap[Constants::MISC_KEY].push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(worldSize.x - 100.f, worldSize.y * 0.1f), worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY])));
	//m_gameObjects.push_back(std::make_shared<Nest>(Nest(sf::Vector2f(worldSize.x - 100.f, worldSize.y * 0.5f), worldSize)));
	gameObjectsMap[Constants::OBSTACLES_KEY].push_back(std::shared_ptr<Meteor>(new Meteor(worldSize, Helpers::randomNumber(10, 5) * bounds.width / 128))); //todo: 128???
	

	gameObjectsMap[Constants::MUTANT_KEY].push_back(std::shared_ptr<Mutant>(new Mutant(sf::Vector2f(100.f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY], gameObjectsMap[Constants::MUTANT_KEY])));
	gameObjectsMap[Constants::MUTANT_KEY].push_back(std::shared_ptr<Mutant>(new Mutant(sf::Vector2f(200.f, worldSize.y * 0.2f),
		worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY], gameObjectsMap[Constants::MUTANT_KEY])));
	gameObjectsMap[Constants::MUTANT_KEY].push_back(std::shared_ptr<Mutant>(new Mutant(sf::Vector2f(300.f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY], gameObjectsMap[Constants::MUTANT_KEY])));
	gameObjectsMap[Constants::MUTANT_KEY].push_back(std::shared_ptr<Mutant>(new Mutant(sf::Vector2f(400.f, worldSize.y * 0.2f),
		worldSize, player, gameObjectsMap[Constants::PROJECTILE_KEY], gameObjectsMap[Constants::MUTANT_KEY])));
	
	Background background(bounds, player);

	float testing = 0;
	for (int i = 0; i < 1; i++)
	{
		if (testing > worldSize.x)
			break;
		gameObjectsMap[Constants::ASTRONAUT_KEY].push_back(std::shared_ptr<Astronaut>(new Astronaut(testing, worldSize, background.getSurfacePath())));
		testing += 600.f;
	}

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
				GameObject::Type type = gameObject->getType();
				//update 
				gameObject->update(dt); //TODO: another loop before .clear

				//make abdbuctors abductor
				if (type == GameObject::Type::Abductor)
				{//loop through all astronauts
					GameObjectPtrVector::iterator begin = gameObjectsMap[Constants::ASTRONAUT_KEY].begin();
					GameObjectPtrVector::iterator end = gameObjectsMap[Constants::ASTRONAUT_KEY].end();
					for (GameObjectPtrVector::iterator astroIt = begin; astroIt != end; ++astroIt)
					{
						std::shared_ptr<GameObject> astroGameObject = (*astroIt);
						std::shared_ptr<Abductor> abductor = std::static_pointer_cast<Abductor>(gameObject);
						if (abductor->checkIfVictim(astroGameObject)) //if it can be made a abductor
						{
							std::shared_ptr<Astronaut> astronaut = std::static_pointer_cast<Astronaut>(astroGameObject);
							if (astronaut->getBeingAbducted() == false && astronaut->getBeingChased() == false) //if can be made victim
							{
								abductor->setAbducting(true);
								abductor->setAbductingVictim(astronaut);
								astronaut->setBeingChased(true);
								astronaut->setAbductor(abductor);
							}
						}
						
					}
				} //check for collision with meteor - AI (not astronaut)
				if (type != GameObject::Type::Meteor && type != GameObject::Type::Astronaut
					&& type != GameObject::Type::Player)
				{
					GameObjectPtrVector::iterator begin = gameObjectsMap[Constants::OBSTACLES_KEY].begin();
					GameObjectPtrVector::iterator end = gameObjectsMap[Constants::OBSTACLES_KEY].end();
					for (GameObjectPtrVector::iterator obstacleIT = begin; obstacleIT != end; ++obstacleIT)
					{
						std::shared_ptr<GameObject> obstacle = (*obstacleIT);
						gameObject->collision(obstacle);
					}
				}

				//draw
				drawGameObject(leftTexture, gameObject, sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height));
				drawGameObject(rightTexture, gameObject, sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height));

				drawGameObject(leftTexture, gameObject, sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height));
				drawGameObject(rightTexture, gameObject, sf::FloatRect(0.f, 0.f, bounds.width, bounds.height));

				if (gameObject->getRect().intersects(getRectFromView(window.getView())))//bounds)) test test test 
				{
					window.draw(*gameObject);
				}
				//TODO: removing this causes flicker??
				if (player->getPosition().x < bounds.width)
				{
					drawGameObject(preTeleportTexture, gameObject, sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height));
				}
				else if (player->getPosition().x > worldSize.x - bounds.width)
				{
					drawGameObject(preTeleportTexture, gameObject, sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height));
				}


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

void GameScreen::drawGameObject(sf::RenderTarget & target, std::shared_ptr<GameObject>& gameObject, const sf::FloatRect & viewBounds)
{
	target.setView(sf::View(viewBounds));
	if (gameObject->getRect().intersects(getRectFromView(target.getView())))
	{
		target.draw(*gameObject);
	}
	target.setView(target.getDefaultView());
}
