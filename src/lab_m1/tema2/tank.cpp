#include "tank.h"
#include "utils/text_utils.h"
#include "tema2.h"
#include "bullets.h"

#include <iostream>
#include <algorithm>

using namespace game;

Tank::Tank(unsigned int type) : type(type) {
	meshes = Meshes::getInstance()->getTankMeshes();
	bullet = Meshes::getInstance()->getBulletMesh();

	if (type == PLAYER) {
		colors = Colors::getInstance()->getPlayerTankColors();
	}
	else {
		colors = Colors::getInstance()->getEnemyTankColors();
	}
	
	offsets = Meshes::getInstance()->getTankOffsets();
}

Tank::Tank(unsigned int type, glm::vec3 position) : type(type), position(position) {
	meshes = Meshes::getInstance()->getTankMeshes();
	bullet = Meshes::getInstance()->getBulletMesh();

	if (type == PLAYER) {
		colors = Colors::getInstance()->getPlayerTankColors();
	}
	else {
		colors = Colors::getInstance()->getEnemyTankColors();
	}

	offsets = Meshes::getInstance()->getTankOffsets();
}

void Tank::render(Shader* shader, WorldOfTanks* ctx, float deltaTime) {
	for (const auto& pair : meshes) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::translate(modelMatrix, TERRAIN_OFFSET);
		modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
		
		if (type == PLAYER && (pair.first == "gun" || pair.first == "hat")) {
			modelMatrix = glm::rotate(modelMatrix, headRot, glm::vec3(0, 1, 0));
		}

		modelMatrix = glm::translate(modelMatrix, offsets[pair.first]);

		if (type == PLAYER && pair.first == "gun") {
			float angle = gunRot + deltaY * 0.003f;

			if (angle >= RADIANS(-45) && angle <= RADIANS(15)) {
				gunRot = angle;
			}

			modelMatrix = glm::rotate(modelMatrix, gunRot, glm::vec3(0, 0, 1));
			deltaY = 0.0f;
		}

		if (type == PLAYER && pair.first == "head") {
			headRot += -deltaX * 0.005f;
			modelMatrix = glm::rotate(modelMatrix, headRot, glm::vec3(0, 1, 0));
			deltaX = 0.0f;
		}

		ctx->RenderMesh3D(pair.second, shader, modelMatrix, colors[pair.first], this);
	}

	if (type == ENEMY && hp > 0) {
		Tank* player = ctx->getTerrain()->getPlayer();
		float enemyPlayerDistance = glm::distance(position, player->getPosition());

		if (enemyPlayerDistance < collider->radius + playerActiveEnemiesRadius) {
			glm::vec3 playerDirection = glm::normalize(player->getPosition() - position);
			float defRot = glm::acos(glm::dot(playerDirection, glm::vec3(-1, 0, 0)));
			
			if (playerDirection.z < 0) {
				defRot = glm::pi<float>() * 2 - defRot;
			}
			
			if (rotation.y < defRot && abs(rotation.y - defRot) > 0.1f) {
				rotateRight(3.0f * deltaTime);
			}
			else if (rotation.y > defRot && abs(rotation.y - defRot) > 0.1f) {
				rotateRight(- 3.0f * deltaTime);
			}
			else {
				rotation.y = defRot;
				forward = playerDirection;
				
				if (shotTime < 0.0f) {
					fire();
					shotTime = 1.0f;
				}

				shotTime -= deltaTime;

				if (enemyPlayerDistance > 4.0f) {
					moveForward(deltaTime);
				}
			}

		}
		else {
			std::srand(static_cast<unsigned int>(std::time(nullptr) + (float)rand()));

			if (state == 0) {
				if (!turnAround) {
					moveForward(deltaTime);
					time -= deltaTime;

					if (position.x > 50.0f ||
						position.x < -50.0f ||
						position.z > 50.0f ||
						position.z < -50.0f) {
						turnAround = true;

						marginCollisionAngle = rotation.y + M_PI;
						marginCollisionAngle = fmod(marginCollisionAngle, 2 * glm::pi<float>());

						if (marginCollisionAngle < 0.0f) {
							marginCollisionAngle += 2 * glm::pi<float>();
						}
					}
				}
				else {
					rotateRight(deltaTime);

					if (abs(rotation.y - marginCollisionAngle) < 0.1f) {
						turnAround = false;
						moveForward(1.0f);
					}
				}


				if (time < 0) {
					time = randf(2.0f, 4.0f);
					state = 1;
				}
			}
			else {
				if (abs(desiredAngle - rotation.y) > abs(2.0f * M_PI - desiredAngle + rotation.y)) {
					rotateRight(deltaTime);

					if (rotation.y > desiredAngle) {
						desiredAngle = rotation.y + randf(-2.0 * M_PI, 2.0 * M_PI);
						desiredAngle = fmod(desiredAngle, 2 * glm::pi<float>());

						if (desiredAngle < 0.0f) {
							desiredAngle += 2 * glm::pi<float>();
						}

						state = 0;
					}
				}
				else {
					rotateRight(-deltaTime);

					if (rotation.y < desiredAngle) {
						desiredAngle = rotation.y + randf(-2.0 * M_PI, 2.0 * M_PI);
						desiredAngle = fmod(desiredAngle, 2 * glm::pi<float>());

						if (desiredAngle < 0.0f) {
							desiredAngle += 2 * glm::pi<float>();
						}

						state = 0;
					}
				}
			}
		}
	}

	checkForBulletCollisions();
}

void Tank::checkForBulletCollisions() {
	std::vector<Bullet> bullets = BulletsManager::getInstance()->getBullets();

	for (int i = 0; i < bullets.size(); i++) {
		if (bulletTankCollision(bullets[i], this) && bullets[i].tank != this) {
			BulletsManager::getInstance()->removeBullet(i);
			bullets.erase(bullets.begin() + i);
			i--;

			if (hp > 0) {
				hp--;
			}
		}
	}
}

void Tank::fire() {
	glm::vec3 shotDirection = glm::vec3(-1, 0, 0);

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, headRot, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, gunRot, glm::vec3(0, 0, 1));
	shotDirection = glm::vec3(glm::normalize(modelMatrix * glm::vec4(shotDirection, 1.0f)));

	glm::vec3 bulletPos = offsets["gun"];

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::translate(modelMatrix, TERRAIN_OFFSET);
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, headRot, glm::vec3(0, 1, 0));
	bulletPos = glm::vec3(modelMatrix * glm::vec4(bulletPos, 1.0f));

	BulletsManager::getInstance()->addBullet(Bullet(
		bulletPos,
		shotDirection,
		shotDirection * 15.0f,
		new SphereCollider(0.05f, bulletPos),
		this
	));
}

void Tank::moveForward(float deltaTime) {
	if (type == PLAYER) {
		updatePosition(position + PLAYER_SPEED * forward * deltaTime);
	}
	else {
		updatePosition(position + TANK_SPEED * forward * deltaTime);
	}
	collider->position = position;
}

void Tank::rotateRight(float deltaTime) {
	float angle = rotation.y + deltaTime;

	angle = fmod(angle, 2 * glm::pi<float>());

	if (angle < 0.0f) {
		angle += 2 * glm::pi<float>();
	}

	updateRotation(glm::vec3(
		rotation.x,
		angle,
		rotation.z
	));

	glm::mat4 model(1.0f);
	glm::mat4 rotMat = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
	glm::vec4 rotVec = glm::normalize(rotMat * glm::normalize(glm::vec4(-1.0f, 0, 0, 1.0f)));

	updateForward(glm::normalize(glm::vec3(rotVec)));
}

void Tank::set(glm::vec3 position, glm::vec3 rotation) {
	updatePosition(position);
	this->rotation = rotation;

	glm::mat4 rotMat = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
	glm::vec4 rotVec = glm::normalize(rotMat * glm::normalize(glm::vec4(-1.0f, 0, 0, 1.0f)));
	updateForward(glm::normalize(glm::vec3(rotVec)));
}

std::vector<std::vector<glm::vec3>> Tank::checkForCollisions(std::vector<Building> buildings, std::vector<Bullet> bullets, std::vector<Tank*> tanks) {
	std::vector<glm::vec3> buildingCollisions;
	std::vector<glm::vec3> tankCollisions;
	std::vector<glm::vec3> terrainBorder;
	
	for (int i = 0; i < buildings.size(); i++) {
		glm::vec3 position = buildingTankCollision(buildings[i].collider, this->collider);

		if (position != glm::vec3(0)) {
			buildingCollisions.push_back(position);
		}
	}

	for (int i = 0; i < tanks.size(); i++) {
		if (tanks[i] != this) {
			glm::vec3 position = tankTankCollision(this, tanks[i]);

			if (position != glm::vec3(0)) {
				tankCollisions.push_back(position);
			}
		}
	}

	return { buildingCollisions, tankCollisions };
}

glm::vec3 Tank::buildingTankCollision(BoxCollider* boxCollider, SphereCollider* sphereCollider) {
	float x = std::max(boxCollider->x.min, std::min(sphereCollider->position.x, boxCollider->x.max));
	float y = std::max(boxCollider->y.min, std::min(sphereCollider->position.y, boxCollider->y.max));
	float z = std::max(boxCollider->z.min, std::min(sphereCollider->position.z, boxCollider->z.max));

	float distance = glm::distance(glm::vec3(x, y, z), sphereCollider->position);

	if (distance < sphereCollider->radius) {
		float displacement = sphereCollider->radius - distance;
		glm::vec3 diff = glm::vec3(x, y, z) - position;
		glm::vec3 normal = abs(displacement) * glm::normalize(glm::vec3(diff));

		position += normal * -0.5f;
		return normal * -0.5f;
	}

	return glm::vec3(0);
}

glm::vec3 Tank::tankTankCollision(Tank* a, Tank* b) {
	float distance = glm::distance(a->position, b->position);

	if (distance < (a->collider->radius + b->collider->radius)) {
		float absP = a->collider->radius + b->collider->radius - distance;
		glm::vec3 diff = b->collider->position - a->collider->position;
		glm::vec3 P = abs(absP) * glm::normalize(diff);
		updatePosition(position + P * -0.5f);
		return P * -0.5f;
	}
	return glm::vec3(0);
}

bool Tank::bulletTankCollision(Bullet a, Tank* b) {
	float distance = glm::distance(a.position, b->position);

	if (distance < (a.collider->radius + b->collider->radius)) {
		return true;
	}

	return false;
}

glm::vec3 Tank::getPosition() {
	return position;
}

void Tank::updatePosition(glm::vec3 position) {
	collider->position = position;
	this->position = position;
}

glm::vec3 Tank::getRotation() {
	return rotation;
}

void Tank::updateRotation(glm::vec3 rotation) {
	this->rotation = rotation;
}

glm::vec3 Tank::getForward() {
	return forward;
}

void Tank::updateForward(glm::vec3 forward) {
	this->forward = forward;
}

glm::vec3 Tank::getRight() {
	return right;
}

void Tank::updateRight(glm::vec3 right) {
	this->right = right;
}

glm::vec3 Tank::getUp() {
	return up;
}

void Tank::updateUp(glm::vec3 up) {
	this->up = up;
}

SphereCollider* Tank::getCollider() {
	return collider;
}