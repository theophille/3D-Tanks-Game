#pragma once

#include <string>

#include "utils/glm_utils.h"
#include "sphereCollider.h"

namespace game
{
	struct Range {
		float min;
		float max;
		Range() : min(0), max(0) {}
		Range(float min, float max) : min(min), max(max) {}
	};

	class BoxCollider {
	public:
		Range x;
		Range y;
		Range z;
		BoxCollider(Range x, Range y, Range z);
		bool collisionCheck(SphereCollider* sc);
	};
}