#include "Body.h"
Body::Body(): 
pos{}
, vel{}
, accel{}
, mass{ 1.0f }
, radius{ 1.0f }
, mesh{ nullptr }
, texture{ nullptr }
, rotationalInertia{ 
	Matrix3((2 / 5 * mass * pow(radius, 2) + mass * pow(radius, 2)), 0, 0,
			 0, (2 / 5 * mass * pow(radius, 2) + mass * pow(radius, 2)), 0,
			 0, 0, (2 / 5 * mass * pow(radius, 2) + mass * pow(radius, 2))) }
{
}

Body::~Body() {}

void Body::Update(float deltaTime) {
	/// From 1st semester physics class
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;
}
void Body::UpdatePos(float deltaTime) {
	// just update position based on velocity from the roll
	pos += vel * deltaTime;
}
void Body::ApplyForce(Vec3 force) {
	accel = force / mass;
}


void Body::UpdateOrientation(float deltaTime) {
	float angularVelMag = VMath::mag(angularVel);
	float angleRadians = angularVelMag * deltaTime;
	float angleDegrees = angleRadians * RADIANS_TO_DEGREES;

	if (angleDegrees < VERY_SMALL) return; // this happens if the angle is zero

	Vec3 axisOfRotation = VMath::normalize(angularVel);

	Quaternion rotation = QMath::angleAxisRotation(angleDegrees, axisOfRotation);

	orientation = rotation * orientation;
	
}
void Body::UpdateAngularVel(float deltaTime) {
	angularVel += angularAcc * deltaTime;

}
void Body::ApplyTorque(Vec3 netTorque) {
	angularAcc = MMath::inverse(rotationalInertia) * netTorque;

}

bool Body::OnCreate() {
	return true;
}

void Body::OnDestroy() {
}

void Body::Render() const {
}

