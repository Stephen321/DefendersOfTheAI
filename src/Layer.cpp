#include "Layer.h"

Layer::Layer(const std::string& path, const sf::FloatRect& bounds, int sections, const std::shared_ptr<GameObject> player, float scrollMultiplier)
	: SECTIONS(sections)
	, m_sections(SECTIONS)
	, m_lastPlayerPos(m_player->getPosition())
	, m_bounds(bounds)
	, m_left(0)
	, m_middle(1)
	, m_right(2)
	, m_player(player)
	, m_scrollMultiplier(scrollMultiplier)
	, m_scrollable(m_scrollMultiplier != 0.f)
{
	for (int i = 0; i < SECTIONS; i++)
	{
		Section& s = m_sections[i];
		s.texture.loadFromFile(path + std::to_string(i) + ".png");
		s.texture.setSmooth(true);
		s.sprite.setTexture(s.texture);
		float scalar = bounds.width / s.sprite.getLocalBounds().width;
		s.sprite.setScale(scalar, scalar);
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
	//target.draw(m_sections[m_left].debugShape);
	//target.draw(m_sections[m_middle].debugShape);
	//target.draw(m_sections[m_right].debugShape);
	//target.draw(m_sections[m_left].debugText);
	//target.draw(m_sections[m_middle].debugText);
	//target.draw(m_sections[m_right].debugText);

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
}