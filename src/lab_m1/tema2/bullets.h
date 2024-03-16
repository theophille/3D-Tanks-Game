#pragma once

#include <string>
#include <vector>

#include "utils/glm_utils.h"
#include "gameConstants.h"
#include "core/gpu/shader.h"
#include "core/gpu/mesh.h"
#include "sphereCollider.h"
#include "boxCollider.h"

namespace game
{
	class Tank;
	class Terrain;
	class WorldOfTanks;

	struct Bullet {
		glm::vec3 position;
		glm::vec3 shotDirection;
		glm::vec3 speed;
		SphereCollider* collider;
		Tank* tank;
		bool collided = false;
		Bullet() : position(glm::vec3(0, 0, 0)), shotDirection(glm::vec3(0)), speed(0), collider(NULL), tank(NULL) {}
		Bullet(glm::vec3 position, glm::vec3 shotDirection, glm::vec3 speed, SphereCollider* collider, Tank* tank)
			: position(position), shotDirection(shotDirection), speed(speed), collider(collider), tank(tank) {}
	};

	class BulletsManager {
	private:
		float time = 0.0f;
		std::vector<Bullet> bullets;
		static BulletsManager* instance;
		BulletsManager();
		Mesh* bulletMesh;

	public:
		BulletsManager(const BulletsManager& obj) = delete;
		static BulletsManager* getInstance();
		std::vector<Bullet> getBullets();
		void addBullet(Bullet bullet);
		void renderBullets(float deltaTime, Shader* shader, WorldOfTanks* ctx);
		bool checkBulletsCollisions();
		void removeBullet(uint32_t index);
	};
}