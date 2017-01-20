//#include "MutantStates.h"
//
//void AFlockState::start(Mutant* abductor)
//{
//	abductor->setMoving(true);
//}
//
//void AFlockState::update(Mutant* abductor, float dt)
//{
//	int neighbourCount = abductor->getNeighbourCount();
//	if (neighbourCount == 0)
//	{
//		abductor->changeState(ADropState::getInstance());
//	}
//	sf::Vector2f sep = abductor->separation();
//	sf::Vector2f ali = abductor->alignment();
//	sf::Vector2f coh = abductor->cohesion();
//	sf::Vector2f acceleration;
//	acceleration += sep;
//	acceleration += ali;
//	acceleration += coh;
//	abductor->setAcceleration(acceleration);
//	abductor->move(dt);
//	abductor->checkWorldBounds();
//	abductor->fire(dt);
//	if (abductor->getAbducting())
//	{
//		abductor->changeState(AAbductingState::getInstance());
//	}
//}
//
//void AFlockState::end(Mutant* abductor)
//{
//}
//
////-----------------------------------------------------------------------------
//void ADropState::start(Mutant* abductor)
//{
//	abductor->setMoving(true);
//}
//
//void ADropState::update(Mutant* abductor, float dt)
//{ //TODO: this has a fixed speed not set from anywhere (player x moves at max vel * 0.5 and y is sin way
//	abductor->updateDropAcceleration();
//	abductor->move(dt);
//	if (abductor->reachedTarget())
//	{
//		abductor->changeState(APatrolState::getInstance());
//	}
//}
//
//void ADropState::end(Mutant * abductor)
//{
//}
//
////-----------------------------------------------------------------------------
//void APatrolState::start(Mutant* abductor)
//{
//	abductor->setMoving(true);
//	abductor->setDirection(sf::Vector2f((abductor->getDirection().x > 0.f) ? 1.f : -1.f, 0.f));
//}
//
//void APatrolState::update(Mutant* abductor, float dt)
//{ //TODO: this has a fixed speed not set from anywhere (player x moves at max vel * 0.5 and y is sin way
//	int neighbourCount = abductor->getNeighbourCount();
//	if (neighbourCount > 0)
//	{
//		abductor->changeState(AFlockState::getInstance());
//	}
//	abductor->updatePatrolAcceleration();
//	abductor->checkWorldBounds();
//	if (abductor->getAbducting())
//	{
//		abductor->changeState(AAbductingState::getInstance());
//	}
//}
//
//void APatrolState::end(Mutant * abductor)
//{
//}
//
//
////-----------------------------------------------------------------------------
//void AAbductingState::start(Mutant* abductor)
//{
//	abductor->setMoving(true);
//}
//
//void AAbductingState::update(Mutant* abductor, float dt)
//{
//	//TODO: arrive to target , accelerate up
//	//abductor->setAcceleration(sf::Vector2f(0, -1.f) * abductor->getForceAmount());
//	abductor->updateAbduction(dt);
//	abductor->move(dt);
//	abductor->checkAbductionBounds();
//}
//
//void AAbductingState::end(Mutant * abductor)
//{
//}
