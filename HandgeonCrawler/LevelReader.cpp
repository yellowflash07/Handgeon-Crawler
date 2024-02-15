#include "LevelReader.h"
#include <iostream>

LevelReader::LevelReader(/*Engine engine*/)
{
	//this->engine = engine;
}

LevelReader::LevelReader(Engine engine)
{
	this->engine = &engine;
}

LevelReader::~LevelReader()
{
}

void LevelReader::LoadLevel(LevelData& levelData)
{
	JsonReader reader;
	rapidjson::Document document;

	if (!reader.LoadJsonFile("LevelData.json", document))
	{
		std::cout << "Failed to load json file" << std::endl;
		return;
	}


	if (document.HasMember("LevelData"))
	{
		rapidjson::Value& levelDataVal = document["LevelData"];

		printf("yea\n");
		if (levelDataVal.HasMember("Characters"))
		{
			rapidjson::Value& characters = document["LevelData"]["Characters"];

			for (rapidjson::SizeType i = 0; i < characters.Size(); i++)
			{
				Character* character = new Character();
				character->name = characters[i]["Name"].GetString();
				character->meshName = characters[i]["MeshName"].GetString();
				character->health = characters[i]["Health"].GetInt();
				character->attack = characters[i]["Attack"].GetInt();
				character->speed = characters[i]["Speed"].GetInt();
				character->position = glm::vec3(characters[i]["Position"][0].GetFloat(), characters[i]["Position"][1].GetFloat(), characters[i]["Position"][2].GetFloat());
				LoadCharacter(character);
				levelData.characters.push_back(character);
			}
		}
	}

	

	
}

void LevelReader::LoadCharacter(Character* character)
{
	character->mesh = engine->LoadMesh(character->meshName, character->name);
	character->mesh->drawPosition = character->position;
}
