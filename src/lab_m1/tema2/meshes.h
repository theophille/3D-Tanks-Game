#pragma once

#include <string>
#include <unordered_map>

#include "utils/glm_utils.h"
#include "core/gpu/mesh.h"
#include "core/window/window_object.h"

namespace game
{
	class Meshes {
	private:
		std::unordered_map<std::string, Mesh*> tankMeshes;
		std::unordered_map<std::string, glm::vec3> tankOffsets;
		Mesh* bullet;
		Mesh* terrain;
		Mesh* cube;
		static Meshes* instance;
		Meshes();
		void InitBulletMesh(WindowObject* window);
		void InitTankMeshes(WindowObject* window);
		void InitTerrainMeshes(WindowObject* window);
		void InitTankOffsets();

	public:
		Meshes(const Meshes& obj) = delete;
		static Meshes* getInstance();
		Mesh* getBulletMesh();
		Mesh* getTerrainMesh();
		Mesh* getCubeMesh();
		std::unordered_map<std::string, Mesh*> getTankMeshes();
		std::unordered_map<std::string, glm::vec3> getTankOffsets();
		void InitMeshes(WindowObject* window);
	};
}