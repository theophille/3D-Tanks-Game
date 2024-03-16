#include "boxCollider.h"

using namespace game;

BoxCollider::BoxCollider(Range x, Range y, Range z) : x(x), y(y), z(z) {}

bool BoxCollider::collisionCheck(SphereCollider* sc) {
	return false;
}