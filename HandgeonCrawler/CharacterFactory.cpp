#include "CharacterFactory.h"

CharacterFactory::CharacterFactory()
{
}

CharacterFactory::~CharacterFactory()
{
}

iCharacter* CharacterFactory::CreateCharacter(Character* character)
{
    if (character->name == "Player")
    {
		Player* player = new Player();
		player->mesh = character->mesh;
		player->mesh->drawScale = glm::vec3(20, 20, -20);

		player->playerSpeed = character->speed;
		return player;
	}
    return nullptr;
}
