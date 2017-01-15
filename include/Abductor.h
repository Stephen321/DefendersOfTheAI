#pragma once
#include "AI.h"
#include <vector>

class Abductor : public AI<Abductor>
{
public:
	bool predator;
	sf::Vector2f location;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float maxSpeed;
	float maxForce;
	Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, bool predCheck);
/* 
Destructors are commented out for now. g++ throws errors if they are included.
   If compiling on Visual Studio, however, no errors are thrown.
	//Destructor
	Boid::~Boid()
	{
		//cout << "Boid is being deleted by destructor!" << endl;
	}
*/	
	void applyForce(sf::Vector2f force);
	// Three Laws that boids follow
	sf::Vector2f Separation(const std::vector<Abductor>& Boids);
	sf::Vector2f Alignment(const std::vector<Abductor>& Boids);
	sf::Vector2f Cohesion(const std::vector<Abductor>& Boids);
	//Functions involving SFML and visualisation linking
	sf::Vector2f seek(const sf::Vector2f&  v);
	void run(float dt, const std::vector<Abductor>& v);
	void update(float dt);
	void flock(const std::vector<Abductor>& v);
	float angle(sf::Vector2f v);
	void swarm(const std::vector<Abductor>& v);
};