#include "bullets.h"
#include "tema2.h"
#include "terrain.h"

#include <iostream>

using namespace game;

BulletsManager::BulletsManager() {
	bulletMesh = Meshes::getInstance()->getBulletMesh();
}

BulletsManager* BulletsManager::instance = NULL;

BulletsManager* BulletsManager::getInstance() {
	if (instance == NULL) {
		instance = new BulletsManager();
		return instance;
	}
	else {
		return instance;
	}
}

void BulletsManager::addBullet(Bullet bullet) {
	bullets.push_back(bullet);
}

std::vector<Bullet> BulletsManager::getBullets() {
	return bullets;
}

void BulletsManager::renderBullets(float deltaTime, Shader* shader, WorldOfTanks* ctx) {
	for (int i = 0; i < bullets.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		bullets[i].speed -= glm::vec3(0, GACC * deltaTime, 0);
		bullets[i].position += bullets[i].speed * deltaTime;
		bullets[i].collider->position = bullets[i].position;
		modelMatrix = glm::translate(modelMatrix, bullets[i].position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
		ctx->RenderMesh3D(bulletMesh, shader, modelMatrix, Colors::getInstance()->getColor("gray"));

		if (bullets[i].position.y < 0) {
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
}

void BulletsManager::removeBullet(uint32_t index) {
	bullets.erase(bullets.begin() + index);
}