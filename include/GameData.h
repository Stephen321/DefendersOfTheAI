#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <memory>
#include <unordered_map>
#include "SFML\Graphics\Texture.hpp"

class GameData {
public:
	static std::shared_ptr<GameData> getInstance();

	//character info
	struct ObjectInfo{
		std::shared_ptr<sf::Texture> textures;//have to be kept in memory otherwise white square
	};
	ObjectInfo playerInfo;
	ObjectInfo laserInfo;
	ObjectInfo aiInfo;

	template<class T>
	struct T {

	};
private:
	GameData();
	T<sf::Texture> a;//wat
	static std::weak_ptr<GameData> m_instance;
	friend std::weak_ptr<GameData>;
	friend std::shared_ptr<GameData>;
};
#endif