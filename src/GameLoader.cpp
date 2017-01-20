#include "GameLoader.h"
#include <iostream> //testing
GameLoader::GameLoader(const std::string& filePath) 
{
	loadData(filePath);
}

void GameLoader::loadJSONDATA(std::string const & filename) {
	std::ifstream file(filename);
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			m_JSONData.append(line);
		}
	}
}

void GameLoader::loadData(const std::string& filePath) {
	GameData& ptr = GameData::getInstance();
	m_JSONData.clear();
	loadJSONDATA(filePath + "data.json");
	m_document.Parse<0>(m_JSONData.c_str());

	Value::ConstMemberIterator it = m_document.MemberBegin();
	std::string spritesPath = it->value.GetString();
	++it;

	//object properties
	Value::ConstMemberIterator charPropsIT = it->value.MemberBegin();
	Value::ConstMemberIterator charPropsITEnd = it->value.MemberEnd();
	int id = 0;
	for (; charPropsIT != charPropsITEnd; ++id, ++charPropsIT) {
		GameData::ObjectProperties& props = ptr.getObjectProperties(id);
		Value::ConstMemberIterator propsIT = charPropsIT->value.MemberBegin();

		//texture
		props.texture.loadFromFile(filePath + spritesPath + propsIT->value.GetString());

		//physics
		props.forceAmount = (++propsIT)->value.GetFloat();
		props.dragCoefficent = (++propsIT)->value.GetFloat();
		props.maxVelocity = (++propsIT)->value.GetFloat();

		//health
		props.maxHealth = (++propsIT)->value.GetInt();
	}

	++it;
	//misc sprites
	Value::ConstMemberIterator miscIT = it->value.MemberBegin();
	ptr.hyperJumpIconTexture.loadFromFile(filePath + spritesPath + (miscIT++)->value.GetString());
	ptr.smartBombIconTexture.loadFromFile(filePath + spritesPath + (miscIT++)->value.GetString());
	ptr.hyperJumpIconBGTexture.loadFromFile(filePath + spritesPath + (miscIT)->value.GetString());
}