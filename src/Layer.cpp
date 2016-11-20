#include "Layer.h"

Layer::Layer(const std::string& path, const sf::FloatRect& bounds, int sections, float scrollMultiplier)
	: m_teleportSection(-1)
	, m_teleported(true)
	, SECTIONS(sections)
	, m_sections(SECTIONS)
	, m_bounds(bounds)
	, m_left(0)
	, m_middle(1)
	, m_right(2)
	, m_sectionWidth(0)
	, m_sectionHeight(0)
	, m_scrollMultiplier(scrollMultiplier)
	, m_scrollable(m_scrollMultiplier != 0.f)
{
	for (int i = 0; i < SECTIONS; i++)
	{
		Section& s = m_sections[i];
		s.texture.loadFromFile(path + std::to_string(i) + ".png");
		s.sprite.setTexture(s.texture);
		if (m_sectionWidth == 0 && m_sectionHeight == 0)
		{
			m_sectionWidth = (int)s.sprite.getGlobalBounds().width;
			m_sectionHeight = (int)s.sprite.getGlobalBounds().height;
		}
		s.sprite.setOrigin((int)(s.sprite.getLocalBounds().width * 0.5f), (int)(s.sprite.getLocalBounds().height * 0.5f));
		s.sprite.setPosition(s.sprite.getOrigin().x + (m_sectionWidth * i), (m_bounds.height - (m_sectionHeight * 0.5f)));

		//debug
		s.debugShape.setOutlineThickness(3.f);
		s.debugShape.setFillColor(sf::Color::Transparent);
		s.debugShape.setOutlineColor(sf::Color::Magenta);
		s.debugShape.setOrigin(s.sprite.getOrigin());
		s.debugShape.setSize(sf::Vector2f(m_sectionWidth, m_sectionHeight));
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
}

void Layer::update(float worldVelX)
{
	if (m_scrollable)
	{
		m_sections[m_left].sprite.move(worldVelX * m_scrollMultiplier, 0);
		m_sections[m_middle].sprite.move(worldVelX * m_scrollMultiplier, 0);
		m_sections[m_right].sprite.move(worldVelX * m_scrollMultiplier, 0);

		//debug
		m_sections[m_left].debugShape.move(worldVelX * m_scrollMultiplier, 0);
		m_sections[m_middle].debugShape.move(worldVelX * m_scrollMultiplier, 0);
		m_sections[m_right].debugShape.move(worldVelX * m_scrollMultiplier, 0);
		m_sections[m_left].debugText.move(worldVelX * m_scrollMultiplier, 0);
		m_sections[m_middle].debugText.move(worldVelX * m_scrollMultiplier, 0);
		m_sections[m_right].debugText.move(worldVelX * m_scrollMultiplier, 0);


	/*	std::cout << "Middle X: " << m_sections[m_middle].sprite.getPosition().x << " Y: " << m_sections[m_middle].sprite.getPosition().y << ". ";
		std::cout << "Bounds X: " << m_bounds.left << " Y: " << m_bounds.top << std::endl;*/
	}


	if (m_sections[m_middle].sprite.getPosition().x + (m_sectionWidth * 0.5f) < m_bounds.left)
	{
		m_left = m_middle;
		m_middle = m_right;
		m_right = Helpers::clamp(m_right + 1, 0, SECTIONS);
		positionSection(m_right, 1);
	}
	else if (m_sections[m_middle].sprite.getPosition().x - (m_sectionWidth * 0.5f) > m_bounds.left + m_bounds.width)
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
}

void Layer::teleport(int section, int direction, int sectionLocation)
{
	m_sections[section].sprite.move(m_sectionWidth * SECTIONS * direction, 0.f);

	//debug
	m_sections[section].debugShape.move(m_sectionWidth * SECTIONS * direction, 0.f);
	m_sections[section].debugText.move(m_sectionWidth * SECTIONS * direction, 0.f);

	//teleport game objects that were in that section
	for (std::shared_ptr<GameObject>& go : m_gameObjects)
	{
		int location = (int)go->getPosition().x / m_sectionWidth; //forced integer division to get number between 0 and SECTIONS - 1
		if (go->getPosition().x < 0.f)
		{
			location--;
		}
		if (location == sectionLocation)
		{
			go->move(m_sectionWidth * SECTIONS * direction, 0.f);
		}
	}

	m_teleported = true;
}

sf::Vector2u Layer::getTotalSectionSize() const
{
	return sf::Vector2u(m_sectionWidth * SECTIONS, m_sectionHeight);
}

void Layer::positionSection(int section, int direction)
{
	if (m_scrollable)
	{
		m_sections[section].sprite.setPosition(m_sections[m_middle].sprite.getPosition().x + (m_bounds.width * direction),
			m_sections[m_middle].sprite.getPosition().y);

		//debug
		m_sections[section].debugShape.setPosition(m_sections[m_middle].sprite.getPosition().x + (m_bounds.width * direction),
			m_sections[m_middle].sprite.getPosition().y);
		m_sections[section].debugText.setPosition(m_sections[m_middle].sprite.getPosition().x + (m_bounds.width * direction),
			m_sections[m_middle].sprite.getPosition().y);
	}
	else if (m_teleportSection < 0 && (m_middle == 0 || m_middle == SECTIONS - 1))
	{
		m_teleported = false;
		m_teleportSection = section;
	}
}


void Layer::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	m_gameObjects.push_back(gameObject);
}