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
	if (deltaTime < VERY_SMALL) {
		return;
	}
	float positionConstraint = pos.y - slope * pos.x - yIntercept;
	Vec3 JT = Vec3(-slope, 1, 0); // this is the jacobian transpose
	float baumgarteNumber = 0.1f;
	// watch out for divide by zero
	
	float bias = baumgarteNumber * positionConstraint / deltaTime; // might have messed up sign on that
	float JJT = pow(slope, 2) + 1;
	float JV = vel.y - slope * vel.x;
	float lagrangian = (-JV - bias) * mass / JJT; // mass needed? maybe not
	vel += JT * lagrangian / mass;

}
void Body::RodConstraint(float deltaTime, Vec3 anchorPoint, float rodLength) {
	if (deltaTime < VERY_SMALL) {
		return;
	}
	Vec3 rodVector = pos - anchorPoint;
	float positionConstraint = VMath::mag(rodVector) - rodLength;
	float JV = VMath::dot(rodVector, vel) / VMath::mag(rodVector);
	const float baumgarteNumber = 0.1f;
	float b = -(baumgarteNumber / deltaTime) * positionConstraint;
	float lambda = -mass * (JV + b);
	Vec3 JTransposed = rodVector / VMath::mag(rodVector);
	Vec3 deltaVel = JTransposed * lambda / mass;
	vel += deltaVel;
}
void Body::CircleConstraint(float circleRadius, Vec3 circleCenter, float deltaTime) {
	if (deltaTime < VERY_SMALL) {
		return;
	}
	float positionConstraint = pow((pos.x - circleCenter.x), 2) + pow((pos.y - circleCenter.y), 2) - pow(circleRadius, 2);

	Vec3 JT = Vec3(2 * pos.x - 2 * circleCenter.x, 2 * pos.y - 2 * circleCenter.y, 0);
	float baumgarteNumber = 0.1f;
	// watch out for divide by zero

	float bias = baumgarteNumber * positionConstraint / deltaTime; // might have messed up sign on that
	float JJT = pow(JT.x, 2) + pow(JT.y, 2); // JT dot JT also works for this
	float JV = (2 * pos.x * vel.x - 2 * vel.x * circleCenter.x) + (2 * pos.y * vel.y - 2 * vel.y * circleCenter.y);
	float lagrangian = (-JV - bias) * mass / JJT; // mass needed? maybe not
	vel += JT * lagrangian / mass;

}

void Body::SphereConstraint(float sphereRadius, Vec3 sphereCenter, float deltaTime) {
	if (deltaTime < VERY_SMALL) {
		return;
	}
	float positionConstraint = pow((pos.x - sphereCenter.x), 2) + pow((pos.y - sphereCenter.y), pow(pos.z - sphereCenter.z, 2)) - pow(sphereRadius, 2);

	Vec3 JT = Vec3(2 * pos.x - 2 * sphereCenter.x, 2 * pos.y - 2 * sphereCenter.y, 2 * pos.z - 2 * sphereCenter.z);
	float baumgarteNumber = 0.1f;
	// watch out for divide by zero

	float bias = baumgarteNumber * positionConstraint / deltaTime; // might have messed up sign on that
	float JJT = pow(JT.x, 2) + pow(JT.y, 2); // JT dot JT also works for this
	float JV = (2 * pos.x * vel.x - 2 * vel.x * sphereCenter.x) + (2 * pos.y * vel.y - 2 * vel.y * sphereCenter.y) + (2 * pos.z * vel.z - 2 * vel.z * sphereCenter.z);
	float lagrangian = (-JV - bias) * mass / JJT; // mass needed? maybe not
	vel += JT * lagrangian / mass;

}

