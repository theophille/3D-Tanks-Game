#pragma once

#include <string>

namespace game
{
	class Collidable {
	public:
		std::string wrapperType;
		Collidable(std::string wrapperType) : wrapperType(wrapperType) {}
		virtual void onCollisionEnter() = 0;
	};
}