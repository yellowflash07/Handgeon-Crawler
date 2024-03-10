
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
    keyHit = key;

    if (action == GLFW_PRESS)
    {
		input->keys[key] = true;
	}
    else if (action == GLFW_RELEASE)
    {
		input->keys[key] = false;
	}

    input->keyHit = key;
    input->scancode = scancode;
    input->action = action;
    input->mods = mods;
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

  //  engine.meshManager->LoadTexture("Ground.png");
    bool loaded = engine.meshManager->LoadCubeMap("space",
        "CubeMaps/TropicalSunnyDayLeft2048.bmp",
        "CubeMaps/TropicalSunnyDayRight2048.bmp",
        "CubeMaps/TropicalSunnyDayUp2048.bmp",
        "CubeMaps/TropicalSunnyDayDown2048.bmp",
        "CubeMaps/TropicalSunnyDayFront2048.bmp",
        "CubeMaps/TropicalSunnyDayBack2048.bmp",
        true);

    engine.LoadSave();

    cMesh* skyBoxMesh = engine.LoadMesh("Sphere_1_unit_Radius_UV.ply", "skybox");
    skyBoxMesh->isSkyBox = true;
    skyBoxMesh->setUniformDrawScale(5000.0f);

    //Flat_1x1_plane
    cMesh* plane = engine.LoadMesh("Grid_10x10_vertical_in_y.ply", "plane");
    plane->setUniformDrawScale(100.0f);
    plane->setRotationFromEuler (glm::vec3(-1.57, 0.0, 0.0));
    plane->texture[0] = "Ground.bmp";
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
    player->window = engine.window;

    //create the physics bodies for the meshes
    for (size_t i = 0; i < engine.meshManager->meshList.size(); i++)
    {
        cMesh* mesh = engine.meshManager->meshList[i];

        //skip the skybox
        if (mesh->friendlyName == "skybox")
        {
            continue;
        }

        PhysicsBody* pb = engine.AddPhysicsBody(mesh->friendlyName);
       // pb->inverseMass = 1.0f;
        pb->shapeType = PhysicsShapes::AABB;
        pb->setShape(new PhysicsShapes::sAABB(glm::vec3(0), glm::vec3(0)));
        engine.physicsManager->GenerateAABBs(pb, 1);        
    }

    player->physicsBody = engine.physicsManager->FindPhysicsBodyByFriendlyName("Player");
    player->physicsBody->isEnabled = true;
    player->physicsBody->acceleration = glm::vec3(0, -9.1f, 0);
    player->physicsBody->inverseMass = 1.0f;

    while (!glfwWindowShouldClose(engine.window))
    {
        engine.Update();

        //rotate camera
    	 glm::vec3 cameraRot = glm::vec3(camera->pitch / 100.0f, -camera->yaw / 100.0f, 0);

        ////follow the player
        camera->Follow(player->mesh->drawPosition, glm::vec3(0, 0.3f, 1),
            player->mesh->drawPosition, cameraRot);

        player->Update(engine.deltaTime);      

    }

    engine.ShutDown();
}
