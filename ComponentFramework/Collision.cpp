#include "Collision.h"
Vec3 distance;

bool COLLISION::Detection(const Body& a, const Body& b)
{
	distance = Vec3((a.pos.x - b.pos.x), (a.pos.y - b.pos.y), (a.pos.z - b.pos.z));

	if (distance[0] > a.radius + b.radius || distance[1] > a.radius + b.radius || distance[2] > a.radius + b.radius) {
		return false;
	}
	else if (distance[0] < -(a.radius + b.radius) || distance[1] < -(a.radius + b.radius) || distance[2] < -(a.radius + b.radius)) {
		return false;
	}

	return true;


}

void COLLISION::Response(Body& a, Body& b) {
	Vec3 collisionNormal = a.pos - b.pos;
	collisionNormal = VMath::normalize(collisionNormal);
	float relativeVelAlongNormal = VMath::dot((a.vel - b.vel), collisionNormal); // a.vel - b.vel dot collisionNormal
	if (relativeVelAlongNormal > 0) {
		return;
	}
	float epsilon = 0.9f; // bounciness
	float impulse = (-(1 + epsilon) * relativeVelAlongNormal) / (1 / a.mass + 1 / b.mass); // 

	// update the velocities
	a.vel += (impulse * collisionNormal / a.mass);
	b.vel -= (impulse * collisionNormal / b.mass);


}
