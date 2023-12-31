/******************************************************************************
* \file			Collision.cpp
* \brief		Contain all the collision related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include "Collision/Collision.h"

/**************************************************************************/
/*!
	Collision
	*/
	/**************************************************************************/
bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2)
{
	// step 1
	if (aabb1.max.x > aabb2.min.x&& aabb1.min.x < aabb2.max.x &&
		aabb1.max.y > aabb2.min.y&& aabb1.min.y < aabb2.max.y)
		return true;


	// step 2
	// finding the velocity
	float DiffVelocityX = vel2.x - vel1.x;
	float DiffVelocityY = vel2.y - vel1.y;

	if (DiffVelocityX == 0 && DiffVelocityY == 0)
		return false;
	//caluculating the time for a frame
	float tFirst = 0.0f;
	float tLast = (float)AEFrameRateControllerGetFrameTime();

	// step 3
	// x direction
	if (DiffVelocityX < 0)
	{
		// case 1
		if (aabb1.min.x > aabb2.max.x)
			return false;
		// case 4
		if (aabb1.max.x < aabb2.min.x)
		{
			if (tFirst < (aabb1.max.x - aabb2.min.x) / DiffVelocityX)
				tFirst = (aabb1.max.x - aabb2.min.x) / DiffVelocityX;

		}
		if (aabb1.min.x < aabb2.max.x)
		{
			if (tLast > (aabb1.min.x - aabb2.max.x) / DiffVelocityX)
				tLast = (aabb1.min.x - aabb2.max.x) / DiffVelocityX;
		}
	}
	if (DiffVelocityX > 0)
	{
		// case 3
		if (aabb1.max.x < aabb2.min.x)
			return false;
		// case 2
		if (aabb1.min.x > aabb2.max.x)
		{
			if (tFirst < (aabb1.min.x - aabb2.max.x) / DiffVelocityX)
				tFirst = (aabb1.min.x - aabb2.max.x) / DiffVelocityX;
		}
		if (aabb1.max.x > aabb2.min.x)
		{
			if (tLast > (aabb1.max.x - aabb2.min.x) / DiffVelocityX)
				tLast = (aabb1.max.x - aabb2.min.x) / DiffVelocityX;
		}

	}
	else if (!(aabb1.max.x > aabb2.min.x&& aabb1.min.x < aabb2.max.x))
	{
		return false;
	}

	// step 4
	// y direction
	if (DiffVelocityY < 0)
	{
		// case 1
		if (aabb1.min.y > aabb2.max.y)
			return false;
		// case 4
		if (aabb1.max.y < aabb2.max.y)
		{
			if (tFirst < (aabb1.min.y - aabb2.max.y) / DiffVelocityY)
				tFirst = (aabb1.min.y - aabb2.max.y) / DiffVelocityY;

		}
		if (aabb1.min.y < aabb2.max.y)
		{
			if (tLast > (aabb1.min.y - aabb2.max.y) / DiffVelocityY)
				tLast = (aabb1.min.y - aabb2.max.y) / DiffVelocityY;
		}
	}
	if (DiffVelocityY > 0)
	{
		// case 3
		if (aabb1.max.y < aabb2.min.y)
			return false;
		// case 2
		if (aabb1.min.y > aabb2.max.y)
		{
			if (tFirst < (aabb1.min.y - aabb2.max.y) / DiffVelocityY)
				tFirst = (aabb1.min.y - aabb2.max.y) / DiffVelocityY;
		}
		if (aabb1.max.y > aabb2.min.y)
		{
			if (tLast > (aabb1.max.y - aabb2.min.y) / DiffVelocityY)
				tLast = (aabb1.max.y - aabb2.min.y) / DiffVelocityY;
		}


	}

	else if (!(aabb1.max.y > aabb2.min.y&& aabb1.min.y < aabb2.max.y))
	{
		return false;
	}
	// case 5
	if (tFirst > tLast)
		return false;

	// step 5
	return true;
}