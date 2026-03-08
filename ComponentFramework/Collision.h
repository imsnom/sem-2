#pragma once
#include "Body.h"
#include "VMath.h"
// Instead of a class, let’s put all the collision equations in a namespace.  
// I got this idea from the book "Beautiful C++: 30 Core Guidelines for Writing 
// Clean, Safe, and Fast Code" 


namespace COLLISION {

	//Vec3 distance;

	// True or false, did the spheres intersect
	// Better to pass by const reference as the Body class is getting quite big
	bool Detection(const Body& a, const Body& b);

	void Response(Body& a, Body& b);

}