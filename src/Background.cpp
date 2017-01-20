#include "Background.h"
#include <iostream>

//TODO: good to use shared_ptr here?
Background::Background(const sf::FloatRect& bounds, const std::shared_ptr<GameObject>& player)
{
	m_layers.push_back(std::make_unique<Layer>(Layer("assets/sprites/enviroment/stars", bounds, 3, player, 0.8f)));
	m_layers.push_back(std::make_unique<Layer>(Layer("assets/sprites/enviroment/mountains", bounds, 7, player, 0.2222222f)));
	//m_layers.push_back(std::make_unique<Layer>(Layer("assets/sprites/enviroment/surface", bounds, 9, player, NULL, gameObjects)));
	calculateSurfacePoints(sf::Vector2f(bounds.width, bounds.height));
}

void Background::calculateSurfacePoints(const sf::Vector2f& screenSize)
{
	//variables to store the positional data of each shape
	int xPos = 0;
	int yPos = (int)(screenSize.y * 0.75f);

	const int worldWidth = (int)screenSize.x * 9;
	const int screenUnit = (int)(screenSize.x / 128);
	int allShapesWidth = 0;	

	while (allShapesWidth < worldWidth)
	{
		int width = (rand() % 16 + 4) * screenUnit;
		int height = 0;

		if (pointCoords.size() != 0)
		{
			if (pointCoords.size() % 4 == 0) //make sure there's a flat piece between every slope by creating a slope every 2nd shape
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
					(allShapesWidth > worldWidth * 0.9f && yPos > pointCoords[0].y))
				{
					height *= -1;
				}			

				//if near the end of the surface generation try to make the ends meet by adjusting the slope
				if (allShapesWidth > worldWidth * 0.975f && yPos + height != pointCoords[0].y)
				{
					if (yPos + height < pointCoords[0].y)
						height += pointCoords[0].y - (yPos + height);
					else					
						height -= (yPos + height) - pointCoords[0].y;

					width = abs(height);
				}
			}
			else //only do this on flat surface
			{
				//make sure the shape is near the very end of the generated surface
				// check if the current shapes yPos is equal to the first shape, if so extend the width of the shape to "meet" first shape
				if (allShapesWidth > worldWidth * 0.975f && yPos == pointCoords[0].y)
				{
					width = worldWidth - allShapesWidth;					
				}
			}								
		}	

		pointCoords.push_back(sf::Vector2i(xPos, yPos));					//left
		pointCoords.push_back(sf::Vector2i(xPos + width, yPos + height));	//right
		allShapesWidth += width;
		xPos = allShapesWidth;
		yPos += height;
	}

	createSurface(screenSize, screenUnit);
}

void Background::createSurface(const sf::Vector2f& screenSize, int screenUnit)
{
	sf::ConvexShape convex; //each segment of the surface is a convex shape
	// give the convex shape 4 points, every part of the surface is a rect or trapezoid
	convex.setPointCount(4);

	for (int i = 0; i < pointCoords.size(); i += 2)
	{
		// define the points
		convex.setPoint(0, sf::Vector2f(pointCoords[i].x, pointCoords[i].y));			//top left
		convex.setPoint(1, sf::Vector2f(pointCoords[i + 1].x, pointCoords[i + 1].y));	//top right
		convex.setPoint(2, sf::Vector2f(pointCoords[i + 1].x, screenSize.y));			//bottom right
		convex.setPoint(3, sf::Vector2f(pointCoords[i].x, screenSize.y));				//bottom left

		if (convex.getPoint(0).y == convex.getPoint(1).y)
			convex.setFillColor(sf::Color(196, 55, 98));	//flat surface, use default colour	
		else if (convex.getPoint(0).y < convex.getPoint(1).y)
			convex.setFillColor(sf::Color(119, 24, 52));	//decline, use shade colour	
		else
			convex.setFillColor(sf::Color(222, 112, 145));	//incline, use highlight colour		

		m_surfaceShapes.push_back(convex); //add the shape to the shape vector

		// define the points for the rim highlight, reusing the convex shape from before
		convex.setPoint(0, sf::Vector2f(pointCoords[i].x, pointCoords[i].y - (screenUnit * 0.5f)));				//top left
		convex.setPoint(1, sf::Vector2f(pointCoords[i + 1].x, pointCoords[i + 1].y - (screenUnit * 0.5f)));		//top right
		convex.setPoint(2, sf::Vector2f(pointCoords[i + 1].x, pointCoords[i + 1].y));							//bottom right
		convex.setPoint(3, sf::Vector2f(pointCoords[i].x, pointCoords[i].y));									//bottom left
		convex.setFillColor(sf::Color(225, 155, 176));	//set the rim highlight colour
		m_surfaceShapes.push_back(convex);
	}
}

void Background::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const std::unique_ptr<Layer>& layer : m_layers)
	{
		target.draw(*layer);
	}

	for (int i = 0; i < m_surfaceShapes.size(); i++)
	{
		sf::FloatRect rect = m_surfaceShapes[i].getGlobalBounds();
		//if draw only rects within the camera bounds or at either end camera width of the world
		if (rect.intersects(m_cameraBounds) || rect.left < m_cameraBounds.width || rect.left > m_cameraBounds.width * (Constants::WORLD_SCREEN_SIZES - 1))
		{
			target.draw(m_surfaceShapes[i]);
		}
	}
}

void Background::update(float dt, const sf::FloatRect& cameraBounds)
{
	m_cameraBounds = cameraBounds;
	for (std::unique_ptr<Layer>& layer : m_layers)
	{
		layer->update(dt);
	}
}

std::vector<sf::Vector2i> Background::getSurfacePath() const
{
	return pointCoords;
}