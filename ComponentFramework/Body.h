#ifndef BODY_H
#define BODY_H
#include <Vector.h> /// This is in GameDev
#include <QMath.h>
#include <Quaternion.h>
#include <MMath.h>
#include "Mesh.h"
using namespace MATH; 

/// Just forward declair these classes so I can define a pointer to them
/// Used later in the course
class Mesh;
class Texture;

class Body {
public:
    Body();
    ~Body();
public: /// Physics stuff (was originally private)
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float mass;
	// rotation stuff 
	Quaternion orientation; // this doesn't work ?? works now i think

	Vec3 angularVel; // angle / time (rad/s) also points along axis of rotation
	// angular acceleration is change in angularVel with time
	Vec3 angularAcc;
	float radius; // pretend all body objects are spheres
	Matrix3 rotationalInertia; // using solid sphere inertia
	
private: /// Graphics stuff 
	Mesh *mesh;
	Texture *texture;
	
public:
	bool OnCreate();
	void OnDestroy();
	void Update(float deltaTime);
	void UpdatePos(float deltaTime);
	void UpdateVel(float deltaTime);
	void StraightLineConstraint(float slope, float yIntercept, float deltaTime);
	void Render() const;
	void ApplyForce(Vec3 force);
	void setAccel(const Vec3 &accel_) { accel = accel_;}
	// rotation stuff
	void ApplyTorque(Vec3 netTorque);
	void UpdateOrientation(float deltaTime);
	void UpdateAngularVel(float deltaTime);
	Matrix4 GetModelMatrix() const;
	void SetMesh(const char* filename_);
	Mesh* GetMesh() const;
};

#endif
