#pragma once

#include <string>
#include <unordered_map>

#include "utils/glm_utils.h"
#include "gameConstants.h"

#define CHANNEL_PERCENTAGE(value) ((value) / 255.0f)

namespace game
{
	class Colors {
	private:
		std::unordered_map<std::string, glm::vec3> colors;
		std::unordered_map<std::string, glm::vec3> playerTankColors;
		std::unordered_map<std::string, glm::vec3> enemyTankColors;
		std::vector<glm::vec3> entitiesColors;
		static Colors* instance;
		void InitColors();
		void InitPlayerTankColors();
		void InitEnemyTankColors();
		Colors();

	public:
		Colors(const Colors& obj) = delete;
		static Colors* getInstance();
		glm::vec3 getColor(std::string color);
		std::unordered_map<std::string, glm::vec3> getPlayerTankColors();
		std::unordered_map<std::string, glm::vec3> getEnemyTankColors();
	};
}