#include "Layer.h"

Layer::Layer(const std::string& path, const sf::Vector2f& screenSize, int sections, float scrollMultiplier)
	: SECTIONS(sections)
	, m_sections(SECTIONS)
	, m_screenSize(screenSize)
	, m_left(sections - 1)
	, m_middle(0)
	, m_right(1)
	, m_sectionWidth(0)
	, m_sectionHeight(0)
	, m_scrollMultiplier(scrollMultiplier)
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
		s.sprite.setOrigin((int)(s.sprite.getLocalBounds().width / 2.f), (int)(s.sprite.getLocalBounds().height / 2.f));
	}
	setPositions();
}

void Layer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//sf::Sprite left = m_sections[m_left].sprite;
	//left.setPosition((int)left.getPosition().x, (int)left.getPosition().y);

	//sf::Sprite middle = m_sections[m_left].sprite;
	//middle.setPosition((int)middle.getPosition().x, (int)middle.getPosition().y);

	//sf::Sprite right = m_sections[m_left].sprite;
	//right.setPosition((int)right.getPosition().x, (int)right.getPosition().y);

	target.draw(m_sections[m_left].sprite);
	target.draw(m_sections[m_middle].sprite);
	target.draw(m_sections[m_right].sprite);
}

void Layer::update(const sf::Vector2f & worldVelocity)
{
	m_sections[m_left].sprite.move(worldVelocity * m_scrollMultiplier);
	//m_sections[m_left].sprite.setPosition((int)m_sections[m_left].sprite.getPosition().x, (int)m_sections[m_left].sprite.getPosition().y);
	m_sections[m_middle].sprite.move(worldVelocity * m_scrollMultiplier);
	m_sections[m_right].sprite.move(worldVelocity * m_scrollMultiplier);

	if (m_sections[m_middle].sprite.getPosition().x + (m_sectionWidth / 2.f) < 0)
	{
		m_left = m_middle;
		m_middle = m_right;
		m_right = clamp(m_right + 1);
		//setPositions();
		positionSection(m_right, 1);
	}
	else if (m_sections[m_middle].sprite.getPosition().x - (m_sectionWidth / 2.f) > m_screenSize.x)
	{
		m_right = m_middle;
		m_middle = m_left;
		m_left = clamp(m_left - 1);
		//setPositions();
		positionSection(m_left, -1);
	}
}

void Layer::positionSection(int section, int direction)
{	
	m_sections[section].sprite.setPosition(sf::Vector2f(m_sections[m_middle].sprite.getPosition().x + (m_screenSize.x * direction),
														m_sections[m_middle].sprite.getPosition().y));
}



void Layer::setPositions()
{
	m_sections[m_left].sprite.setPosition(sf::Vector2f((int)((m_screenSize.x / 2.f) - m_sectionWidth), (int)(m_screenSize.y - (m_sectionHeight / 2.f))));
	m_sections[m_middle].sprite.setPosition(sf::Vector2f((int)(m_screenSize.x / 2.f), (int)(m_screenSize.y - (m_sectionHeight / 2.f))));
	m_sections[m_right].sprite.setPosition(sf::Vector2f((int)((m_screenSize.x / 2.f) + m_sectionWidth), (int)(m_screenSize.y - (m_sectionHeight / 2.f))));
}

int Layer::clamp(int value)
{
	if (value < 0)
	{
		return SECTIONS - 1;
	}
	if (value >= SECTIONS)
	{
		return 0;
	}

	return value; 
}