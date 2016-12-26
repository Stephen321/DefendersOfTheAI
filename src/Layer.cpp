#include "Layer.h"

Layer::Layer(const std::string& path, const sf::FloatRect& bounds, int sections, const std::shared_ptr<GameObject> player, float scrollMultiplier,
	const std::vector<std::shared_ptr<GameObject>> gameObjects)
	: m_teleportSection(-1)
	, m_teleported(true)
	, SECTIONS(sections)
	, m_sections(SECTIONS)
	, m_lastPlayerPos(m_player->getPosition())
	, m_bounds(bounds)
	, m_left(0)
	, m_middle(1)
	, m_right(2)
	, m_player(player)
	, m_scrollMultiplier(scrollMultiplier)
	, m_gameObjects(gameObjects)
	, m_scrollable(m_scrollMultiplier != 0.f)
{
	for (int i = 0; i < SECTIONS; i++)
	{
		Section& s = m_sections[i];
		s.texture.loadFromFile(path + std::to_string(i) + ".png");
		s.sprite.setTexture(s.texture);
		s.sprite.setScale(bounds.width / s.sprite.getLocalBounds().width, bounds.height / s.sprite.getLocalBounds().height);
		if (m_scrollMultiplier == 0.f)
		{
			s.sprite.setScale(s.sprite.getScale().x, 1.f);
		}
		int width = (int)s.sprite.getGlobalBounds().width;
		int height = (int)s.sprite.getGlobalBounds().height;

		s.sprite.setOrigin((int)(s.sprite.getLocalBounds().width  * 0.5f), (int)(s.sprite.getLocalBounds().height * 0.5f));
		s.sprite.setPosition((width * 0.5f) + (width * i), (m_bounds.height - (height * 0.5f)));

		//debug
		s.debugShape.setOutlineThickness(3.f);
		s.debugShape.setFillColor(sf::Color::Transparent);
		s.debugShape.setOutlineColor(sf::Color::Magenta);
		s.debugShape.setSize(sf::Vector2f(width, height));
		s.debugShape.setOrigin(s.debugShape.getLocalBounds().width * 0.5f, s.debugShape.getLocalBounds().height * 0.5f);
		s.debugShape.setPosition(s.sprite.getPosition());

		s.debugFont.loadFromFile("assets/fonts/GROBOLD.ttf");
		s.debugText = sf::Text(std::to_string(i), s.debugFont, 50.f);
		s.debugText.setFillColor(sf::Color::White);
		s.debugText.setPosition(s.sprite.getPosition());
		s.debugText.setStyle(sf::Text::Regular);
	}
}

void Layer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sections[m_left].sprite);
	target.draw(m_sections[m_middle].sprite);
	target.draw(m_sections[m_right].sprite);

	//debug
	target.draw(m_sections[m_left].debugShape);
	target.draw(m_sections[m_middle].debugShape);
	target.draw(m_sections[m_right].debugShape);
	target.draw(m_sections[m_left].debugText);
	target.draw(m_sections[m_middle].debugText);
	target.draw(m_sections[m_right].debugText);

	/*for (Section s : m_sections)
	{
	target.draw(s.sprite);
	target.draw(s.debugShape);
	target.draw(s.debugText);
	}*/
}

void Layer::update(float dt)
{
	sf::Vector2f currentPlayerPos = m_player->getPosition();
	float worldVelX = m_player->getVelocity().x * dt;
	float teleportCorrectionDist = 0.f;

	float distanceMoved = Helpers::getLength(currentPlayerPos - m_lastPlayerPos);
	if (distanceMoved > m_bounds.width && m_scrollMultiplier != 0.f) //if player just moved more than an entire screen in one frame then we know they have teleported
	{ //move all scrolling sections to be the same
		teleportCorrectionDist = distanceMoved;//moved right
		if (currentPlayerPos.x < m_lastPlayerPos.x)
		{
			teleportCorrectionDist *= -1;//moved left
		}
	}

	if (m_sections[m_middle].sprite.getPosition().x + (m_bounds.width * 0.5f) < m_bounds.left)
	{
		m_left = m_middle;
		m_middle = m_right;
		m_right = Helpers::clamp(m_right + 1, 0, SECTIONS);
		positionSection(m_right, 1);
	}
	else if (m_sections[m_middle].sprite.getPosition().x - (m_bounds.width * 0.5f) > m_bounds.left + m_bounds.width)
	{
		m_right = m_middle;
		m_middle = m_left;
		m_left = Helpers::clamp(m_left - 1, 0, SECTIONS);
		positionSection(m_left, -1);
	}

	if (m_teleportSection > -1) //is there a section to teleport?
	{
		int direction;
		int sectionLocation = m_teleportSection;
		int section = m_teleportSection;
		if (m_teleportSection == 0) //we have teleported section 0
		{
			direction = 1;
			if (m_middle == 0 || m_middle == SECTIONS - 2) //if we have moved onto it (0) or back away (SECTIONS - 2) then we need to teleport it back to its normal position
			{
				direction = -1;
				m_teleported = false;
				sectionLocation = SECTIONS;
				m_teleportSection = -1;
				if (m_middle == 0)
				{
					teleport(SECTIONS - 1, direction, SECTIONS - 1);
					m_teleportSection = 8;
					m_teleported = false;
				}
			}
		}
		else if (m_teleportSection == SECTIONS - 1)
		{
			direction = -1;
			if (m_middle == 1 || m_middle == SECTIONS - 1)
			{
				direction = 1;
				m_teleported = false;
				sectionLocation = -1;
				m_teleportSection = -1;
				if (m_middle == SECTIONS - 1)
				{
					teleport(0, direction, 0);
					m_teleportSection = 0;
					m_teleported = false;
				}
			}
		}
		if (m_teleported == false)//not already teleported
		{
			teleport(section, direction, sectionLocation);
		}
	}

	m_sections[m_left].sprite.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_sections[m_middle].sprite.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_sections[m_right].sprite.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);

	//debug
	m_sections[m_left].debugShape.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_sections[m_middle].debugShape.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_sections[m_right].debugShape.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_sections[m_left].debugText.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_sections[m_middle].debugText.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_sections[m_right].debugText.move(teleportCorrectionDist + worldVelX * m_scrollMultiplier, 0);
	m_lastPlayerPos = currentPlayerPos;
}

void Layer::teleport(int section, int direction, int sectionLocation)
{
	sf::Vector2f sectionPos = m_sections[section].sprite.getPosition();
	int sectionWidth = m_bounds.width;

	m_sections[section].sprite.setPosition(sectionPos.x + sectionWidth  * SECTIONS * direction, sectionPos.y);

	//debug
	m_sections[section].debugShape.setPosition(sectionPos.x + sectionWidth  * SECTIONS * direction, sectionPos.y);
	m_sections[section].debugText.setPosition(sectionPos.x + sectionWidth  * SECTIONS * direction, sectionPos.y);

	//teleport game objects that were in that section
	for (std::shared_ptr<GameObject>& go : m_gameObjects)
	{
		int location = (int)go->getPosition().x / sectionWidth; //forced integer division to get number between 0 and SECTIONS - 1
		if (go->getPosition().x < 0.f)
		{
			location--;
		}
		if (location == sectionLocation)
		{
			go->setPosition(sf::Vector2f(go->getPosition().x + sectionWidth * SECTIONS * direction, go->getPosition().y));
		}
	}

	m_teleported = true;
}

void Layer::positionSection(int section, int direction)
{
	if (m_scrollable)
	{
		float offset = ((m_bounds.width * 0.5f) + (m_bounds.width * 0.5f)) * direction;
		m_sections[section].sprite.setPosition(m_sections[m_middle].sprite.getPosition().x + offset,
			m_sections[section].sprite.getPosition().y);

		//debug
		m_sections[section].debugShape.setPosition(m_sections[m_middle].sprite.getPosition().x + offset,
			m_sections[section].sprite.getPosition().y);
		m_sections[section].debugText.setPosition(m_sections[m_middle].sprite.getPosition().x + offset,
			m_sections[section].sprite.getPosition().y);
	}
	else if (m_teleportSection < 0 && (m_middle == 0 || m_middle == SECTIONS - 1))
	{
		m_teleported = false;
		m_teleportSection = section;
	}
}