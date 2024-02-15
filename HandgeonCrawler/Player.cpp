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
    if (input->keyHit == GLFW_KEY_W)
    {
        mesh->drawPosition += playerSpeed * -forward;
    }
    else if (input->keyHit == GLFW_KEY_S)
    {
        mesh->drawPosition += playerSpeed * forward;
    }
    else if (input->keyHit == GLFW_KEY_A)
    {
        mesh->drawPosition += playerSpeed * right;
    }
    else if (input->keyHit == GLFW_KEY_D)
    {
        mesh->drawPosition += playerSpeed * -right;
    }
}
