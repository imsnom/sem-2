#include "Collision.h"
Vec3 distance;

bool COLLISION::Detection(const Body& a, const Body& b)
{
	distance = Vec3((a.pos.x - b.pos.x), (a.pos.y - b.pos.y), (a.pos.z - b.pos.z));

	if (distance[0] > a.radius + b.radius || distance[1] > a.radius + b.radius || distance[2] > a.radius + b.radius) {
			//std::cout << distance[0] << ", " << distance[1] << ", " << distance[2] << std::endl;
			return false;
	}
	else if (distance[0] < -(a.radius + b.radius) || distance[1] < -(a.radius + b.radius) || distance[2] < -(a.radius + b.radius)) {
		return false;
	}
	
	return true;
	




}
