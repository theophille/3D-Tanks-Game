#include "terrain.h"
#include "boxCollider.h"
#include "sphereCollider.h"
#include "tema2.h"
#include <time.h>
#include <iostream>

using namespace game;

Terrain::Terrain(uint32_t numOfBuildings, uint32_t numOfEnemies, Tank* player) : player(player) {
	terrainMesh = Meshes::getInstance()->getTerrainMesh();
	cubeMesh = Meshes::getInstance()->getCubeMesh();
	generateBuildings(numOfBuildings);
	generateEnemies(numOfEnemies);
}

void Terrain::render(Shader* shader, WorldOfTanks* ctx, float deltaTime) {
	glm::mat4 modelMatrix = glm::mat4(1);
	glm::vec3 color = Colors::getInstance()->getColor("green");
	ctx->RenderMesh3D(terrainMesh, shader, modelMatrix, color);

	color = Colors::getInstance()->getColor("brightGray");

	for (int i = 0; i < buildings.size(); i++) {
		glm::vec3 bp = buildings[i].position;
		glm::vec3 bs = buildings[i].scale;

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, bp);
		modelMatrix = glm::scale(modelMatrix, bs);
		ctx->RenderMesh3D(cubeMesh, shader, modelMatrix, color);
	}


	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i] != player) {
			std::vector<std::vector<glm::vec3>> col = enemies[i]->checkForCollisions(
				getBuildings(),
				BulletsManager::getInstance()->getBullets(),
				getEnemies());

			for (int i = 0; i < col[0].size(); i++) {
				enemies[i]->updatePosition(enemies[i]->getPosition() + col[0].at(i));
			}

			for (int i = 0; i < col[1].size(); i++) {
				enemies[i]->updatePosition(enemies[i]->getPosition() + col[1].at(i));
			}

			enemies[i]->render(shader, ctx, deltaTime);
		}
	}

	std::vector<Bullet> bullets = BulletsManager::getInstance()->getBullets();

	for (int i = 0; i < buildings.size(); i++) {
		for (int j = 0; j < bullets.size(); j++) {
			if (tankBuildingOverlap(bullets[j].collider, buildings[i].collider)) {
				BulletsManager::getInstance()->removeBullet(j);
				bullets.erase(bullets.begin() + j);
				j--;
			}
		}
	}
}

void Terrain::generateBuildings(uint32_t numOfBuildings) {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	
	for (int i = 0; i < numOfBuildings; i++) {
		BoxCollider* collider;
		float posX, posZ;
		float scaleX, scaleY, scaleZ;

		do {
			float sign;
		
			sign = static_cast<float>(rand() % 2);
			posX = (sign == 0 ? -1 : 1) * randf(POS_MIN, POS_MAX);
			sign = static_cast<float>(rand() % 2);
			posZ = (sign == 0 ? -1 : 1) * randf(POS_MIN, POS_MAX);

			scaleX = randf(BD_SCALE_MIN, BD_SCALE_MAX);
			scaleY = randf(BD_SCALE_MIN, BD_SCALE_MAX);
			scaleZ = randf(BD_SCALE_MIN, BD_SCALE_MAX);

			collider = new BoxCollider(
				Range(posX - scaleX, posX + scaleX),
				Range(0, 2 * scaleY),
				Range(posZ - scaleZ, posZ + scaleZ));

		} while (buildingsGenerationOverlapping(collider));

		buildings.push_back(Building(
			glm::vec3(posX, 0, posZ),
			glm::vec3(scaleX, scaleY, scaleZ),
			collider
		));
	}
}

void Terrain::generateEnemies(uint32_t numOfEnemies) {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	for (int i = 0; i < numOfEnemies; i++) {
		Tank* enemy = new Tank(ENEMY);
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 rotation = glm::vec3(0, 0, 0);

		while(true) {
			float sign;

			sign = static_cast<float>(rand() % 2);
			position.x = (sign == 0 ? -1 : 1) * randf(POS_MIN, POS_MAX);
			sign = static_cast<float>(rand() % 2);
			position.z = (sign == 0 ? -1 : 1) * randf(POS_MIN, POS_MAX);
			sign = static_cast<float>(rand() % 2);
			rotation.y = (sign == 0 ? -1 : 1) * randf(0, 2.0f * M_PI);
			enemy->set(position, rotation);

			if (!enemiesGenerationOverlapping(enemy->getCollider())) {
				break;
			}
		}

		enemies.push_back(enemy);
	}

	enemies.push_back(player);
}

bool Terrain::buildingsGenerationOverlapping(BoxCollider* a) {
	
	for (int i = 0; i < buildings.size(); i++) {
		if (buildingsOverlap(a, buildings[i].collider)) {
			return true;
		}
	}

	return false;
}

bool Terrain::buildingsOverlap(BoxCollider* a, BoxCollider* b) {
	return (a->x.min <= b->x.max &&
			a->x.max >= b->x.min &&
			a->y.min <= b->y.max &&
			a->y.max >= b->y.min &&
			a->z.min <= b->z.max &&
			a->z.max >= b->z.min);
}

bool Terrain::enemiesGenerationOverlapping(SphereCollider* a) {
	
	for (int i = 0; i < buildings.size(); i++) {
		if (tankBuildingOverlap(a, buildings[i].collider)) {
			return true;
		}
	}

	for (int i = 0; i < enemies.size(); i++) {
		if (tanksOverlap(a, enemies[i]->getCollider())) {
			return true;
		}
	}

	return false;
}

bool Terrain::tanksOverlap(SphereCollider* a, SphereCollider* b) {
	return glm::distance(a->position, b->position) < (a->radius + b->radius);
}

bool Terrain::tankBuildingOverlap(SphereCollider* a, BoxCollider* b) {
	float x = std::max(b->x.min, std::min(a->position.x, b->x.max));
	float y = std::max(b->y.min, std::min(a->position.y, b->y.max));
	float z = std::max(b->z.min, std::min(a->position.z, b->z.max));

	float distance = glm::distance(glm::vec3(x, y, z), a->position);

	if (distance < a->radius) {
		return true;
	}

	return false;
}

std::vector<Building> Terrain::getBuildings() {
	return buildings;
}

std::vector<Tank*> Terrain::getEnemies() {
	return enemies;
}

Tank* Terrain::getPlayer() {
	return player;
}