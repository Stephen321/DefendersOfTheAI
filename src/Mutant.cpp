//#include <string>
//#include "Mutant.h"
//
//Mutant::Mutant(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameMutants,
//	std::shared_ptr<GameObject> player, GameObjectPtrVector& gameProjectiles)
//	: AI(Type::Mutant, startPos, worldSize)
//	, m_gameMutants(gameMutants)
//	, m_gameProjectiles(gameProjectiles)
//	, m_player(player)
//	, HIGHEST_DISTANCE(worldSize.y * 0.35f)
//	, ABDUCTION_RANGE(worldSize.y * 0.55f) //lowest astronaut is worldSie.y * 0.9
//	, m_reachedTarget(false)
//	, m_abducting(false)
//	, m_abductionVictim(nullptr)
//	, m_reachedAbductionTarget(false)
//	, m_abductionOffset(sf::Vector2f(0.f, -100))
//
//{
//	m_fsm.init(this);
//	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
//	m_targetInZone = Helpers::randomNumberF(HIGHEST_DISTANCE + halfHeight, LOWEST_DISTANCE - halfHeight); // where in the target zone we should go
//	if (m_targetInZone < m_position.y) //if target is above the player then no need to move up to reach it
//	{
//		m_targetInZone = m_position.y;
//	}
//	int neighbourCount = getNeighbourCount();
//	if (neighbourCount == 0)
//	{
//		m_fsm.changeState(ADropState::getInstance());
//	}
//	else
//	{
//		m_fsm.changeState(ADropState::getInstance());
//	}
//
//
//	//testing
//	m_beamRect = sf::RectangleShape(sf::Vector2f(BEAM_SIZE, BEAM_SIZE));
//	m_beamRect.setOrigin(0.f, m_beamRect.getSize().y * 0.5f);
//	m_beamRect.setPosition(-m_position);
//	m_beamRect.setFillColor(sf::Color(200, 0, 0, 40));
//}
//
//void Mutant::fire(float dt)
//{
//	m_reloadTimer += dt;
//	if (m_reloadTimer < RELOAD_TIME)
//		return;
//	sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_player->getPosition());
//	if (Helpers::getLength(vectorBetween) < PLAYER_LASER_RANGE)
//	{
//		m_reloadTimer = 0.f;
//		m_gameProjectiles.push_back(std::shared_ptr<Laser>(new Laser(m_position, m_worldSize, Helpers::normaliseCopy(vectorBetween), LASER_VEL_SCALE)));
//	}
//}
//
//// Function that checks and modifies the distance
//// of a Mutant if it breaks the law of separation.
//sf::Vector2f Mutant::separation()
//{
//	sf::Vector2f steer(0, 0);
//	int count = 0;
//	// For every Mutant in the system, check if it's too close
//	for (int i = 0; i < m_gameMutants.size(); i++)
//	{
//		// Calculate distance from current Mutant to Mutant we're looking at
//		float d = Helpers::getLength(m_position - m_gameMutants[i]->getPosition());
//		// If this is a fellow Mutant and it's too close, move away from it
//		if (d > 0 && d < DESIRED_SEPARATION)
//		{
//			sf::Vector2f diff = m_position - m_gameMutants[i]->getPosition();
//			Helpers::normalise(diff);
//			diff /= d;      // Weight by distance. Further away doesnt influence as much
//			steer += diff;
//			count++;
//		}
//	}
//	//check if player is nearby
//	if (m_player->getPosition().y > HIGHEST_DISTANCE - m_player->getHeight())
//	{
//		float d = Helpers::getLength(Helpers::getVectorBetweenWrap(m_worldSize, m_player->getPosition(), m_position));
//		// If this is a fellow Mutant and it's too close, move away from it
//		if (d < PLAYER_DESIRED_SEPARATION)
//		{
//			sf::Vector2f diff = Helpers::getVectorBetweenWrap(m_worldSize, m_player->getPosition(), m_position);
//			Helpers::normalise(diff);
//			diff /= d * PLAYER_SEPERATION_FORCE_SCALE;
//			steer += diff * PLAYER_FORCE_SCALER;
//			count++;
//		}
//	}
//
//	// Adds average difference of m_position to m_acceleration
//	if (count > 0)
//		steer /= (float)count;
//	if (Helpers::getLength(steer) > 0)
//	{
//		// Steering = Desired - Velocity
//		Helpers::normalise(steer);
//		steer *= m_maxVelocity;
//		steer -= m_velocity;
//		Helpers::limit(steer, m_forceAmount);
//	}
//	return steer * SEPERATION_WEIGHT;
//}
//
//// Alignment calculates the average velocity in the field of view and 
//// manipulates the velocity of the Mutant passed as parameter to adjust to that
//// of nearby Mutants.
//sf::Vector2f Mutant::alignment()
//{
//	sf::Vector2f sum(0, 0);
//	int count = 0;
//	for (int i = 0; i < m_gameMutants.size(); i++)
//	{
//		float d = Helpers::getLength(m_position - m_gameMutants[i]->getPosition());
//		if (d > 0 && d < NEIGHBOUR_RADIUS)
//		{
//			sum += m_gameMutants[i]->getVelocity();
//			count++;
//		}
//	}
//	// If there are Mutants close enough for alignment...
//	if (count > 0 && m_abducting == false)
//	{
//		sum /= (float)count;// Divide sum by the number of close Mutants (average of velocity)
//		Helpers::normalise(sum);	   		// Turn sum into a unit vector, and
//		sum *= m_maxVelocity;    // Multiply by maxSpeed
//								 // Steer = Desired - Velocity
//		sf::Vector2f steer;
//		steer = sum - m_velocity; //sum = desired(average)  
//		Helpers::limit(steer, m_forceAmount);
//		return steer * ALIGNMENT_WEIGHT;
//	}
//	else {
//		sf::Vector2f temp(0, 0);
//		return temp;
//	}
//}
//
//// Cohesion finds the average m_position of nearby Mutants and manipulates the 
//// steering force to move in that direction.
//sf::Vector2f Mutant::cohesion()
//{
//	sf::Vector2f sum(0, 0);
//	int count = 0;
//	for (int i = 0; i < m_gameMutants.size(); i++)
//	{
//		float d = Helpers::getLength(m_position - m_gameMutants[i]->getPosition());
//		if (d > 0 && d < NEIGHBOUR_RADIUS)
//		{
//			sum += m_gameMutants[i]->getPosition();
//			count++;
//		}
//	}
//	if (count > 0 && m_abducting == false)
//	{
//		sum /= (float)count;
//		return seek(sum) * COHESION_WEIGHT;
//	}
//	else {
//		return sf::Vector2f();
//	}
//}
//
//// Seek function limits the maxSpeed, finds necessary steering force and
//// normalizes the vectors.
//sf::Vector2f Mutant::seek(const sf::Vector2f& v)
//{
//	sf::Vector2f desired = v - m_position;  // A vector pointing from the m_position to the target
//											// Normalize desired and scale to maximum speed
//	Helpers::normalise(desired);
//	desired *= m_maxVelocity;
//	// Steering = Desired minus Velocity
//	sf::Vector2f steer;
//	steer = desired - m_velocity;
//	Helpers::limit(steer, m_forceAmount);  // Limit to maximum steering force
//	return steer;
//}
//
//void Mutant::setAcceleration(const sf::Vector2f & acceleration)
//{
//	m_acceleration = acceleration;
//}
//
//void Mutant::checkWorldBounds()
//{
//	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
//	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
//	if (m_position.y < HIGHEST_DISTANCE + halfHeight)
//	{
//		m_position.y = HIGHEST_DISTANCE + halfHeight;
//		m_velocity.y = -m_velocity.y * 0.9f;
//	}
//	else if (m_position.y > LOWEST_DISTANCE - halfHeight)
//	{
//		m_position.y = LOWEST_DISTANCE - halfHeight;
//		m_velocity.y = -m_velocity.y * 0.9f;
//	}
//	if (m_position.x < -halfWidth)
//	{
//		m_position.x = m_worldSize.x - halfWidth;
//	}
//	else if (m_position.x > m_worldSize.x + halfWidth)
//	{
//		m_position.x = halfWidth;
//	}
//}
//
//bool Mutant::getAbducting() const
//{
//	return m_abducting;
//}
//
//void Mutant::setAbducting(bool value)
//{
//	m_abducting = value;
//}
//
//void Mutant::setAbductingVictim(const std::shared_ptr<Astronaut>& abductionVictim)
//{
//	m_abductionVictim = abductionVictim;
//	m_abductionTargetPos = m_abductionVictim->getPosition();
//	m_abductionTargetPos += m_abductionOffset;
//}
//
//void Mutant::updateAbduction(float dt)
//{
//	//if not above victim move there
//	//now above target so stop victim moving
//	//start ascending with victim locking beneath you on its y
//	//reach top of screen 
//	//astro checks world bounds and sets active to false
//	//turn self into mutant
//	if (m_reachedAbductionTarget == false)
//	{
//		//update target pos
//		m_abductionTargetPos = m_abductionVictim->getPosition();
//		m_abductionTargetPos += m_abductionOffset;
//
//		float distance = Helpers::getLength(m_abductionTargetPos);
//		float speed = Helpers::getLength(m_velocity);
//		float prediction;
//		if (speed <= distance / MAX_PREDICTION)
//		{
//			prediction = MAX_PREDICTION;
//		}
//		else
//		{
//			prediction = distance / speed;
//		}
//
//		m_abductionTargetPos.x += m_abductionVictim->getVelocity().x * prediction;
//
//		sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_abductionTargetPos);
//		float d = Helpers::getLength(vectorBetween);
//
//
//		if (d < ABDUCT_SEEK_RANGE)
//		{
//			m_velocity.y = 0.f;
//			m_reachedAbductionTarget = true;
//			m_abductionVictim->setBeingAbducted(true); //caught victim so abduct!
//		}
//
//		float targetSpeed;
//
//		if (d > ABDUCT_ARRIVE_RADIUS)
//		{
//			targetSpeed = m_maxVelocity;
//		}
//		else
//		{
//			targetSpeed = m_maxVelocity * distance / ABDUCT_ARRIVE_RADIUS;
//		}
//
//		sf::Vector2f targetVelocity = Helpers::normaliseCopy(vectorBetween) * targetSpeed;
//		if (abs(m_velocity.x) > MIN_VEL)
//		{
//			m_acceleration.x = m_dragCoefficent * -m_velocity.x;
//		}
//		if (abs(m_velocity.y) > MIN_VEL)
//		{
//			m_acceleration.y = m_dragCoefficent * -m_velocity.y;
//		}
//		m_acceleration += targetVelocity - m_velocity;
//		m_acceleration /= ABDUCT_TIME_TO_ARRIVE;
//		Helpers::limit(m_acceleration, m_forceAmount);
//	}
//	else //above victim
//	{
//		m_beamRect.setFillColor(sf::Color(255, 0, 0, 200));
//		m_velocity = sf::Vector2f(0, -m_maxVelocity * ASCEND_SCALE); //ascend
//		m_abductionVictim->setTarget(sf::Vector2f(m_position.x, m_position.y + getHeight()));
//	}
//	sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_abductionVictim->getPosition());
//	sf::Vector2f dir = Helpers::normaliseCopy(vectorBetween);
//	float d = Helpers::getLength(vectorBetween);
//	m_beamRect.setPosition(m_position);
//	m_beamRect.setSize(sf::Vector2f(d, m_beamRect.getSize().y));
//	m_beamRect.setRotation(atan2(dir.y, dir.x) * (180.f / M_PI));
//}
//
//void Mutant::checkAbductionBounds()
//{
//	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
//	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
//	if (m_position.y < m_abductionOffset.y - m_abductionVictim->getHeight())
//	{
//		m_position.y = m_abductionOffset.y - m_abductionVictim->getHeight();
//		m_velocity.y = 0.f;
//		//TODO: successful abduction
//	}
//	else if (m_position.y > m_abductionVictim->getPosition().y - m_abductionVictim->getHeight())
//	{
//		m_position.y = m_abductionVictim->getPosition().y - m_abductionVictim->getHeight();
//		m_velocity.y = 0;
//	}
//	if (m_position.x < -halfWidth)
//	{
//		m_position.x = m_worldSize.x - halfWidth;
//	}
//	else if (m_position.x > m_worldSize.x + halfWidth)
//	{
//		m_position.x = halfWidth;
//	}
//}
//
//bool Mutant::checkIfVictim(const std::shared_ptr<GameObject>& astroObject)
//{
//	return (Helpers::getLength(Helpers::getVectorBetweenWrap(m_worldSize, m_position, astroObject->getPosition())) < ABDUCTION_RANGE &&
//		Helpers::getVectorBetweenWrap(m_worldSize, m_position, astroObject->getPosition()).x < ABDUCTION_X_DIFF &&
//		m_abducting == false);
//}
//
//void Mutant::draw(sf::RenderTarget & target, sf::RenderStates states) const
//{
//	target.draw(m_beamRect);
//	AI::draw(target, states);
//}
//
//void Mutant::move(float dt)
//{
//	m_velocity += m_acceleration  * dt; //v = u + at
//	Helpers::limit(m_velocity, m_maxVelocity);
//	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2
//
//	m_sprite.setPosition(m_position);
//}
//
//int Mutant::getNeighbourCount() const
//{
//	int count = 0;
//	for (int i = 0; i < m_gameMutants.size(); i++)
//	{
//		float d = Helpers::getLength(m_position - m_gameMutants[i]->getPosition());
//		if (this != m_gameMutants[i].get() && d < NEIGHBOUR_RADIUS)
//		{
//			count++;
//		}
//	}
//	return count;
//}
//
//float Mutant::getForceAmount() const
//{
//	return m_forceAmount;
//}
//
//void Mutant::setDirection(const sf::Vector2f & dir)
//{
//	m_dir = dir;
//}
//
//sf::Vector2f Mutant::getDirection() const
//{
//	return m_dir;
//}
//
//void Mutant::updateDropAcceleration()
//{
//	//add on the patrol accel
//	sf::Vector2f vectorBetween = sf::Vector2f(m_position.x, m_targetInZone) - m_position;
//	float distance = Helpers::getLength(vectorBetween);
//
//
//	if (abs(m_targetInZone - m_position.y) < TARGET_RANGE)
//	{
//		m_velocity.y = 0.f;
//		m_reachedTarget = true;
//		return;
//	}
//
//	float targetSpeed;
//
//	if (distance > ARRIVE_RADIUS)
//	{
//		targetSpeed = m_maxVelocity;
//	}
//	else
//	{
//		targetSpeed = m_maxVelocity * distance / ARRIVE_RADIUS;
//	}
//
//	sf::Vector2f targetVelocity = Helpers::normaliseCopy(vectorBetween) * targetSpeed;
//	m_acceleration = targetVelocity - m_velocity;
//	m_acceleration /= TIME_TO_TARGET;
//	Helpers::limit(m_acceleration, m_forceAmount);
//}
//
//void Mutant::updatePatrolAcceleration()
//{
//	m_acceleration = getForceAmount() * m_dir;
//	m_acceleration += separation();
//}
//
//bool Mutant::reachedTarget() const
//{
//	return m_reachedTarget;
//}