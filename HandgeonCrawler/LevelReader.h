#pragma once

#include <string>
#include <cMesh.h>
#include <JsonReader.h>
#include <Engine.h>

struct Character
{
	std::string name;
	std::string meshName;
	int health;
	int attack;
	int speed;
	glm::vec3 position;
};

struct LevelData
{
	std::vector<Character*> characters;
};


class LevelReader
{
public:
	LevelReader();
	LevelReader(Engine engine);
	~LevelReader();

	void LoadLevel(LevelData& levelData);
private:
	Engine* engine;
	void LoadCharacter(Character* character);
};

