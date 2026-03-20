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

Body::~Body() {
	if (mesh) delete mesh;
	if (texture) delete texture;
}

void Body::Update(float deltaTime) {
	/// From 1st semester physics class
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;
}
void Body::UpdatePos(float deltaTime) {
	// just update position based on velocity from the roll
	pos += vel * deltaTime;
}
void Body::UpdateVel(float deltaTime) {
	vel += accel * deltaTime;
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

void Body::SetMesh(const char* filename_) {
	mesh = new Mesh(filename_);
	mesh->OnCreate();
}
Mesh* Body::GetMesh() const {
	return mesh;
}

bool Body::OnCreate() {
	return true;
}

void Body::OnDestroy() {
}

void Body::Render() const {
}
Matrix4 Body::GetModelMatrix() const {
	Matrix4 T = MMath::translate(pos);
	Matrix4 S = MMath::scale(radius, radius, radius);
	Matrix4 R = MMath::toMatrix4(orientation);
	return T * R * S;
}

void Body::StraightLineConstraint(float slope, float yIntercept, float deltaTime) {
	float positionConstraint = pos.y - slope * pos.x - yIntercept;
	Vec3 JT = Vec3(-slope, 1, 0); // this is the jacobian transpose
	float baumgarteNumber = 0.1f;
	float bias = baumgarteNumber * positionConstraint / deltaTime; // might have messed uo sign on that
	float JJT = pow(slope, 2) + 1;
	float JV = vel.y - slope * vel.x;
	float lagrangian = (-JV - bias) * mass / JJT; // mass needed? maybe not
	vel += JT * lagrangian / mass;

}


