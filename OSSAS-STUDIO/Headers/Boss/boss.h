/******************************************************************************/
/*!
\file		boss.h
\author 	Nur Syazwan Bin Syamsuddin
\par    	nursyazwan.b@digipen.edu
			100% Code Contribution
\date   	20 Jun, 2020
\brief      This document contains the function headers
			for the boss minion.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef BOSS_H_
#define BOSS_H_

#include "AEEngine.h"
#include "Collision/Collision.h"
#include "GameState/GameObj/GameObj.h"

enum bossDirection
{
	Right,
	Left,
	Top,
	Down
};


enum bossAttack
{
	None = 0,
	Basic,
	Regen,
	Ultimate,
};

struct Boss : public GameObjInst
{
	bool			IsJumping;
	bool			IsMoving;
	f32				Direction;
	int				Alive;
	int				Lives;
	f32				Scale;
	float			Height;

	// basic Constructor
	Boss();
	// Destructor
	~Boss();
	AEVec2 getPos();

};

typedef struct attack
{
	union Damage
	{
		int Basic;
		int Heavy;
		int Ultimate;
	}Damage;

	int		Type;
	f32		Scale;
	f32		Direction;
	AEVec2	Range;
	AEVec2	PosCurr;
	AEVec2  VelCurr;
	AEMtx33 Transform;
	AABB	BoundingBox;

}attack;

//Bullet Obj

struct BulletObj
{
	unsigned int	 type;		// object type
	AEGfxVertexList* pMesh;		// pbject
	AEGfxTexture* pText;
};

struct BossBullet
{
	BulletObj* pObject;
	AEVec2 posCurr;
	AEVec2 velCurr;
	unsigned int flag;
	float scale;
	float dirCurr;
	AEMtx33 transform;
	AABB boundingBox;
	double lifetime;
};

void boss_init();
void boss_load ();
void draw_boss();
void free_boss();
void unload_boss();
void update_boss();

//Bullet Spawn
void LoadBullet();
BossBullet* CreateBullet(float scale, AEVec2* pPos, AEVec2* pVel, float dir);
void DestroyBossBullet(BossBullet* pInst);
void FreeBossBullet();
void UnloadBullet();
void UpdateBossBullet();
void DrawBossBullet();

#endif // PLAYER_H_