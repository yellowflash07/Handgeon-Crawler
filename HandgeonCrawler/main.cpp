
#include "Engine.h"
#include "Random.h"
#include <iostream>
#include <map>
#include "LevelReader.h"

extern Camera* camera;
int keyHit = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        keyHit = key;
    }
    else if (action == GLFW_RELEASE)
    {
		keyHit = 0;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera->ProcessMouseMovement(xpos, ypos);
}

int main(void)
{
    Engine engine;
    if (!engine.Initialize())
    {
        return 1;
    }

    engine.meshManager->LoadTexture("Water_Texture_01.bmp");
    bool loaded = engine.meshManager->LoadCubeMap("space",
        "CubeMaps/TropicalSunnyDayLeft2048.bmp",
        "CubeMaps/TropicalSunnyDayRight2048.bmp",
        "CubeMaps/TropicalSunnyDayUp2048.bmp",
        "CubeMaps/TropicalSunnyDayDown2048.bmp",
        "CubeMaps/TropicalSunnyDayFront2048.bmp",
        "CubeMaps/TropicalSunnyDayBack2048.bmp",
        true);

    cMesh* skyBoxMesh = engine.LoadMesh("Sphere_1_unit_Radius_UV.ply", "skybox");
    skyBoxMesh->isSkyBox = true;
    skyBoxMesh->setUniformDrawScale(5000.0f);

    //Flat_1x1_plane
    cMesh* plane = engine.LoadMesh("Flat_1x1_plane.ply", "plane");
    plane->setUniformDrawScale(1000.0f);
    plane->texture[0] = "Water_Texture_01.bmp";
    plane->textureRatio[0] = 1.0f;
    camera->SetPosition(glm::vec3(0.0f, 400.0f, 200.0f));

    //read the level from json
    LevelReader levelReader(engine);
    LevelData levelData;
    levelReader.LoadLevel(levelData);

    float currTime = 0;
    float myTime = 0;

    float playerSpeed = 0;

    for (size_t i = 0; i < levelData.characters.size(); i++)
    {
        Character* character = levelData.characters[i];
        if (character->name == "Player")
        {
            playerSpeed = character->speed;
        }
    }

    cMesh* player = engine.meshManager->FindMeshByFriendlyName("Player");

    player->drawScale = glm::vec3(20, 20, -20);

    while (!glfwWindowShouldClose(engine.window))
    {
        engine.Update();
        //rotate camera
         glm::vec3 cameraRot = glm::vec3(camera->pitch / 100.0f, -camera->yaw / 100.0f, 0);

        //follow the player
        camera->Follow(player->drawPosition, glm::vec3(0, 0.3f, 1),
            player->drawPosition, cameraRot);

        player->setRotationFromEuler(glm::vec3(0, -camera->yaw / 100.0f, 0));

        //get the forward and right vectors of the player
        glm::vec3 forward = player->GetForwardVector();
        forward = glm::vec3(forward.x, 0, forward.z); // Flatten the forward vector
        glm::vec3 right = player->GetRightVector();


        //move the player
        if (keyHit == GLFW_KEY_W)
        {
            player->drawPosition += playerSpeed * -forward;
        }
        else if (keyHit == GLFW_KEY_S)
        {
            player->drawPosition += playerSpeed * forward;
        }
        else if (keyHit == GLFW_KEY_A)
        {
            player->drawPosition += playerSpeed * right;
        }
        else if (keyHit == GLFW_KEY_D)
        {
            player->drawPosition += playerSpeed * -right;
        }
    }

    engine.ShutDown();
}
