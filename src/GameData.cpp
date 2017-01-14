#include "GameData.h"

GameData::GameData() {}

GameData& GameData::getInstance() 
{
	static GameData gd;
	return gd;
}


GameData::ObjectProperties & GameData::getObjectProperties(int id)
{
	if (m_objectProperties.find(id) == m_objectProperties.end())
	{
		m_objectProperties.insert(std::pair<int, ObjectProperties>(id, ObjectProperties()));
	}
	return m_objectProperties.at(id);
}