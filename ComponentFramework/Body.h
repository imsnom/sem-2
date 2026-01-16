#ifndef BODY_H
#define BODY_H
#include <Vector.h> /// This is in GameDev
#include <QMath.h>
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
	Quaternion orientation; // this doesn't work ??

	Vec3 angularVel; // angle / time (rad/s) also points along axis of rotation

	
private: /// Graphics stuff 
	Mesh *mesh;
	Texture *texture;
	
public:
	bool OnCreate();
	void OnDestroy();
	void Update(float deltaTime);
	void UpdateOrientation(float deltaTime);
	void Render() const;
	void ApplyForce(Vec3 force);
	void setAccel(const Vec3 &accel_) { accel = accel_;}
};

#endif
