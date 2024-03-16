#pragma once

#include <string>
#include <time.h>

#define GACC 9.82f
#define BULLET_SPEED 15.0f
#define ZNEAR 0.01f
#define ZFAR  200.0f
#define WOT_PATH "wot"
#define TANK_PARTS_LEN 8
#define TANK_PARTS "body", "deco", "gun", "hat", "head", "tracks", "wheels", "wings"
#define TERRAIN_OFFSET glm::vec3(0, 0.2f, 0)
#define TANK_SPEED 3.0f
#define PLAYER_SPEED 5.0f
#define TANK_COL_RAD 0.8f
#define BD_SCALE_MIN 2.0f
#define BD_SCALE_MAX 3.0f
#define POS_MIN 4.0f
#define POS_MAX 45.0f
#define DURATION 120.f

#define RAD_TO_DEG(rad) ((rad) * 180.0f / M_PI)

enum TankParts {
	BODY,
	DECO,
	GUN,
	HAT,
	HEAD,
	TRACKS,
	WHEELS,
	WINGS
};

enum Entities {
	PLAYER,
	ENEMY,
	BUILDING,
	BULLET
};

namespace game {
	static float randf(int a, int b) {
		float random = a + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (b - a)));
		return random;
	}
}