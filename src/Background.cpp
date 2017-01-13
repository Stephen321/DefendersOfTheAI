#include "Background.h"
#include <iostream>

Background::Background(const sf::Vector2f& screenSize)
{
	m_layers.push_back(Layer("assets/sprites/enviroment/stars", screenSize, 3, 0.33f));
	//m_layers.push_back(Layer("assets/sprites/enviroment/surface", screenSize, 9, 1.f));

	generateSurface(screenSize);
}

void Background::generateSurface(const sf::Vector2f& screenSize)
{
	int xPos = 0;
	int yPos = screenSize.y * 0.75f;
	int worldWidth = screenSize.x * 9;
	int currentWidth = 0;

	while (currentWidth < worldWidth)
	{
		sf::ConvexShape convex;

		int width = (rand() % 16 * 10) + 40;
		int height = 0;


		if (m_surfaceShapes.size() != 0)
		{
			if (m_surfaceShapes.size() % 2 == 0)
			{
				height += rand() % 2 + 1;

				if (((rand() % 2 == 0 || yPos > screenSize.y * 0.9) && yPos > screenSize.y * 0.75f) || currentWidth > worldWidth * 0.925f)
				{
					height *= -1;
				}

				height *= 10;
				width = abs(height);
			}

			// make the first and last shape y the same so they seamlessly connect
			if (yPos == m_surfaceShapes[0].getPoint(0).y && currentWidth > worldWidth * 0.925f)
			{
				width = worldWidth - currentWidth; //close the gap
			}
		}
				

		// resize it to 4 points
		convex.setPointCount(4);

		// define the points
		convex.setPoint(0, sf::Vector2f(xPos, yPos));
		convex.setPoint(1, sf::Vector2f(xPos + width, yPos + height));
		convex.setPoint(2, sf::Vector2f(xPos + width, screenSize.y));
		convex.setPoint(3, sf::Vector2f(xPos, screenSize.y));
		
		if (convex.getPoint(0).y == convex.getPoint(1).y) //flat surface, use default colour
		{
			convex.setFillColor(sf::Color(196, 55, 98));
		}
		else if (convex.getPoint(0).y < convex.getPoint(1).y) //decline, use shade colour
		{
			convex.setFillColor(sf::Color(119, 24, 52));
		}
		else //incline, use highlight colour
		{			
			convex.setFillColor(sf::Color(222, 112, 145));
		}				

		m_surfaceShapes.push_back(convex);		

		// define the points for the rim highlight, reusing the convex shape from before
		convex.setPoint(0, sf::Vector2f(xPos, yPos - 4));
		convex.setPoint(1, sf::Vector2f(xPos + width, yPos + height - 4));
		convex.setPoint(2, sf::Vector2f(xPos + width, yPos + height));
		convex.setPoint(3, sf::Vector2f(xPos, yPos));
		convex.setFillColor(sf::Color(225, 155, 176)); //set the rim highlight colour
		m_surfaceRimShapes.push_back(convex); 		
	
		currentWidth += width;
		xPos = currentWidth;
		yPos += height;
	}
}

void Background::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const Layer& layer : m_layers)
	{
		target.draw(layer);
	}

	for (int i = 0; i < m_surfaceShapes.size(); i++)
	{
		target.draw(m_surfaceShapes[i]);
	}

	for (int i = 0; i < m_surfaceRimShapes.size(); i++)
	{
		target.draw(m_surfaceRimShapes[i]);
	}
}

void Background::update(float worldVelX)
{
	for (Layer& layer : m_layers)
	{
		layer.update(worldVelX);
	}

	for (int i = 0; i < m_surfaceShapes.size(); i++)
	{
		m_surfaceShapes[i].move(sf::Vector2f(worldVelX, 0));
	}

	for (int i = 0; i < m_surfaceRimShapes.size(); i++)
	{
		m_surfaceRimShapes[i].move(sf::Vector2f(worldVelX, 0));
	}
}

sf::Vector2u Background::getWorldSize() const
{ 
	return m_layers.back().getTotalSectionSize(); //world size is the total section size of the last layer (the surface)
}
