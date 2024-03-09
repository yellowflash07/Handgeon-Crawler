
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

    cMesh* playerModel = engine.meshManager->FindMeshByFriendlyName("Player");
 
	PhysicsBody* playerPB = engine.AddPhysicsBody("Player");
    playerPB->inverseMass = 1.0f;
    playerPB->shapeType = PhysicsShapes::AABB;
    playerPB->setShape(new PhysicsShapes::sAABB(glm::vec3(0), glm::vec3(0)));
    engine.physicsManager->GenerateAABBs(playerPB, 1);

    player->physicsBody = playerPB;
    
    std::map<cMesh*, cAABB*> playerAABBMesh;
    for (size_t i = 0; i < playerPB->aabbs.size(); i++)
    {
        cMesh* aabbMesh1 = engine.LoadMesh("Cube_1x1x1_xyz_n_rgba.ply", "playerAABBMesh" + std::to_string(i));
        aabbMesh1->drawScale = playerPB->aabbs[i]->getExtentsXYZ();
        aabbMesh1->drawPosition = playerPB->aabbs[i]->getCentreXYZ();
        aabbMesh1->bDoNotLight = true;
        aabbMesh1->bUseDebugColours = true;
        aabbMesh1->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        aabbMesh1->drawPosition = playerPB->aabbs[i]->getCentreXYZ();
        aabbMesh1->bIsWireframe = true;
        playerAABBMesh[aabbMesh1] = playerPB->aabbs[i];
    }


 //   cMesh* SomeCube =engine.LoadMesh("PuppetHand.ply", "SomeCube");
 //   SomeCube ->drawPosition = glm::vec3(-15, 50, -10);
 //   SomeCube->setUniformDrawScale(20);
 //   SomeCube->bUseDebugColours = true;
 //   SomeCube->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

 //   PhysicsBody* some_cube_p_body = engine.AddPhysicsBody("SomeCube");
 //   some_cube_p_body->shapeType = PhysicsShapes::AABB;
 //   some_cube_p_body->setShape(new PhysicsShapes::sAABB(glm::vec3(0), glm::vec3(0)));
 //   engine.physicsManager->GenerateAABBs(some_cube_p_body, 1);




 //   std::map<cMesh*, cAABB*> someCubeAABBMesh;
 //   for (size_t i = 0; i < some_cube_p_body->aabbs.size(); i++)
 //   {
 //       cMesh* aabbMesh2 = engine.LoadMesh("Cube_1x1x1_xyz_n_rgba.ply", "someCubeAABBMesh" + std::to_string(i));
 //       aabbMesh2->drawScale = some_cube_p_body->aabbs[i]->getExtentsXYZ();
 //       aabbMesh2->drawPosition = some_cube_p_body->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bDoNotLight = true;
 //       aabbMesh2->bUseDebugColours = true;
 //       aabbMesh2->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
 //       aabbMesh2->drawPosition = some_cube_p_body->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bIsWireframe = true;
 //       someCubeAABBMesh[aabbMesh2] = some_cube_p_body->aabbs[i];
 //   }



 //   cMesh* wall1room1 = engine.LoadMesh("wall.fbx", "wall1room1");
 //   wall1room1->drawPosition = glm::vec3(-273.386, 21.1555, 309.443);
 //   wall1room1->drawScale = glm::vec3(0.5 ,2 ,10);
 //   wall1room1->bUseDebugColours = true;
 //   wall1room1->texture[0] = "colormap.bmp";
 //   wall1room1->textureRatio[0] = 1.0f;


	//PhysicsBody* wall1_pb = engine.AddPhysicsBody("wall1room1");
 //   wall1_pb->shapeType = PhysicsShapes::AABB;
 //   wall1_pb->setShape(new PhysicsShapes::sAABB(glm::vec3(0), glm::vec3(0)));
 //   engine.physicsManager->GenerateAABBs(wall1_pb, 1);

 //   std::map<cMesh*, cAABB*> wall1AABBMesh;
 //   for (size_t i = 0; i < wall1_pb->aabbs.size(); i++)
 //   {
 //       cMesh* aabbMesh2 = engine.LoadMesh("Cube_1x1x1_xyz_n_rgba.ply", "wall1AABBMesh" + std::to_string(i));
 //       aabbMesh2->drawScale = wall1_pb->aabbs[i]->getExtentsXYZ();
 //       aabbMesh2->drawPosition = wall1_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bDoNotLight = true;
 //       aabbMesh2->bUseDebugColours = true;
 //       aabbMesh2->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
 //       aabbMesh2->drawPosition = wall1_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bIsWireframe = true;
 //       wall1AABBMesh[aabbMesh2] = wall1_pb->aabbs[i];
 //   }

 //   cMesh* wall2room1 = engine.LoadMesh("wall.fbx", "wall2room1");
 //   wall2room1->drawPosition = glm::vec3(761.311, 0 ,293.422);
 //   wall2room1->drawScale = glm::vec3(0.5, 2, 10);
 //   wall2room1->texture[0] = "colormap.bmp";
 //   wall2room1->textureRatio[0] = 1.0f;

 //   PhysicsBody* wall2_pb = engine.AddPhysicsBody("wall2room1");
 //   wall2_pb->shapeType = PhysicsShapes::AABB;
 //   wall2_pb->setShape(new PhysicsShapes::sAABB(glm::vec3(0), glm::vec3(0)));
 //   engine.physicsManager->GenerateAABBs(wall2_pb, 1);

 //   std::map<cMesh*, cAABB*> wall2AABBMesh;
 //   for (size_t i = 0; i < wall2_pb->aabbs.size(); i++)
 //   {
 //       cMesh* aabbMesh2 = engine.LoadMesh("Cube_1x1x1_xyz_n_rgba.ply", "wall2AABBMesh" + std::to_string(i));
 //       aabbMesh2->drawScale = wall2_pb->aabbs[i]->getExtentsXYZ();
 //       aabbMesh2->drawPosition = wall2_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bDoNotLight = true;
 //       aabbMesh2->bUseDebugColours = true;
 //       aabbMesh2->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
 //       aabbMesh2->drawPosition = wall2_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bIsWireframe = true;
 //       wall2AABBMesh[aabbMesh2] = wall2_pb->aabbs[i];
 //   }


 //   cMesh* wall3room1 = engine.LoadMesh("wall.fbx", "wall3room1");
 //   wall3room1->drawPosition = glm::vec3(242.702 ,0 ,- 181.339);
 //   wall3room1->drawScale = glm::vec3(10 ,2 ,0.5);
 //   wall3room1->texture[0] = "colormap.bmp";
 //   wall3room1->textureRatio[0] = 1.0f;

 //   PhysicsBody* wall3_pb = engine.AddPhysicsBody("wall3room1");
 //   wall3_pb->shapeType = PhysicsShapes::AABB;
 //   wall3_pb->setShape(new PhysicsShapes::sAABB(glm::vec3(0), glm::vec3(0)));
 //   engine.physicsManager->GenerateAABBs(wall3_pb, 1);

 //   std::map<cMesh*, cAABB*> wall3AABBMesh;
 //   for (size_t i = 0; i < wall3_pb->aabbs.size(); i++)
 //   {
 //       cMesh* aabbMesh2 = engine.LoadMesh("Cube_1x1x1_xyz_n_rgba.ply", "wall3AABBMesh" + std::to_string(i));
 //       aabbMesh2->drawScale = wall3_pb->aabbs[i]->getExtentsXYZ();
 //       aabbMesh2->drawPosition = wall3_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bDoNotLight = true;
 //       aabbMesh2->bUseDebugColours = true;
 //       aabbMesh2->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
 //       aabbMesh2->drawPosition = wall3_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bIsWireframe = true;
 //       wall3AABBMesh[aabbMesh2] = wall3_pb->aabbs[i];
 //   }

 //   cMesh* wall4room1 = engine.LoadMesh("wall.fbx", "wall4room1");
 //   wall4room1->drawPosition = glm::vec3(108.585, 0 ,782.058);
 //   wall4room1->drawScale = glm::vec3(10, 2, 0.5);
 //   wall4room1-> transperancy = 1.0f;
 //   wall4room1-> texture[0] ="colormap.bmp";
 //   wall4room1-> textureRatio[0] = 1.0f;


 //   PhysicsBody* wall4_pb = engine.AddPhysicsBody("wall4room1");
 //   wall4_pb->shapeType = PhysicsShapes::AABB;
 //   wall4_pb->setShape(new PhysicsShapes::sAABB(glm::vec3(0), glm::vec3(0)));
 //   engine.physicsManager->GenerateAABBs(wall4_pb, 1);

 //   std::map<cMesh*, cAABB*> wall4AABBMesh;
 //   for (size_t i = 0; i < wall4_pb->aabbs.size(); i++)
 //   {
 //       cMesh* aabbMesh2 = engine.LoadMesh("Cube_1x1x1_xyz_n_rgba.ply", "wall4AABBMesh" + std::to_string(i));
 //       aabbMesh2->drawScale = wall4_pb->aabbs[i]->getExtentsXYZ();
 //       aabbMesh2->drawPosition = wall4_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bDoNotLight = true;
 //       aabbMesh2->bUseDebugColours = true;
 //       aabbMesh2->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
 //       aabbMesh2->drawPosition = wall4_pb->aabbs[i]->getCentreXYZ();
 //       aabbMesh2->bIsWireframe = true;
 //       wall4AABBMesh[aabbMesh2] = wall4_pb->aabbs[i];
 //   }

 

    while (!glfwWindowShouldClose(engine.window))
    {
        engine.Update();

        //rotate camera

    	 glm::vec3 cameraRot = glm::vec3(camera->pitch / 100.0f, -camera->yaw / 100.0f, 0);

        //follow the player
      /*  camera->Follow(player->mesh->drawPosition, glm::vec3(0, 0.3f, 1),
            player->mesh->drawPosition, cameraRot);*/

       // player->Update(engine.deltaTime);

        //update player aabb meshes

        for (std::map<cMesh*, cAABB*>::iterator it = playerAABBMesh.begin(); it != playerAABBMesh.end(); it++)
        {
	       it->first->drawPosition = it->second->getCentreXYZ();
		}

    }

    engine.ShutDown();
}
