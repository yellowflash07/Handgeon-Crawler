#pragma once

#include "iCharacter.h"
#include "cMesh.h"
#include "Input.h"
#include "../PandaEngine/GraphicsCommon.h"
#include <Camera.h>

extern Camera* camera;
extern Input* input;

class Player : public iCharacter
{
public:
	Player();
	~Player();

	void Update(float deltaTime);
	cMesh* mesh;
	float playerSpeed = 0.1f;
};

