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
	//srand(time(NULL));
	//variables to store the positional data of each shape
	int xPos = 0;
	int yPos = screenSize.y * 0.75f;

	const int worldWidth = screenSize.x * 9;
	const int screenUnit = (screenSize.x / 128);
	int allShapesWidth = 0;

	while (allShapesWidth < worldWidth)
	{
		sf::ConvexShape convex; //each segment of the surface is a convex shape

		int width = (rand() % 16 + 4) * screenUnit;
		int height = 0;

		if (m_surfaceShapes.size() != 0)
		{
			if (m_surfaceShapes.size() % 2 == 0) //make sure there's a flat piece between every slope by creating a slope every 2nd shape
			{
				height += (rand() % 2 + 1) * screenUnit;
				width = height; //make every slope a perfect 45 degrees by ensuring width and height are equal				

				//first randomly choose a number, 1 or 0, 0 signifies negative height (incline as - on the y is vertical),
				//OR, ensure a slope goes up if the current yPos of shapes is too low on the screen, don't want the path going off screen,
				//third check makes sure the path doesn't go too high as it's inclining, which guarantees a decline to keep the path somewhat level,
				//and lastly is an override that forces the slopes to rise up if the surface is near the end of the generation, 
				//ensuring opposing ends meet on the same yPos, if the yPos is < shape 0 a decline is guaranteed
				if (((rand() % 2 == 0 || yPos > screenSize.y * 0.9) && 
					yPos > screenSize.y * 0.75f) || 
					(allShapesWidth > worldWidth * 0.9f && yPos > m_surfaceShapes[0].getPoint(0).y))
				{
					height *= -1;
				}			

				//if near the end of the surface generation try to make the ends meet by adjusting the slope
				if (allShapesWidth > worldWidth * 0.975f && yPos + height != m_surfaceShapes[0].getPoint(0).y)
				{
					if (yPos + height < m_surfaceShapes[0].getPoint(0).y)					
						height += m_surfaceShapes[0].getPoint(0).y - (yPos + height);					
					else					
						height -= (yPos + height) - m_surfaceShapes[0].getPoint(0).y;					

					width = abs(height);
				}
			}
			else //only do this on flat surface
			{
				//make sure the shape is near the very end of the generated surface
				// check if the current shapes yPos is equal to the first shape, if so extend the width of the shape to "meet" first shape
				if (allShapesWidth > worldWidth * 0.975f && yPos == m_surfaceShapes[0].getPoint(0).y)
				{
					width = worldWidth - allShapesWidth;
				}
			}								
		}				

		// give the convex shape 4 points, every part of the surcase is a rect or trapezoid
		convex.setPointCount(4);

		// define the points
		convex.setPoint(0, sf::Vector2f(xPos, yPos));						//top left
		convex.setPoint(1, sf::Vector2f(xPos + width, yPos + height));		//top right
		convex.setPoint(2, sf::Vector2f(xPos + width, screenSize.y));		//bottom right
		convex.setPoint(3, sf::Vector2f(xPos, screenSize.y));				//bottom left
		
		if (convex.getPoint(0).y == convex.getPoint(1).y)					//flat surface, use default colour		
			convex.setFillColor(sf::Color(196, 55, 98));		
		else if (convex.getPoint(0).y < convex.getPoint(1).y)				//decline, use shade colour		
			convex.setFillColor(sf::Color(119, 24, 52));		
		else																//incline, use highlight colour					
			convex.setFillColor(sf::Color(222, 112, 145));						

		m_surfaceShapes.push_back(convex); //add the shape to the shape vector

		// define the points for the rim highlight, reusing the convex shape from before
		convex.setPoint(0, sf::Vector2f(xPos, yPos - (screenUnit * 0.5f)));						//top left
		convex.setPoint(1, sf::Vector2f(xPos + width, yPos + height - (screenUnit * 0.5f)));	//top right
		convex.setPoint(2, sf::Vector2f(xPos + width, yPos + height));							//bottom right
		convex.setPoint(3, sf::Vector2f(xPos, yPos));											//bottom left
		convex.setFillColor(sf::Color(225, 155, 176));											//set the rim highlight colour
		m_surfaceRimShapes.push_back(convex); //add the 
	
		allShapesWidth += width;
		xPos = allShapesWidth;
		yPos += height;
	}

	std::cout << m_surfaceShapes[0].getPoint(0).y << "  point1Y  " << m_surfaceShapes[m_surfaceShapes.size() - 1].getPoint(0).y << "  endPoint y  " << std::endl;
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
