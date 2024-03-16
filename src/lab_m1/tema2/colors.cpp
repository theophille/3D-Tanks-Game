#include "colors.h"

using namespace game;

Colors* Colors::instance = NULL;

Colors::Colors() {
	InitColors();
	InitPlayerTankColors();
	InitEnemyTankColors();
}

void Colors::InitColors() {
	colors["beige"] = glm::vec3(
		CHANNEL_PERCENTAGE(0xF2),
		CHANNEL_PERCENTAGE(0xD2),
		CHANNEL_PERCENTAGE(0xBF)
	);

	colors["darkBrown"] = glm::vec3(
		CHANNEL_PERCENTAGE(0x1E),
		CHANNEL_PERCENTAGE(0x1E),
		CHANNEL_PERCENTAGE(0x1E)
	);

	colors["orange"] = glm::vec3(
		CHANNEL_PERCENTAGE(0xE7),
		CHANNEL_PERCENTAGE(0x65),
		CHANNEL_PERCENTAGE(0x3E)
	);

	colors["brightBrown"] = glm::vec3(
		CHANNEL_PERCENTAGE(0x86),
		CHANNEL_PERCENTAGE(0x76),
		CHANNEL_PERCENTAGE(0x67)
	);

	colors["blue"] = glm::vec3(
		CHANNEL_PERCENTAGE(0x91),
		CHANNEL_PERCENTAGE(0xD3),
		CHANNEL_PERCENTAGE(0xFF)
	);

	colors["gray"] = glm::vec3(
		CHANNEL_PERCENTAGE(0xC4),
		CHANNEL_PERCENTAGE(0xC4),
		CHANNEL_PERCENTAGE(0xC4)
	);

	colors["green"] = glm::vec3(
		CHANNEL_PERCENTAGE(0x32),
		CHANNEL_PERCENTAGE(0xA8),
		CHANNEL_PERCENTAGE(0x52)
	);

	colors["brightGray"] = glm::vec3(
		CHANNEL_PERCENTAGE(0x53),
		CHANNEL_PERCENTAGE(0x80),
		CHANNEL_PERCENTAGE(0x74)
	);

	colors["red"] = glm::vec3(
		CHANNEL_PERCENTAGE(0xB5),
		CHANNEL_PERCENTAGE(0x36),
		CHANNEL_PERCENTAGE(0x38)
	);
}

void Colors::InitPlayerTankColors() {
	std::string tankParts[] = {TANK_PARTS};

	playerTankColors[tankParts[BODY]] = getColor("darkBrown");
	playerTankColors[tankParts[DECO]] = getColor("orange");
	playerTankColors[tankParts[GUN]] = getColor("darkBrown");
	playerTankColors[tankParts[HAT]] = getColor("darkBrown");
	playerTankColors[tankParts[HEAD]] = getColor("beige");
	playerTankColors[tankParts[TRACKS]] = getColor("darkBrown");
	playerTankColors[tankParts[WHEELS]] = getColor("orange");
	playerTankColors[tankParts[WINGS]] = getColor("beige");
}

void Colors::InitEnemyTankColors() {
	std::string tankParts[] = {TANK_PARTS};

	enemyTankColors[tankParts[BODY]] = getColor("red");
	enemyTankColors[tankParts[DECO]] = getColor("orange");
	enemyTankColors[tankParts[GUN]] = getColor("red");
	enemyTankColors[tankParts[HAT]] = getColor("red");
	enemyTankColors[tankParts[HEAD]] = getColor("beige");
	enemyTankColors[tankParts[TRACKS]] = getColor("red");
	enemyTankColors[tankParts[WHEELS]] = getColor("orange");
	enemyTankColors[tankParts[WINGS]] = getColor("beige");
}

Colors* Colors::getInstance() {
	if (instance == NULL) {
		instance = new Colors();
		return instance;
	}
	else {
		return instance;
	}
}

glm::vec3 Colors::getColor(std::string color) {
	return colors.at(color);
}

std::unordered_map<std::string, glm::vec3> Colors::getPlayerTankColors() {
	return playerTankColors;
}

std::unordered_map<std::string, glm::vec3> Colors::getEnemyTankColors() {
	return enemyTankColors;
}