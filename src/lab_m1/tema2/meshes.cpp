#include "meshes.h"
#include "utils/text_utils.h"
#include "core/managers/resource_path.h"
#include "gameConstants.h"

using namespace game;

Meshes* Meshes::instance = NULL;

Meshes::Meshes() {}

Meshes* Meshes::getInstance() {
	if (instance == NULL) {
		instance = new Meshes();
		return instance;
	}
	else {
		return instance;
	}
}

Mesh* Meshes::getBulletMesh() {
	return bullet;
}

Mesh* Meshes::getTerrainMesh() {
	return terrain;
}

Mesh* Meshes::getCubeMesh() {
	return cube;
}

std::unordered_map<std::string, Mesh*> Meshes::getTankMeshes() {
	return tankMeshes;
}

std::unordered_map<std::string, glm::vec3> Meshes::getTankOffsets() {
	return tankOffsets;
}

void Meshes::InitBulletMesh(WindowObject* window) {
	bullet = new Mesh("bullet");
	bullet->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, WOT_PATH), "bullet.ply");
}

void Meshes::InitTerrainMeshes(WindowObject* window) {
	terrain = new Mesh("terrain");
	terrain->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, WOT_PATH), "terrain.ply");
	cube = new Mesh("cube");
	cube->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, WOT_PATH), "cube.ply");
}

void Meshes::InitTankMeshes(WindowObject* window) {
	std::string tankParts[] = { TANK_PARTS };

	for (int i = 0; i < TANK_PARTS_LEN; i++) {
		Mesh* mesh = new Mesh(tankParts[i]);
		std::string fileName = "tank_" + tankParts[i] + ".ply";
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, WOT_PATH), fileName);
		tankMeshes[mesh->GetMeshID()] = mesh;
	}
}

void Meshes::InitTankOffsets() {
	std::string tankParts[] = { TANK_PARTS };

	tankOffsets[tankParts[BODY]] = glm::vec3(0, 0.2f, 0);
	tankOffsets[tankParts[DECO]] = glm::vec3(-0.42f, 0.3f, 0);
	tankOffsets[tankParts[GUN]] = glm::vec3(-0.2f, 0.55f, 0);
	tankOffsets[tankParts[HAT]] = glm::vec3(0, 0.65f, 0);
	tankOffsets[tankParts[HEAD]] = glm::vec3(0, 0.55f, 0);
	tankOffsets[tankParts[TRACKS]] = glm::vec3(-0.04f, 0, 0);
	tankOffsets[tankParts[WHEELS]] = glm::vec3(0, 0, 0);
	tankOffsets[tankParts[WINGS]] = glm::vec3(-0.08f, 0.15f, 0);
}

void Meshes::InitMeshes(WindowObject* window) {
	InitBulletMesh(window);
	InitTankMeshes(window);
	InitTerrainMeshes(window);
	InitTankOffsets();
}
