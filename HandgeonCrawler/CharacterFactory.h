#pragma once

#include "iCharacter.h"
#include "LevelReader.h"
#include "Player.h"

class CharacterFactory
{
public:
	CharacterFactory();
	~CharacterFactory();

	iCharacter* CreateCharacter(Character* character);
};

