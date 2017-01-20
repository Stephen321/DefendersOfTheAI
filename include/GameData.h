#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <unordered_map>
#include "SFML\Graphics\Texture.hpp"

class GameData {
public:
	static GameData& getInstance();
	// delete copy and move constructors and assign operators
	GameData(GameData const&) = delete;             // Copy construct
	GameData(GameData&&) = delete;                  // Move construct
	GameData& operator=(GameData const&) = delete;  // Copy assign
	GameData& operator=(GameData &&) = delete;      // Move assign

	//object info
	struct ObjectProperties{
		ObjectProperties() {};
		sf::Texture texture;//have to be kept in memory 
		float forceAmount;
		float dragCoefficent;
		float maxVelocity;
		int maxHealth;
	};
	ObjectProperties& getObjectProperties(int id);
private:
	GameData();
	std::unordered_map<int, ObjectProperties> m_objectProperties;
};
#endif