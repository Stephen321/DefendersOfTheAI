#ifndef GAMELOADER_H
#define GAMELOADER_H

#include "rapidjson\document.h"
#include <memory>
#include <fstream>
#include "SFML/Graphics.hpp" 
#include "GameData.h"
//#include <dirent.h>

using namespace rapidjson;

class GameLoader {
public:
	GameLoader(std::string const & filePath);
private:
	rapidjson::Document m_document;
	std::string m_JSONData;
	const std::string m_filePath;
	void loadData();
	void loadJSONDATA(std::string const & filename);
	//std::vector<std::string> loadJSONFileNames(const std::string & animationFilePath);
	//void loadAnimations(const std::string & animationFilePath, const std::vector<std::string> & jsonAnimFileNames);
	//GameData::CharInfo* findCurrentChar(std::string name);
	//void setCollisionFilter(std::string charName, GameData::CharInfo* info);
};

#endif 