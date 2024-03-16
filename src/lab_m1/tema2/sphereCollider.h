#pragma once

#include <string>

#include "utils/glm_utils.h"

namespace game
{
	class SphereCollider {
	public:
		float radius;
		glm::vec3 position;
		SphereCollider(float radius, glm::vec3 position);
		void onCollisionEnter();
	};
}