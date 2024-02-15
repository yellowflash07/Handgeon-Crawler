
#include "Engine.h"
#include "Random.h"
#include <iostream>
#include <map>
#include "LevelReader.h"
#include "Input.h"
#include "CharacterFactory.h"

extern Camera* camera;
int keyHit = 0;

Input* input;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        keyHit = key;
        input->keyHit = key;
    }
    else if (action == GLFW_RELEASE)
    {
		keyHit = 0;
        input->keyHit = 0;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera->ProcessMouseMovement(xpos, ypos);
}

int main(void)
{
    input = new Input();
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

    //read the level from json
    LevelReader levelReader(engine);
    LevelData levelData;
    levelReader.LoadLevel(levelData);

    float currTime = 0;
    float myTime = 0;

    float playerSpeed = 0;
    CharacterFactory characterFactory;
    Player* player = (Player*)characterFactory.CreateCharacter(levelData.characters[0]);

    while (!glfwWindowShouldClose(engine.window))
    {
        engine.Update();
        //rotate camera
         glm::vec3 cameraRot = glm::vec3(camera->pitch / 100.0f, -camera->yaw / 100.0f, 0);

        //follow the player
        camera->Follow(player->mesh->drawPosition, glm::vec3(0, 0.3f, 1),
            player->mesh->drawPosition, cameraRot);

        player->Update(engine.deltaTime);   

    }

    engine.ShutDown();
}
