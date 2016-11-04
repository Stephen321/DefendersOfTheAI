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
		s.sprite.setOrigin((int)(s.sprite.getLocalBounds().width * 0.5f), (int)(s.sprite.getLocalBounds().height * 0.5f));
	}
	
	int yPos = (int)(m_screenSize.y - (m_sectionHeight * 0.5f));
	m_sections[m_left].sprite.setPosition(sf::Vector2f((int)((m_screenSize.x * 0.5f) - m_sectionWidth), yPos));
	m_sections[m_middle].sprite.setPosition(sf::Vector2f((int)(m_screenSize.x * 0.5f), yPos));
	m_sections[m_right].sprite.setPosition(sf::Vector2f((int)((m_screenSize.x * 0.5f) + m_sectionWidth), yPos));
}

void Layer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sections[m_left].sprite);
	target.draw(m_sections[m_middle].sprite);
	target.draw(m_sections[m_right].sprite);
}

void Layer::update(float worldVelX)
{
	m_sections[m_left].sprite.move(worldVelX * m_scrollMultiplier, 0);
	m_sections[m_middle].sprite.move(worldVelX * m_scrollMultiplier, 0);
	m_sections[m_right].sprite.move(worldVelX * m_scrollMultiplier, 0);

	if (m_sections[m_middle].sprite.getPosition().x + (m_sectionWidth * 0.5f) < 0)
	{
		m_left = m_middle;
		m_middle = m_right;
		m_right = Helpers::clamp(m_right + 1, 0, SECTIONS);
		positionSection(m_right, 1);
	}
	else if (m_sections[m_middle].sprite.getPosition().x - (m_sectionWidth * 0.5f) > m_screenSize.x)
	{
		m_right = m_middle;
		m_middle = m_left;
		m_left = Helpers::clamp(m_left - 1, 0, SECTIONS);
		positionSection(m_left, -1);
	}
}

sf::Vector2u Layer::getTotalSectionSize() const
{
	return sf::Vector2u(m_sectionWidth * SECTIONS, m_sectionHeight);
}

void Layer::positionSection(int section, int direction)
{	
	m_sections[section].sprite.setPosition(m_sections[m_middle].sprite.getPosition().x + (m_screenSize.x * direction),
										   m_sections[m_middle].sprite.getPosition().y);
}