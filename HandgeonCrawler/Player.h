#pragma once

#include "iCharacter.h"
#include "cMesh.h"
#include  "PhysicsManager.h"
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
	PhysicsBody* physicsBody;
	float playerSpeed = 0.1f;
	GLFWwindow* window;
private:
	glm::vec3 maxVelocity = glm::vec3(0.5f);
};

