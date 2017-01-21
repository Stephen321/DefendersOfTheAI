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

	sf::Font font;
	font.loadFromFile("assets/fonts/GROBOLD.ttf");
	sf::Text scoreText("test", font, 30.f);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10.f, 10.f);
	scoreText.setStyle(sf::Text::Regular);

	sf::View view = window.getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	
	sf::Vector2f worldSize(bounds.width * Constants::WORLD_SCREEN_SIZES, bounds.height);

	sf::Sprite hyperJumpIcon;
	sf::Sprite iconBG;
	sf::Sprite bombIcon;

	hyperJumpIcon.setTexture(GameData::getInstance().hyperJumpIconTexture);
	iconBG.setTexture(GameData::getInstance().hyperJumpIconBGTexture);
	bombIcon.setTexture(GameData::getInstance().smartBombIconTexture);


	sf::RenderTexture leftTexture;
	leftTexture.create((unsigned int)bounds.width, (unsigned int)bounds.height);
	leftTexture.setView(sf::View(sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height)));
	sf::RenderTexture rightTexture;
	rightTexture.create((unsigned int)bounds.width, (unsigned int)bounds.height);
	rightTexture.setView(sf::View(sf::FloatRect(0.f, 0.f, bounds.width, bounds.height)));
	sf::RenderTexture preTeleportTexture;
	preTeleportTexture.create((unsigned int)bounds.width, (unsigned int)bounds.height);
	preTeleportTexture.setView(sf::View());

	GameObjectMap gameObjectsMap; 
	gameObjectsMap[Constants::ABDUCTOR_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::MUTANT_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::PROJECTILE_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::MISC_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::ASTRONAUT_KEY] = GameObjectPtrVector();
	gameObjectsMap[Constants::OBSTACLES_KEY] = GameObjectPtrVector();

	std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(sf::Vector2f(500.f, worldSize.y * 0.5f),
															 worldSize, gameObjectsMap[Constants::PROJECTILE_KEY]));
	gameObjectsMap.at(Constants::MISC_KEY).push_back(player);
	gameObjectsMap.at(Constants::MISC_KEY).push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(100.f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap)));
	gameObjectsMap.at(Constants::MISC_KEY).push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(worldSize.x * 0.25f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap)));
	gameObjectsMap.at(Constants::MISC_KEY).push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(worldSize.x * 0.5f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap)));
	gameObjectsMap.at(Constants::MISC_KEY).push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(worldSize.x * 0.6f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap)));
	gameObjectsMap.at(Constants::MISC_KEY).push_back(std::shared_ptr<Nest>(new Nest(sf::Vector2f(worldSize.x * 0.9f, worldSize.y * 0.1f),
		worldSize, player, gameObjectsMap)));

	gameObjectsMap.at(Constants::OBSTACLES_KEY).push_back(std::shared_ptr<Meteor>(new Meteor(worldSize, Helpers::randomNumber(10, 5) * bounds.width / 128))); 
	
	Background background(bounds, player);
	Radar radar(background.getSurfacePath(), worldSize);

	float astronautAmount = 100;
	for (int i = 0; i < astronautAmount; i++)
	{
		float x = Helpers::randomNumberF(0.f, worldSize.x);
		gameObjectsMap[Constants::ASTRONAUT_KEY].push_back(std::shared_ptr<Astronaut>(new Astronaut(x, worldSize, background.getSurfacePath())));
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
				//std::cout << "Going to screen: " << 2 << std::endl;
				//return (2);
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
		if (dt > 0.3f || pause)
			dt = 0.f;

		//update background
		background.update(dt, bounds);

		//update camera
		view.setCenter(player->getPosition().x , view.getCenter().y);
		bounds.left = view.getCenter().x - (bounds.width * 0.5f);
		bounds.top = view.getCenter().y - (bounds.height * 0.5f);
		window.setView(view);

		window.clear(sf::Color(96, 23, 54));
		leftTexture.clear();
		rightTexture.clear();
		preTeleportTexture.clear(sf::Color::Transparent);

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

		std::vector<std::pair<GameObject::Type, sf::Vector2f>>  radarEntities;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		{
			if (player->BombAvailable())
			{
				player->smartBomb();

				for (GameObjectMap::iterator it = gameObjectsMap.begin(); it != gameObjectsMap.end(); ++it)
				{
					GameObjectPtrVector& v = it->second;
					for (GameObjectPtrVector::iterator itV = v.begin(); itV != v.end();++itV)
					{
						std::shared_ptr<GameObject>& gameObject = (*itV);

						GameObject::Type type = gameObject->getType();
						if (type != GameObject::Type::Astronaut &&
							type != GameObject::Type::HyperJumpPickup &&
							type != GameObject::Type::Player &&
							type != GameObject::Type::Nest)
						{
							if (gameObject->getPosition().x + gameObject->getWidth() > bounds.left &&
								gameObject->getPosition().x < bounds.left + bounds.width)
							{
								gameObject->setActive(false);
								if (gameObject->getType() == GameObject::Type::Abductor)
								{
									std::shared_ptr<Abductor> abductor = std::static_pointer_cast<Abductor>(gameObject);
									if (abductor->getAbducting())
									{
										abductor->stopAbducting();
									}
								}
							}
						}
					}
				}
			}
		}

		if (player->canHyperJump())
		{
			hyperJumpIcon.setColor(sf::Color(255, 255, 255, 255));
		}
		else
		{
			hyperJumpIcon.setColor(sf::Color(255, 255, 255, 50));
		}

		if (player->BombAvailable())
		{
			bombIcon.setColor(sf::Color(255, 255, 255, 255));
		}
		else
		{
			bombIcon.setColor(sf::Color(255, 255, 255, 50));
		}

		for (GameObjectMap::iterator it = gameObjectsMap.begin(); it != gameObjectsMap.end(); ++it)
		{
			GameObjectPtrVector& v = it->second;
			for (GameObjectPtrVector::iterator itV = v.begin(); itV != v.end();)
			{				
				std::shared_ptr<GameObject>& gameObject = (*itV);
				GameObject::Type type = gameObject->getType();
				//update 
				gameObject->update(dt); //TODO: another loop before .clear
				std::pair<GameObject::Type, sf::Vector2f> entity;
				entity.first = gameObject->getType();
				entity.second = gameObject->getPosition();
				radarEntities.push_back(entity);

				//make abdbuctors abduct
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
				}

				if (type == GameObject::Type::Meteor)
				{
					//abductor collision with collider(meteor)
					checkForCollisions(gameObjectsMap.at(Constants::ABDUCTOR_KEY), gameObject);
					checkForCollisions(gameObjectsMap.at(Constants::MISC_KEY), gameObject);
					checkForCollisions(gameObjectsMap.at(Constants::MUTANT_KEY), gameObject);
					checkForCollisions(gameObjectsMap.at(Constants::PROJECTILE_KEY), gameObject);
				}

				if (type == GameObject::Type::Laser || type == GameObject::Type::Missile)
				{
					//misc collision with collider(laser/missile)
					checkForCollisions(gameObjectsMap.at(Constants::ABDUCTOR_KEY), gameObject);
					checkForCollisions(gameObjectsMap.at(Constants::MISC_KEY), gameObject); //check laser/missile collisions with misc (player, meteor, nests)
					checkForCollisions(gameObjectsMap.at(Constants::MUTANT_KEY), gameObject);

				}
				//draw
				drawGameObject(leftTexture, gameObject, sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height));
				drawGameObject(rightTexture, gameObject, sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height));

				drawGameObject(leftTexture, gameObject, sf::FloatRect(worldSize.x - bounds.width, 0.f, bounds.width, bounds.height));
				drawGameObject(rightTexture, gameObject, sf::FloatRect(0.f, 0.f, bounds.width, bounds.height));

				if (gameObject->getRect().intersects(bounds)) 
				{
					window.draw(*gameObject);
				}

				if (player->getPosition().x < bounds.width)
				{
					drawGameObject(preTeleportTexture, gameObject, sf::FloatRect(worldSize.x, 0.f, bounds.width, bounds.height));
				}
				else if (player->getPosition().x > worldSize.x - bounds.width)
				{
					drawGameObject(preTeleportTexture, gameObject, sf::FloatRect(-bounds.width, 0.f, bounds.width, bounds.height));
				}

				//remove if not active
				if (gameObject->getActive() == false)
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
		radar.update(player->getPosition(), bounds, radarEntities);

		if (rand() % Constants::METEOR_CHANCE == Constants::METEOR_CHANCE - 1)
			gameObjectsMap[Constants::OBSTACLES_KEY].push_back(std::shared_ptr<Meteor>(new Meteor(worldSize, Helpers::randomNumber(10, 5) * bounds.width / 128)));

		if (rand() % Constants::HYPERJUMP_CHANCE == Constants::HYPERJUMP_CHANCE - 1)
			gameObjectsMap[Constants::OBSTACLES_KEY].push_back(std::shared_ptr<Pickup>(new Pickup(GameObject::Type::HyperJumpPickup, worldSize)));

		/*if (zoomed) {
			view.zoom(zoom);
			zoomed = false;
		}*/
		

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
		else if (player->getPosition().x >= worldSize.x - bounds.width)
		{
			preTeleportTexture.display();
			sf::Sprite s(preTeleportTexture.getTexture());
			s.setPosition(worldSize.x - bounds.width, 0.f);
			window.draw(s);
		}

		iconBG.setPosition(sf::Vector2f(bounds.left + bounds.width * 0.05f, bounds.height * 0.05f));
		hyperJumpIcon.setPosition(sf::Vector2f(bounds.left + bounds.width * 0.05f, bounds.height * 0.05f));

		window.draw(iconBG);
		window.draw(hyperJumpIcon);

		iconBG.setPosition(sf::Vector2f(bounds.left + bounds.width * 0.05f, bounds.height * 0.1f));
		bombIcon.setPosition(sf::Vector2f(bounds.left + bounds.width * 0.05f, bounds.height * 0.1f));
		window.draw(iconBG);
		window.draw(bombIcon);


		scoreText.setPosition(sf::Vector2f(bounds.left + bounds.width * 0.05f, 1.f));
		scoreText.setString("Score: " + std::to_string(player->getScore()));
		window.draw(radar);

		window.draw(scoreText);


		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}

sf::FloatRect GameScreen::getRectFromView(const sf::View & view)
{
	return sf::FloatRect(view.getCenter().x - view.getSize().x * 0.5f,
		view.getCenter().y - view.getSize().y * 0.5f,
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

void GameScreen::checkForCollisions(std::vector<std::shared_ptr<GameObject>>& v, std::shared_ptr<GameObject>& collider)
{
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	GameObjectPtrVector::iterator begin = v.begin();
	GameObjectPtrVector::iterator end = v.end();
	for (GameObjectPtrVector::iterator it = begin; it != end; ++it)
	{
		std::shared_ptr<GameObject> gameObject = (*it);
		gameObject->collision(collider);
	}
}