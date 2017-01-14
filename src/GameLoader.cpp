#include "GameLoader.h"
#include <iostream> //testing
GameLoader::GameLoader(std::string const & filePath) :
	m_filePath(filePath) {
	loadData();
}

//std::vector<std::string> GameLoader::loadJSONFileNames(const std::string & animationFilePath) {
//	DIR *dir;
//	struct dirent *ent;
//	std::vector<std::string> jsonAnimFileNames;
//	if ((dir = opendir((m_filePath + animationFilePath).c_str())) != NULL) {
//		/* load all files ending in .json into a vector */
//		while ((ent = readdir(dir)) != NULL) {
//			if (ent->d_namlen > 5 &&
//				ent->d_name[ent->d_namlen - 5] == '.' &&
//				ent->d_name[ent->d_namlen - 4] == 'j' &&
//				ent->d_name[ent->d_namlen - 3] == 's' &&
//				ent->d_name[ent->d_namlen - 2] == 'o' &&
//				ent->d_name[ent->d_namlen - 1] == 'n') {
//				jsonAnimFileNames.push_back(ent->d_name);
//			}
//		}
//		closedir(dir);
//	}
//	else {
//		/* could not open directory */
//		perror("");
//	}
//	return jsonAnimFileNames;
//}

void GameLoader::loadJSONDATA(std::string const & filename) {
	std::ifstream file(filename);
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			m_JSONData.append(line);
		}
	}
}

void GameLoader::loadData() {
	std::shared_ptr<GameData> ptr = GameData::getInstance();
	m_JSONData.clear();
	loadJSONDATA(m_filePath + "data.json");
	m_document.Parse<0>(m_JSONData.c_str());

	Value::ConstMemberIterator it = m_document.MemberBegin();
	std::string animationsPath = it->value.GetString();
	++it;
	std::string soundsPath = it->value.GetString();
	++it;
	std::string imagesPath = it->value.GetString();
	++it;
	std::string mapsPath = it->value.GetString();
	++it;

	//textures
	Value::ConstMemberIterator texturesIT = it->value.MemberBegin();
	//ptr->projectileTexture.loadFromFile(m_filePath + imagesPath + texturesIT->value.GetString());
	//++texturesIT;


	//character properties
	//Value::ConstMemberIterator charPropsIT = it->value.MemberBegin();
	//Value::ConstMemberIterator charPropsITEnd = it->value.MemberEnd();
	//test = 1;
	//GameData::CharInfo* info;
	//for (; charPropsIT != charPropsITEnd; ++charPropsIT) {
	//	std::string charName = charPropsIT->name.GetString();
	//	Value::ConstMemberIterator props = charPropsIT->value.MemberBegin();
	//	info = findCurrentChar(charName);
	//	assert(info);
	//	info->maxHealth = props->value.GetDouble();
	//	++props;
	//	info->maxSpeed = props->value.GetDouble();
	//	setCollisionFilter(charName, info);
	//	std::cout << charName << " property" << test++ << std::endl;
	//}
}

//void GameLoader::setCollisionFilter(std::string charName, GameData::CharInfo* info) {
//	if (charName == "player") {
//		info->filterCategory = (uint16)CollisionFilters::PLAYER;
//		info->filterMask = (uint16)CollisionFilters::ENEMY | (uint16)CollisionFilters::COLLIDABLE;
//		info->filterSensor = (uint16)CollisionFilters::ENEMY;
//	}
//	else if (charName == "ai" || charName == "popout") {
//		info->filterCategory = (uint16)CollisionFilters::ENEMY;
//		info->filterMask = (uint16)CollisionFilters::PLAYER;// | (uint16)CollisionFilters::COLLIDABLE;
//		info->filterSensor = (uint16)CollisionFilters::PLAYER;
//	}
//}

//GameData::CharInfo* GameLoader::findCurrentChar(std::string name) {
//	std::shared_ptr<GameData> ptr = GameData::getInstance();
//	GameData::CharInfo* info;
//	if (name == "player")
//		info = &ptr->playerInfo;
//	else if (name == "ai")
//		info = &ptr->aiInfo;
//	else if (name == "popout")
//		info = &ptr->popoutInfo;
//	else
//		return nullptr;
//	return info;
//}

//void GameLoader::loadAnimations(const std::string & animationFilePath, const std::vector<std::string> & jsonAnimFileNames) {
//	//if (m_document.Size() > 0)
//	//	m_document.Clear();
//	GameData::CharInfo* info;
//	std::string charName;
//
//	int jsonFileCount = jsonAnimFileNames.size();
//	for (int i = 0; i < jsonFileCount; i++)
//	{
//		m_JSONData.clear();
//		loadJSONDATA(m_filePath + animationFilePath + jsonAnimFileNames[i]);
//		m_document.Parse<0>(m_JSONData.c_str());
//
//		Value::ConstMemberIterator it = m_document.MemberBegin(); //iterator for entire object
//		charName = it->value.GetString();
//		info = findCurrentChar(charName);
//		std::cout << charName << std::endl;
//		assert(info);
//
//		it++;//now has play speed
//		info->playSpeed = 1.f / it->value.GetInt();
//		it++; //now has scale
//		info->spriteScale = it->value.GetDouble();
//		it++;//now has the animations array
//
//		for (Value::ConstValueIterator itA = it->value.Begin(); itA < it->value.End(); itA++) {//iterator for animations array loop for each animation object
//			Value::ConstMemberIterator itAO = itA->MemberBegin();//iterator for object in animations array
//			Animation a;
//			std::string name = itAO->value.GetString();
//			itAO++;//now has name of spritesheet
//			std::shared_ptr<sf::Texture> tex = std::make_shared<sf::Texture>(sf::Texture());
//			tex->loadFromFile(m_filePath + animationFilePath + itAO->value.GetString());
//			itAO++;//now has frames array
//			sf::IntRect r;
//			for (Value::ConstValueIterator itFrames = itAO->value.Begin(); itFrames < itAO->value.End(); itFrames++) {//iterator for frames loop for each frame object
//				Value const & frameObject = (*itFrames)["frame"]; //get the frame object from this object in frames. the frame object contains pos and size info
//				r.left = frameObject["x"].GetInt();
//				r.top = frameObject["y"].GetInt();
//				r.width = frameObject["w"].GetInt();
//				r.height = frameObject["h"].GetInt();
//				a.addFrame(r);
//			}
//			info->textures.push_back(tex);
//			a.setSpriteSheet(*info->textures.back());
//			info->anims[name] = a;
//		}
//	}
//}