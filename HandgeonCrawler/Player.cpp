#include "Player.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    mesh->setRotationFromEuler(glm::vec3(0, -camera->yaw / 100.0f, 0));
    glm::vec3 forward = mesh->GetForwardVector();
    forward = glm::vec3(forward.x, 0, forward.z); // Flatten the forward vector
    glm::vec3 right = mesh->GetRightVector(); 
    glm::vec3 up = mesh->GetUpVector();

    if (input->keyHit == GLFW_KEY_W && input->action == GLFW_PRESS)
    {
        physicsBody->velocity += playerSpeed * -forward;
    }
    if (input->keyHit == GLFW_KEY_S && input->action == GLFW_PRESS)
    {
        physicsBody->velocity += playerSpeed * forward;
    }
    if (input->keyHit == GLFW_KEY_A && input->action == GLFW_PRESS)
    {
        physicsBody->velocity += playerSpeed * right;
    }
    if (input->keyHit == GLFW_KEY_D && input->action == GLFW_PRESS)
    {
        physicsBody->velocity += playerSpeed * -right;
    }
    if (input->keyHit == GLFW_KEY_SPACE && input->action == GLFW_PRESS)
    {
        physicsBody->velocity.y = 35.0f;
    }


    /*if(input->action == GLFW_RELEASE)
    {
		physicsBody->velocity = glm::vec3(0.0f);
	}*/
}
