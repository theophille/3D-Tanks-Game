#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "utils/glm_utils.h"
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/window/window_object.h"
#include "colors.h"
#include "meshes.h"
#include "boxCollider.h"
#include "sphereCollider.h"
#include "terrain.h"
#include "bullets.h"

namespace game
{
	class WorldOfTanks;
	class Tank {
	private:
		unsigned int type;
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 rotation = glm::vec3(0, 0, 0);
		float gunRot = 0.0f;
		float headRot = 0.0f;
		glm::vec3 forward = glm::vec3(-1, 0, 0);
		glm::vec3 right = glm::vec3(0, 0, -1);
		glm::vec3 up = -glm::normalize(glm::cross(forward, right));
		std::unordered_map<std::string, glm::vec3> colors;
		std::unordered_map<std::string, glm::vec3> offsets;
		std::unordered_map<std::string, Mesh*> meshes;
		Mesh* bullet;
		SphereCollider* collider = new SphereCollider(TANK_COL_RAD, glm::vec3(0));

	public:
		int deltaX = 0;
		int deltaY = 0;
		unsigned int hp = 3;
		unsigned int state = 0;
		float time = 2.0f;
		float desiredAngle = 1.0f;
		float marginCollisionAngle = 0.0f;
		float playerActiveEnemiesRadius = 5.0f;
		bool doTurn = false;
		float shotTime = 0.0f;
		bool turnAround = false;

		Tank(unsigned int type);
		Tank(unsigned int type, glm::vec3 position);

		void moveForward(float deltaTime);
		void rotateRight(float deltaTime);
		void set(glm::vec3 position, glm::vec3 rotation);

		void render(Shader* shader, WorldOfTanks* ctx, float deltaTime);
		std::vector<std::vector<glm::vec3>> checkForCollisions(std::vector<Building> buildings, std::vector<Bullet> bullets, std::vector<Tank*> tanks);
		std::vector<std::vector<glm::vec3>> checkEnemyCollisions(std::vector<Building> buildings, std::vector<Bullet> bullets, std::vector<Tank*> tanks);
		glm::vec3 buildingTankCollision(BoxCollider* boxCollider, SphereCollider* sphereCollider);
		glm::vec3 tankTankCollision(Tank* a, Tank* b);
		bool bulletTankCollision(Bullet a, Tank* b);
		void checkForBulletCollisions();
		void fire();

		glm::vec3 getPosition();
		void updatePosition(glm::vec3 position);
		glm::vec3 getRotation();
		void updateRotation(glm::vec3 rotation);
		glm::vec3 getForward();
		void updateForward(glm::vec3 forward);
		glm::vec3 getRight();
		void updateRight(glm::vec3 right);
		glm::vec3 getUp();
		void updateUp(glm::vec3 up);
		SphereCollider* getCollider();
	};
}