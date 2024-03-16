#pragma once

#include "components/simple_scene.h"
#include "followCamera.h"
#include "gameConstants.h"
#include "colors.h"
#include "translate3D.h"
#include "tank.h"
#include "bullets.h"
#include "terrain.h"

namespace game
{
    class WorldOfTanks : public gfxc::SimpleScene {
    public:
        WorldOfTanks();
        ~WorldOfTanks();

        void Init() override;
        void RenderMesh3D(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);
        void RenderMesh3D(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, Tank* tank);
        Terrain* getTerrain();
        
    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void InitShaders();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        FollowCamera* camera;
        glm::mat4 projectionMatrix;
        Tank* player;
        Terrain* terrain;
        float time = 0.0f;
    };
}
