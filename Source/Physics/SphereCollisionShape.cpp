// @file SphereCollisionShape.cpp
// Copyright 2016-2018 John Jackson. All Rights Reserved.

#include "Physics/SphereCollisionShape.h"
#include "Physics/PhysicsUtils.h"

namespace Enjon
{
	//==============================================================

	SphereCollisionShape::SphereCollisionShape( )
	{
		// Construct box collision shape
		mShape = new BulletSphereShape( mRadius );

		// Set up shape type
		mShapeType = CollisionShapeType::Sphere;
	}

	//==============================================================

	SphereCollisionShape::~SphereCollisionShape( )
	{
		// Release memory for shape
		delete mShape;
		mShape = nullptr;
	}

	//==============================================================

	void SphereCollisionShape::Base( )
	{
		// Does nothing...
	}

	//==============================================================
}
