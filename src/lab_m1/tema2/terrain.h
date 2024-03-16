#pragma once

#include <string>
#include <vector>

#include "utils/glm_utils.h"
#include "gameConstants.h"
#include "core/gpu/shader.h"
#include "core/gpu/mesh.h"
#include "meshes.h"

namespace game
{
	class BoxCollider;
	class SphereCollider;
	class Tank;
	struct Building {
		glm::vec3 position;
		glm::vec3 scale;
		BoxCollider* collider;

		Building() : position(glm::vec3(0)), scale(glm::vec3(1)), collider(NULL) {}
		Building(glm::vec3 position, glm::vec3 scale, BoxCollider* collider)
			: position(position), scale(scale), collider(collider) {}
	};

	class WorldOfTanks;
	class Terrain {
	private:
		Mesh* terrainMesh;
		Mesh* cubeMesh;
		std::vector<Building> buildings;
		std::vector<Tank*> enemies;
		Tank* player;

	public:
		Terrain(uint32_t numOfBuildings, uint32_t numOfEnemies, Tank* player);
		void render(Shader* shader, WorldOfTanks* ctx, float deltaTime);
		void generateBuildings(uint32_t numOfBuildings);
		void generateEnemies(uint32_t numOfEnemies);
		bool buildingsGenerationOverlapping(BoxCollider* a);
		bool buildingsOverlap(BoxCollider* a, BoxCollider* b);
		bool enemiesGenerationOverlapping(SphereCollider* a);
		bool tanksOverlap(SphereCollider* a, SphereCollider* b);
		bool tankBuildingOverlap(SphereCollider* a, BoxCollider* b);
		std::vector<Building> getBuildings();
		std::vector<Tank*> getEnemies();
		Tank* getPlayer();
	};
}