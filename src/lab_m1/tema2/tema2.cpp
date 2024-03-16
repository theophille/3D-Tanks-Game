#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#include "meshes.h"

using namespace std;
using namespace game;

WorldOfTanks::WorldOfTanks() {}

WorldOfTanks::~WorldOfTanks() {}

void WorldOfTanks::InitShaders() {
    Shader* shader = new Shader("Colored3D");
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Colored3D.VS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Colored3D.FS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

void WorldOfTanks::Init() {
    InitShaders();
    Meshes::getInstance()->InitMeshes(window);
    player = new Tank(PLAYER);
    camera = new FollowCamera(player, glm::vec3(2.0f, 1.3f, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, ZNEAR, ZFAR);
    terrain = new Terrain(30, 40, player);
}

void WorldOfTanks::FrameStart() {
    window->HidePointer();
    glm::vec3 skyColor = Colors::getInstance()->getColor("blue");
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void WorldOfTanks::Update(float deltaTimeSeconds) {
    player->render(shaders["Colored3D"], this, deltaTimeSeconds);
    BulletsManager::getInstance()->renderBullets(deltaTimeSeconds, shaders["Colored3D"], this);
    terrain->render(shaders["Colored3D"], this, deltaTimeSeconds);
    time += deltaTimeSeconds;

    if (time > DURATION) {
        exit(1);
    }
}

void WorldOfTanks::FrameEnd() {
}

void WorldOfTanks::RenderMesh3D(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();

    GLint locMeshColor = glGetUniformLocation(shader->program, "color");
    glUniform3fv(locMeshColor, 1, glm::value_ptr(color));

    bool isTank = false;
    GLint locIsTank = glGetUniformLocation(shader->program, "isTank");
    glUniform1i(locIsTank, isTank);

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void WorldOfTanks::RenderMesh3D(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, Tank* tank)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();

    GLint locMeshColor = glGetUniformLocation(shader->program, "color");
    glUniform3fv(locMeshColor, 1, glm::value_ptr(color));
    
    GLint locTankPos = glGetUniformLocation(shader->program, "tankPosition");
    glUniform3fv(locTankPos, 1, glm::value_ptr(tank->getPosition()));

    GLint locHp = glGetUniformLocation(shader->program, "hp");
    glUniform1i(locHp, tank->hp);

    bool isTank = true;
    GLint locIsTank = glGetUniformLocation(shader->program, "isTank");
    glUniform1i(locIsTank, isTank);

    float random = randf(0.0, 1.0);
    GLint locRandom = glGetUniformLocation(shader->program, "random");
    glUniform1f(locRandom, random);

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void WorldOfTanks::OnInputUpdate(float deltaTime, int mods) {
    if (window->KeyHold(GLFW_KEY_W)) {
        player->moveForward(deltaTime);
        camera->SetPos(camera->position + PLAYER_SPEED * player->getForward() * deltaTime);
        
        std::vector<std::vector<glm::vec3>> col = 
            player->checkForCollisions(
                terrain->getBuildings(),
                BulletsManager::getInstance()->getBullets(),
                terrain->getEnemies());

        for (int i = 0; i < col[0].size(); i++) {
            camera->SetPos(camera->position + col[0].at(i));
        }

        for (int i = 0; i < col[1].size(); i++) {
            camera->SetPos(camera->position + col[1].at(i));
        }

    }

    if (window->KeyHold(GLFW_KEY_S)) {
        player->moveForward(-deltaTime);
        camera->SetPos(camera->position - PLAYER_SPEED * player->getForward() * deltaTime);

        std::vector<std::vector<glm::vec3>> col =
            player->checkForCollisions(
                terrain->getBuildings(),
                BulletsManager::getInstance()->getBullets(),
                terrain->getEnemies());

        for (int i = 0; i < col[0].size(); i++) {
            camera->SetPos(camera->position + col[0].at(i));
        }

        for (int i = 0; i < col[1].size(); i++) {
            camera->SetPos(camera->position + col[1].at(i));
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        player->rotateRight(deltaTime);
        camera->RotateAroundSubject(player->getRotation().y);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        player->rotateRight(-deltaTime);
        camera->RotateAroundSubject(player->getRotation().y);
    }
}


void WorldOfTanks::OnKeyPress(int key, int mods) {}


void WorldOfTanks::OnKeyRelease(int key, int mods) {
    if (key == GLFW_KEY_R) {
        player->hp = 3.0f;
    }
}


void WorldOfTanks::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }

    player->deltaX = deltaX;
    player->deltaY = deltaY;
}


void WorldOfTanks::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}


void WorldOfTanks::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    if (button == 1) {
        player->fire();
    }
}


void WorldOfTanks::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}


void WorldOfTanks::OnWindowResize(int width, int height) {
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, ZNEAR, ZFAR);
}

Terrain* WorldOfTanks::getTerrain() {
    return terrain;
}
