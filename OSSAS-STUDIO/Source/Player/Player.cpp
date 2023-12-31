/******************************************************************************/
/*!
\file		Player.cpp
\author 	Shaikh Faid Bin Omar, shaikhfaid.binomar, 390003019
			100% Code Contribution
\par    	email: shaikhfaid.binomar@digipen.edu
\date   	May 04, 2020
\brief		Function definitions for all Player game object attributes and
			camera system functions, render functions and particle system

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "AEEngine.h"
#include <time.h>
#include "Player/Player.h"
#include "GameState/Level1/Level1.h"
#include "GameState/GameStateMgr.h"
#include "GameState/Global.h"
#include "Audio/AudioManager.h"
#include "UI/tutorial.h"

// sprite position for jump physics
struct Position
{
	float oldPos_y = 0, oldPos_x = 0, newPos_y = 0, newPos_x = 0;
	float velocity = 120.0f, gravity = -250.0f;
	float floor_y = 0.0f, floor_x = 0.0;
}sprite;

// get the camera object to be operated on
Camera& camera = Global::GetInstance()->camera;
f32 levelWidth = 1500.0f, levelHeight = 100.0f, viewX = 290.0f, viewY = 600.0f;

/******************PLAYER VARIABLES******************/
AEGfxVertexList* playerHealth = nullptr;
AEGfxVertexList* playerHealth_white = nullptr;
AEGfxVertexList* towerHealth = nullptr;
AEGfxVertexList* towerHealth_white = nullptr;
AEGfxVertexList* manaBar = nullptr;
AEGfxVertexList* seedBar = nullptr;
AEGfxTexture* playerRight = nullptr;
AEGfxTexture* playerLeft = nullptr;
AEGfxTexture* playerIdle = nullptr;
AEGfxVertexList* playerMesh = nullptr;
Player& player = Global::GetInstance()->player;
//f32 health_scalex = 130.0f, health_scaley = 20.0f;
//f32 health_posx = 270.0f, health_posy = -105.0f;
f32 health_scalex = 210.0f, health_scaley = 15.0f;
f32 health_posx = -140.0f, health_posy = 262.0f;
//f32 thealth_scalex = 260.0f, thealth_scaley = 20.0f;
f32 thealth_scalex = 210.0f, thealth_scaley = 15.0f;
//f32 thealth_posx = -190.0f, thealth_posy = 260.0f;
f32 thealth_posx = 140.0f, thealth_posy = 262.0f;
f32 bhealth_scalex = 0.0f, bhealth_scaley = 20.0f;
f32 bhealth_posx = 180.0f, bhealth_posy = -115.0f;
f32 shealth_scalex = 0.0f, shealth_scaley = 20.0f;
f32 shealth_posx = 180.0f, shealth_posy = -160.0f;
int prevHealth = 0;
int prevTowerHP = 0;
int manaBarHP = 0;
int seedBarHP = 0;
/*****************************************************/

/******************PARTICLE VARIABLES******************/
static Particle* particle;
static ParticleObj* particleObj;
const int PARTICLE_SIZE = 10;
//Flags
const unsigned int	FLAG_ACTIVE = 0x00000001;
const unsigned int	FLAG_VISIBLE = 0x00000002;
const unsigned int GAME_OBJ_INST_NUM_MAX = 2048U;
const unsigned int GAME_OBJ_NUM_MAX = 32U;
static unsigned int sGameObjNum;
int particleState = 0;
float particletimer = 0.0f;
/*****************************************************/

/******************HEALTHBAR VARIABLES******************/
AEGfxVertexList* healthbarMesh;
AEGfxTexture* healthbar;
/*****************************************************/

/******************ATTACK VARIABLES******************/
f32 PLAYER_SPEED = 70.0f;
f32 ATTACK_SPEED = 200.0f;
f32* f_oldpos = nullptr;
f32 oldpos = 0.0f, oldposy = 0.0f;
bool freeze;
AttackTimer* attackTimer;
AEGfxVertexList* regenMesh = nullptr;
AEGfxTexture* regen = nullptr;
AEGfxTexture* regen1 = nullptr;
AEGfxTexture* basicTex0 = nullptr;
AEGfxTexture* basicTex1 = nullptr;
AEGfxTexture* basicTex2 = nullptr;
AEGfxTexture* basicTex3 = nullptr;
AEGfxTexture* basicTex4 = nullptr;
AEGfxTexture* launchUlt = nullptr;
AEGfxTexture* aimbox = nullptr;
AEGfxVertexList* bombUlt = nullptr;
AEGfxVertexList* cooldown_time = nullptr;
const unsigned int ATTACK_OBJ_NUM_MAX = 5;
static unsigned int attackCount;
float m_offsetx = 390.0f, m_offsety = 0.0f;
float timer = 0, timerbox = 0, manatimer = 0;
float health = 0;
bool platform = false;
int attackhits = 0;
/*****************************************************/

//basic Player constructor
Player::Player() :

	alive{ 1 },
	lives{ 1 },
	direction{ right },
	mana{ 0 },
	height{ 1.0f },
	scale{ 50 },
	isJumping{ false },
	isMoving{ false },
	activate { 0 },
	step { 0 }
{
	posCurr.x = -190.0f;
	posCurr.y = 70.0f;
	velCurr.x = 0.0f;
	velCurr.y = 0.0f;
	HealthCurr = 1000;
	timer = 0.0f;
}

// Destructor
Player::~Player()
{
}

// regen the Player's bloodlust
void RegenMana()
{
	// timer for bloodlust
	manatimer += Global::GetInstance()->g_dt;
	// when timer reaches 0.5, add the player's bloodlust
	// then reset the timer
	if (manatimer >= 0.5f)
	{
		player.mana += player.mana < 100 ? 1 : 0;
		manatimer = 0.0f;
	}
	// set the limit for player's bloodlust
	if (player.mana >= 100)
		player.mana = 100;
}

// load the Player game object
void LoadPlayer()
{
	// start creating the mesh for Player
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	//Saving the mesh (list of triangles) in playerMeshRight
	playerMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(playerMesh, "Failed to create playerMesh!!");

	// load all the player textures
	playerRight = AEGfxTextureLoad(".\\Resources\\Player\\PlayerFrame3.png");
	AE_ASSERT_MESG(playerRight, "Failed to create playerRight texture!");

	playerLeft = AEGfxTextureLoad(".\\Resources\\Player\\PlayerFrame1.png");
	AE_ASSERT_MESG(playerLeft, "Failed to create playerLeft texture!");

	playerIdle = AEGfxTextureLoad(".\\Resources\\Player\\PlayerFrame2.png");
	AE_ASSERT_MESG(playerIdle, "Failed to create playerIdle texture!");
}

void LoadAttack()
{
	//Creating the cooldown shade object
	AEGfxMeshStart();
	//Creating the circle shape
	int Parts = 12;
	for (float i = 0; i < Parts; ++i)
	{
		AEGfxTriAdd(
			0.0f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
			cosf(i * 2 * PI / Parts) * 0.5f, sinf(i * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
			cosf((i + 1) * 2 * PI / Parts) * 0.5f, sinf((i + 1) * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	}
	cooldown_time = AEGfxMeshEnd();
	AE_ASSERT_MESG(cooldown_time, "fail to create cooldown_time!!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	bombUlt = AEGfxMeshEnd();
	AE_ASSERT_MESG(bombUlt, "Failed to create mesh bombUlt!!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	regenMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(regenMesh, "Failed to create regen mesh!!");

	regen = AEGfxTextureLoad(".\\Resources\\Attack\\regenshield.png");
	AE_ASSERT_MESG(regen, "Failed to create egg texture!");

	regen1 = AEGfxTextureLoad(".\\Resources\\Attack\\regenshield1.png");
	AE_ASSERT_MESG(regen, "Failed to create egg texture!");

	launchUlt = AEGfxTextureLoad(".\\Resources\\Attack\\egg.png");
	AE_ASSERT_MESG(launchUlt, "Failed to create egg texture!");

	aimbox = AEGfxTextureLoad(".\\Resources\\Attack\\aimbox.png");
	AE_ASSERT_MESG(aimbox, "Failed to create egg texture!");

	basicTex1 = AEGfxTextureLoad(".\\Resources\\Attack\\basicattack1.png");
	AE_ASSERT_MESG(basicTex1, "Failed to create basicattack1 texture!");

	basicTex2 = AEGfxTextureLoad(".\\Resources\\Attack\\basicattack2.png");
	AE_ASSERT_MESG(basicTex2, "Failed to create basicattack2 texture!");

	basicTex3 = AEGfxTextureLoad(".\\Resources\\Attack\\basicattack1.png");
	AE_ASSERT_MESG(basicTex3, "Failed to create basicattack3 texture!");

	basicTex4 = AEGfxTextureLoad(".\\Resources\\Attack\\basicattack1.png");
	AE_ASSERT_MESG(basicTex4, "Failed to create basicattack4 texture!");
}

// render the texture for regen attack of Player
void DrawRegen(Attack* attack)
{
	// check the current attack type
	if (attack->type == REGEN)
	{
		SpawnParticle(attack, ATTACK, PARTICLE3, 3);
		SpawnParticle(attack, ATTACK, PARTICLE3, 3);
		UpdateParticle();
		// Drawing player object
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		timer += Global::GetInstance()->g_dt;
		// change texture - to make it animated
		if (timer <= 0.3f)
			// set player texture
			AEGfxTextureSet(regen, 1.0f, 1.0f);
		else if (timer <= 0.6f && timer > 0.3f)
			// set player texture
			AEGfxTextureSet(regen, 1.0f, 1.0f);
		else if (timer <= 0.9 && timer)
			// set player texture
			AEGfxTextureSet(regen1, 1.0f, 1.0f);
		else if (timer <= 1.2f)
			// set player texture
			AEGfxTextureSet(regen1, 1.0f, 1.0f);
		else if (timer <= 1.5f)
		{
			// set player texture
			AEGfxTextureSet(regen, 1.0f, 1.0f);
			// reset timer
			timer = 0.0f;
		}
		// set transparency
		AEGfxSetTransparency(1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransform(attack->transform.m);
		AEGfxMeshDraw(regenMesh, AE_GFX_MDM_TRIANGLES);
	}
}

// unload all the meshes and textures related to the regen attack
void UnloadRegen()
{
	AEGfxMeshFree(regenMesh);
	AEGfxTextureUnload(regen);
	AEGfxTextureUnload(regen1);
}

// loading of particles
void LoadParticle()
{
	// dynamically allocating memory for particleObj
	particleObj = (ParticleObj*)calloc(GAME_OBJ_NUM_MAX, sizeof(ParticleObj));
	AE_ASSERT_ALLOC(particleObj);
	if (!particleObj)
	{
		exit(EXIT_FAILURE);
		return;
	}
	// dynamically allocating memory for particle
	particle = (Particle*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(Particle));
	if (!particle)
	{
		exit(EXIT_FAILURE);
		return;
	}
	AE_ASSERT_ALLOC(particle);
	sGameObjNum = 0;

	ParticleObj* pObj;

	// creating particle mesh
	pObj = particleObj + sGameObjNum++;
	pObj->type = PARTICLE1;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create particle mesh!!");


	//Creating the white particle mesh
	pObj = particleObj + sGameObjNum++;
	pObj->type = PARTICLE2;


	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xF4C1AD00, 0.0f, 0.0f,
		0.5f, -0.5f, 0xF4C1AD00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xF4C1AD00, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xF4C1AD00, 0.0f, 0.0f,
		0.5f, -0.5f, 0xF4C1AD00, 0.0f, 0.0f,
		0.5f, 0.5f, 0xF4C1AD00, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");

	//Creating the cyan particle
	pObj = particleObj + sGameObjNum++;
	pObj->type = PARTICLE3;


	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");

	// dynamically allocating memory for attack timer object
	attackTimer = (AttackTimer*)calloc(ATTACK_OBJ_NUM_MAX, sizeof(AttackTimer));
	if (!attackTimer)
	{
		exit(EXIT_FAILURE);
		return;
	}
	AE_ASSERT_ALLOC(attackTimer);
	attackTimer->type = NONE;

}

// creating the particle instances
Particle* CreateParticle(float scale, AEVec2* pPos, AEVec2* pVel, float dir, unsigned int type)
{
	// loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		Particle* pInst = particle + i;
		AEVec2 zero;
		AEVec2Zero(&zero);
		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = particleObj + type;
			pInst->flag = FLAG_ACTIVE | FLAG_VISIBLE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;
			pInst->lifetime = 0;

			// return the newly created instance
			return pInst;
		}
	}
	return 0;
}

// destroy the specific instance of the particle
void DestroyParticle(Particle* pInst)
{
	// if particle is destroyed before just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}

// free the particle list
void FreeParticle()
{
	// kill all object in the list
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		DestroyParticle(particle + i);
	}
}

// unload all particle objects and attack timer objects
void UnloadParticle()
{
	// free all CREATED mesh
	for (u32 i = 0; i < sGameObjNum; i++)
		AEGfxMeshFree(particleObj[i].pMesh);
	free(particle);
	free(particleObj);
	free(attackTimer);
}

// render the particles on the screen
void DrawParticle()
{
	Particle* pInst;
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		pInst = particle + i;

		// skip non-active object
		if ((particle->flag & FLAG_ACTIVE) == 0)
			continue;

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//AEGfxSetBlendMode(AE_GFX_BM_NONE);
		// draw all active object instances
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			Particle* pInst1 = particle + i;

			// skip non-active object
			if (0 == (pInst1->flag & FLAG_ACTIVE) || 0 == (pInst1->flag & FLAG_VISIBLE))
				continue;

			//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
			AEGfxSetTransform(pInst1->transform.m);
			AEGfxMeshDraw(pInst1->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

	}
}

// sets the particle stats for different type objects
template<typename T>
void SpawnParticle(T* type, int typeObj, int particle, int life)
{
	Particle* pParticle;

	AEVec2 position;
	if (typeObj == PLAYER)
	{
		position.x = type->posCurr.x;
		position.y = type->posCurr.y;
	}
	else if (typeObj == ATTACK)
	{
		position.x = type->posCurr.x;
		position.y = type->posCurr.y;
	}

	pParticle = CreateParticle(2.0f, &position, 0, 0.0f, particle);

	// randomize the particles lifetime
	pParticle->lifetime = rand() % life - 0.6;
	// randomize the particles velocity (0-2) and assign negative respectively
	switch (rand() % 2)
	{
	case 0:
		pParticle->velCurr.x = (f32)(rand() % 100);
		pParticle->velCurr.y = (f32)(rand() % 100);
		if (pParticle->velCurr.x < 20)
			pParticle->velCurr.x = 20;
		if (pParticle->velCurr.y < 20)
			pParticle->velCurr.y = 20;
		break;
	case 1:
		pParticle->velCurr.x = -(f32)(rand() % 100);
		pParticle->velCurr.y = (f32)(rand() % 100);
		if (pParticle->velCurr.x > -20)
			pParticle->velCurr.x = -20;
		if (pParticle->velCurr.y < 20)
			pParticle->velCurr.y = 20;
		break;
	default:
		break;
	}
}

// update of particles game instances
void UpdateParticle()
{
	Particle* pInst;
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		pInst = particle + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		// update all active particles positions accordingly
		pInst->posCurr.x += pInst->velCurr.x * Global::GetInstance()->g_dt;
		pInst->posCurr.y += pInst->velCurr.y * Global::GetInstance()->g_dt;

		// bounding rectangle of every active instances
		pInst->boundingBox.min.x = -0.5f * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.max.x = 0.5f * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.min.y = -0.5f * pInst->scale + pInst->posCurr.y;
		pInst->boundingBox.max.y = 0.5f * pInst->scale + pInst->posCurr.y;


		// destroy the particle when counter(lifetime) reaches less than 0
		// otherwise countinue deducting counter(lifetime) accordingly
		pInst->lifetime -= Global::GetInstance()->g_dt;
		if (pInst->lifetime < 0.0)
		{
			DestroyParticle(pInst);
		}
	}

	//Computing the transformation matrices of the game object instances
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		AEMtx33 scale, trans;
		pInst = particle + i;

		// skip non-active object instances
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		AEMtx33Concat(&pInst->transform, &trans, &scale);
	}
}

// update of particle instances
template <typename T1>
void UpdateParticleState(int& state, T1* type, int life)
{
	// timer for particle's lifetime
	particletimer -= Global::GetInstance()->g_dt;
	if (state == 1 && particletimer > 0.0f)
	{
		SpawnParticle(type, ATTACK, PARTICLE1, life);
		UpdateParticle();
	}
	else
	{
		// loop through the particles that are out of their lifetime and destroy it
		for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			Particle* pInst = particle + i;

			// skip non-active object
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			DestroyParticle(pInst);
		}
		// reset the stats
		state = 0;
		particletimer = 0.0f;
	}
}

// a cooldown feature for the player's attack
void AttackCooldown()
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		// cooldown for the regen attack button K
		if (attackTimer[i].type == REGEN)
		{
			attackTimer[i].timer += Global::GetInstance()->g_dt;
			if (attackTimer[i].timer >= attackTimer[i].cooldown)
			{
				attackTimer[i].timer = 0.0f;
				attackTimer[i].type = NONE;
			}
		}
		// cooldown for the ultimate attack button L
		else if (attackTimer[i].type == LAUNCH)
		{
			attackTimer[i].timer += Global::GetInstance()->g_dt;
			if (attackTimer[i].timer >= attackTimer[i].cooldown)
			{
				attackTimer[i].timer = 0.0f;
				attackTimer[i].type = NONE;
			}
		}
	}
}

// reset the attackTimer stats for cooldown usage
void FreeCoolDown()
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		attackTimer->type = NONE;
		attackTimer[i].timer = 0.0f;
		attackTimer[i].cooldown = 0.0f;
	}
}

// initializer for all Player attributes, attack attributes and miscellaneous
void InitPlayer(Attack* attack)
{
	player.alive = 1;
	player.lives = 1;
	player.direction = right;
	player.mana = 0;
	player.height = 1.0f;
	player.scale = 150;
	player.isJumping = false;
	player.isMoving = false;
	player.posCurr.x = -190.0f;
	player.posCurr.y = 70.0f;
	player.velCurr.x = 0.0f;
	player.velCurr.y = 0.0f;
	player.HealthCurr = 1000;
	player.timer = 0.0f;
	player.activate = 0;
	player.step = 0;

	attack->type = NONE;
	attack->boundingBox = { {0.0f,0.0f},{0.0f,0.0f} };
	attack->oldPos = { 0.0f,0.0f };
	attack->posCurr = { 0.0f, 0.0f };
	attack->scale = 0.0f;
	attack->velCurr = { 0.0f, 0.0f };

	health_scalex = 210.0f, health_scaley = 15.0f;
	health_posx = -140.0f, health_posy = 262.0f;
	thealth_scalex = 210.0f, thealth_scaley = 15.0f;
	thealth_posx = 140.0f, thealth_posy = 262.0f;
	prevHealth = 0;
	prevTowerHP = 0;
	particleState = 0;
	particletimer = 0.0f;
	oldpos = 0.0f, oldposy = 0.0f;
	freeze = false;
	attackCount = 0;
	m_offsetx = 390.0f, m_offsety = 0.0f;
	timer = 0, timerbox = 0, manatimer = 0;
	health = 0;
	platform = false;
	attackhits = 0;
	bhealth_scalex = 0.0f, bhealth_scaley = 20.0f;
	bhealth_posx = 180.0f, bhealth_posy = -115.0f;
	shealth_posx = 180.0f, shealth_posy = -160.0f;
	shealth_scalex = 0.0f, shealth_scaley = 20.0f;
	manaBarHP = 0;
	seedBarHP = 0;
}

// the jump physics for the Player
void JumpNew(f64 dt)
{
	if (player.posCurr.y >= 70.0f)
	{
		//player.isJumping = 1;
		sprite.oldPos_y = player.posCurr.y;
		sprite.oldPos_x = player.posCurr.x;
		sprite.newPos_y = sprite.oldPos_y + ((float)dt * sprite.velocity);
		sprite.newPos_x = sprite.oldPos_x + ((float)dt * sprite.velocity);
		sprite.velocity = sprite.velocity + ((float)dt * sprite.gravity);
		//player.pos_x = sprite.newPos_x;
		player.posCurr.y = sprite.newPos_y;
		CameraPos(&camera, player.posCurr.x, 0.0f);
	}
	else if (player.posCurr.y <= 70.0f)
	{
		// Player is on the ground, stop jumping.
		sprite.velocity = 150.0f;
		player.isJumping = false;
		// Force player to be at ground level.
		player.posCurr.y = 70.0f;
	}
}

// process user input and update the player's properties
void MovePlayerNew()
{
	f64 dt, currTime, prevTime;

	prevTime = 0.0f;
	currTime = AEFrameRateControllerGetFrameTime();

	// calculate delta time
	dt = currTime - prevTime;

	// maintain the frame
	if (dt > 0.15f)
		dt = 0.15f;
	// enter this scope if the player is still alive
	if (player.alive)
	{
		// check inputs and jump the player
		if (AEInputCheckTriggered(VK_SPACE))
		{
			player.isJumping = true;
			if (Global::GetInstance()->volumeGlobal > 0.0f)
			{
				PlayAudio(SOUNDTYPE::ST_SPACE);
				SetSFXVolume(Global::GetInstance()->volumeGlobal);
			}	
		}
		// enter this scope if isJumping is set to true
		if (player.isJumping)
		{
			JumpNew(dt);
		}
		// check inputs and move the player, player.step is for the button left and right render use
		if (AEInputCheckCurr(AEVK_A) || player.step == 1)
		{
			// new temporary vector for velocity calculations to be operated on
			AEVec2 heroVel = { 0.0f }, heroVely = { 0.0f }, heroVelx = { 0.0f };
			// Set the velocity 
			AEVec2Set(&heroVel, PLAYER_SPEED, PLAYER_SPEED);
			AEVec2Add(&heroVel, &heroVel, &heroVel);
			AEVec2Add(&heroVelx, &heroVel, &heroVelx);
			AEVec2Neg(&heroVelx, &heroVelx);
			player.velCurr.x = heroVelx.x;
			// update the hero position and direction
			player.posCurr.x += player.velCurr.x * Global::GetInstance()->g_dt;
			player.direction = left;
			player.isMoving = true;

			// update camera positioning
			CameraPos(&camera, player.posCurr.x, 0.0f);
		}
		// check inputs and move the player, player.step is for the button left and right render use
		else if (AEInputCheckCurr(AEVK_D) || player.step == 2)
		{
			// new temporary vector for velocity calculations to be operated on
			AEVec2 heroVel = { 0.0f }, heroVely = { 0.0f }, heroVelx = { 0.0f };
			// Set the velocity 
			AEVec2Set(&heroVel, PLAYER_SPEED, PLAYER_SPEED);
			AEVec2Add(&heroVel, &heroVel, &heroVel);
			AEVec2Add(&heroVelx, &heroVel, &heroVelx);
			player.velCurr.x = heroVelx.x;
			// update the hero position and direction
			player.posCurr.x += player.velCurr.x * Global::GetInstance()->g_dt;
			player.direction = right;
			player.isMoving = true;

			// update camera positioning
			CameraPos(&camera, player.posCurr.x, 0.0f);
		}
		else
			player.isMoving = false;

		// calculate the player bounding box
		player.boundingBox.min.x = -0.5f * player.scale + player.posCurr.x;
		player.boundingBox.max.x = 0.5f * player.scale + player.posCurr.x;
		player.boundingBox.min.y = -0.5f * player.scale + player.posCurr.y;
		player.boundingBox.max.y = 0.5f * player.scale + player.posCurr.y;
		// reset the button var for render
		player.step = 0;
	}
}

// updates the player's movement, physics
void UpdatePlayer()
{
	if (player.alive)
	{
		// local matrix and vector declarations to be operated on
		AEMtx33 trans, rot, scale, reflection;
		AEVec2 reflect;
		// set the reflection matrix with the values of reflection y-axis matrix
		reflect.x = -1;
		reflect.y = 0;
		AEMtx33SetCol(&reflection, 0, &reflect);
		reflect.x = 0;
		reflect.y = 1;
		AEMtx33SetCol(&reflection, 1, &reflect);
		// since this is a 2 matrix, set the 3rd col to be 0
		reflect.x = 0;
		reflect.y = 0;
		AEMtx33SetCol(&reflection, 2, &reflect);

		// when player not moving force the direction to face right
		if (!player.isMoving)
		{
			player.direction = right;
		}
		// calculate the transformation when player is facing right
		if (player.direction == right)
		{
			// Compute the scaling matrix
			AEMtx33Scale(&scale, player.scale, player.scale);
			// Compute the rotation matrix 
			AEMtx33Rot(&rot, player.direction);
			// Compute the translation matrix
			AEMtx33Trans(&trans, player.posCurr.x, player.posCurr.y);
			// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
			// store the multiplication result of rotation and translation matrix into trans
			AEMtx33Concat(&trans, &trans, &rot);
			// store the multiplication of the scaled object and translated object into transform
			AEMtx33Concat(&trans, &trans, &scale);
			player.transform = trans;
		}
		// calculate the transformation when player is facing left
		else if (player.direction == left)
		{
			// Compute the scaling matrix
			AEMtx33Scale(&scale, player.scale, player.scale);
			// Compute the rotation matrix 
			AEMtx33Rot(&rot, right);
			// Compute the translation matrix
			AEMtx33Trans(&trans, player.posCurr.x, player.posCurr.y);
			// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
			// store the multiplication result of rotation and translation matrix into trans
			AEMtx33Concat(&trans, &trans, &rot);
			// store the multiplication of the scaled object and translated object into transform
			AEMtx33Concat(&trans, &trans, &scale);
			// finally multiply the translated matrix with the reflection matrix
			// reflecting along y-axis
			AEMtx33Concat(&trans, &trans, &reflection);
			player.transform = trans;
		}
	}
	// limit the player movement boundary along the x-axis
	if (player.posCurr.x <= -400.0f)
	{
		player.isMoving = false;
		player.posCurr.x = -400.0f;
	}
	if (player.posCurr.x >= 1900.0f - 50.0f)
	{
		player.isMoving = false;
		player.posCurr.x = 1900.0f - 50.0f;
	}
	// check for player's health
	CheckLives();
	// regenerate bloodlust
	RegenMana();
}

// renders the player onto the screen
void DrawPlayer()
{
	// enter this scope if player is still alive
	if (player.alive)
	{
		// Drawing player object
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		// Set position for player object
		AEGfxSetPosition(player.posCurr.x, player.posCurr.y);
		if (player.isMoving == true)
		{
			timer += Global::GetInstance()->g_dt;
			// change texture - to make it animated
			if (timer <= 0.3f)
				// set player texture
				AEGfxTextureSet(playerRight, 1.0f, 1.0f);
			else if (timer <= 0.6f && timer > 0.3f)
				// set player texture
				AEGfxTextureSet(playerLeft, 1.0f, 1.0f);
			else if (timer <= 0.9 && timer)
				// set player texture
				AEGfxTextureSet(playerIdle, 1.0f, 1.0f);
			else if (timer <= 1.2f)
				// set player texture
				AEGfxTextureSet(playerLeft, 1.0f, 1.0f);
			else if (timer <= 1.5f)
			{
				// set player texture
				AEGfxTextureSet(playerLeft, 1.0f, 1.0f);
				// reset timer
				timer = 0.0f;
			}
		}
		else
		{
			// set player texture
			AEGfxTextureSet(playerRight, 1.0f, 1.0f);
		}
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(player.transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxMeshDraw(playerMesh, AE_GFX_MDM_TRIANGLES);
	}
}

// update the player's attack stats
void PlayerAttack(Attack& attack, AEVec2 pos)
{
	// enter this scope if player is alive
	if (player.alive)
	{
		// basic attack
		if ((AEInputCheckReleased(AEVK_J) || player.activate == 1) && !attack.type)
		{
			if (Global::GetInstance()->volumeGlobal > 0.0f)
			{
				PlayAudio(SOUNDTYPE::ST_PLAYERATTACKMONSTER);
				SetSFXVolume(Global::GetInstance()->volumeGlobal);
			}
			// set the basic attack bloodlust cost	
			attack.cost.cbasic = 3;
			// checks if there is enough bloodlust on the player and if the player is facing right direction
			// only when the player is facing right direction then can the player summon the attack
			if (player.mana >= attack.cost.cbasic && player.direction == right)
			{
				// set all the stats for the player and the attack
				player.direction = right;
				player.isMoving = false;
				attack.type = BASIC;
				attack.scale = 140.0f;
				attack.posCurr.x = player.posCurr.x + 25.0f;
				attack.posCurr.y = player.posCurr.y + 50.0f;
				AEVec2 vel;
				AEVec2Set(&vel, ATTACK_SPEED, 0.0f);
				AEVec2Add(&vel, &vel, &vel);
				attack.velCurr.y = vel.y;
				attack.velCurr.x = vel.x;
				// update the current mana that is on the player with the bloodlust cost
				UpdateMana(attack);
			}
		}
		// regenerate health
		else if ((AEInputCheckReleased(AEVK_K) || player.activate == 2) && !attack.type)
		{
			// set the basic attack bloodlust cost	
			attack.cost.cregen = 10;
			// checks if there is enough bloodlust on the player and if the player is facing right direction
			// only when the player is facing right direction then can the player summon the attack
			if (player.mana >= attack.cost.cregen && attackTimer[1].type == NONE)
			{
				// set all the stats for the attack and its cooldown
				attackTimer[1].type = REGEN;
				attackTimer[1].timer = 5.0f;
				attackTimer[1].cooldown = 3.0f;
				attack.type = REGEN;
				attack.scale = 140.0f;
				attack.posCurr.x = player.posCurr.x + 25.0f;
				attack.posCurr.y = player.posCurr.y + 50.0f;
				AEVec2 vel;
				AEVec2Set(&vel, ATTACK_SPEED, 0.0f);
				AEVec2Add(&vel, &vel, &vel);
				attack.velCurr.y = vel.y;
				attack.velCurr.x = vel.x;
				// update the current mana that is on the player with the bloodlust cost
				UpdateMana(attack);
			}
		}
		// ultimate attack
		else if ((AEInputCheckCurr(AEVK_L) || player.activate == 3) && attack.type != LAUNCH)
		{
			// set the platform box for aim assist
			platform = true;
			attack.type = BOMB;
			attack.scale = 150.0f;
			attack.posCurr.x = pos.x - m_offsetx;
			attack.posCurr.y = -55.0f;
			attack.posCurr.x += camera.axis.x;
		}
		// update the current platform box for aim assist
		if (platform)
		{
			attack.type = BOMB;
			attack.posCurr.x = pos.x - m_offsetx;
			attack.posCurr.x += camera.axis.x;
		}
		// summon the ultimate attack left mouse is clicked
		if (AEInputCheckReleased(AEVK_LBUTTON) && attack.type == BOMB && attackTimer[2].type == NONE)
		{
			// set the basic attack bloodlust cost	
			attack.cost.cultimate = 30;
			if (player.mana >= attack.cost.cultimate)
			{
				// set all the stats for the attack and its cooldown
				attackTimer[2].type = LAUNCH;
				attackTimer[2].timer = 0.0f;
				attackTimer[2].cooldown = 10.0f;
				AEVec2 vel;
				AEVec2Set(&vel, 0.0f, ATTACK_SPEED);
				AEVec2Add(&vel, &vel, &vel);
				AEVec2Neg(&vel, &vel);
				attack.velCurr.y = vel.y;
				attack.velCurr.x = vel.x;
				attack.posCurr.y = 250.0f;
				attack.type = LAUNCH;
				attack.scale = 90.0f;
				// check for mana
				UpdateMana(attack);
				platform = false;
			}
		}
		// calculate the attack bounding box
		attack.boundingBox.min.x = -0.5f * attack.scale + attack.posCurr.x;
		attack.boundingBox.max.x = 0.5f * attack.scale + attack.posCurr.x;
		attack.boundingBox.min.y = -0.5f * (attack.scale * 2) + attack.posCurr.y;
		attack.boundingBox.max.y = 0.5f * (attack.scale * 2) + attack.posCurr.y;
		// update the attack skills cooldown for the player
		AttackCooldown();
		// reset the attack activation of the player
		player.activate = 0;
	}
}

// update the general physics and math of the ultimate
void UpdateUlt(Attack* attack)
{
	AEMtx33	trans, rot, scale;

	if (attack->type == BOMB)
	{
		// Compute the scaling matrix
		AEMtx33Scale(&scale, attack->scale, attack->scale);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, attack->direction);
		// Compute the translation matrix
		AEMtx33Trans(&trans, attack->posCurr.x, attack->posCurr.y + 10.0f);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		// store the multiplication result of rotation and translation matrix into trans
		AEMtx33Concat(&trans, &trans, &rot);
		// store the multiplication of the scaled object and translated object into transform
		AEMtx33Concat(&trans, &trans, &scale);
		attack->transform = trans;

		timerbox += Global::GetInstance()->g_dt;
		// limit the bomb ulti duration
		if (timerbox >= 3.0f)
		{
			// reset the timer
			timerbox = 0;
			attack->type = NONE;
			platform = false;
		}
	}
	else if (attack->type == LAUNCH)
	{
		SpawnParticle(attack, ATTACK, PARTICLE1, 2);
		UpdateParticle();
		// Compute the scaling matrix
		AEMtx33Scale(&scale, attack->scale, attack->scale);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, attack->direction);
		// Compute the translation matrix
		AEMtx33Trans(&trans, attack->posCurr.x, attack->posCurr.y);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		// store the multiplication result of rotation and translation matrix into trans
		AEMtx33Concat(&trans, &trans, &rot);
		// store the multiplication of the scaled object and translated object into transform
		AEMtx33Concat(&trans, &trans, &scale);
		attack->transform = trans;
	}
	// enter this scope if utlimate attack is summoned
	if (attack->type == LAUNCH)
	{
		// updates the current attack positioning
		attack->posCurr.y += attack->velCurr.y * Global::GetInstance()->g_dt;
		// checks for collision of the attack and the boss
		if (CollisionIntersection_RectRect(Global::GetInstance()->boss_lvll1.boundingBox, Global::GetInstance()->boss_lvll1.velCurr, attack->boundingBox, attack->velCurr))
		{
			for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
			{
				Particle* pInst = particle + i;

				// skip non-active object
				if ((pInst->flag & FLAG_ACTIVE) == 0)
					continue;

				DestroyParticle(pInst);
			}
			if (Global::GetInstance()->boss_skills)
			{
				attack->type = NONE; // reset the attack type
			}
			Global::GetInstance()->boss_lvll1.HealthCurr -= 300; // deduct the boss's health
		}
		// loop through the enemy object instances list
		for (auto& obj : Global::GetInstance()->Enemy_List)
		{
			// if object type is the enemy tower, enter this scope
			if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_TOWER)
			{
				// checks for collision of the attack and the enemy tower
				if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr, attack->boundingBox, attack->velCurr))
				{
					for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
					{
						Particle* pInst = particle + i;

						// skip non-active object
						if ((pInst->flag & FLAG_ACTIVE) == 0)
							continue;

						DestroyParticle(pInst);
					}
					if (Global::GetInstance()->volumeGlobal > 0.0f)
					{
						PlayAudio(SOUNDTYPE::ST_MONSTERDEATH);
						SetSFXVolume(Global::GetInstance()->volumeGlobal);
					}
					// to display the explosion render upon collision with the tower	
					Global::GetInstance()->Explosion_list.push_back(Explosion(obj));
					attack->type = NONE; // reset the attack type
					obj->HealthCurr -= 100; // deduct the tower's health

				}
			}
			// checks if the current object in the loop is either an enemy duck, penguin or ostrich
			else if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_DUCK
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_PENGIUN
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_OSTRICH)
			{
				// check for collision between these objects
				if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr, attack->boundingBox, attack->velCurr))
				{
					for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
					{
						Particle* pInst = particle + i;

						// skip non-active object
						if ((pInst->flag & FLAG_ACTIVE) == 0)
							continue;

						DestroyParticle(pInst);
					}
					// to display the explosion render upon collision with the tower
					Global::GetInstance()->Explosion_list.push_back(Explosion(obj));
					obj->HealthCurr -= 50;
					if (Global::GetInstance()->volumeGlobal > 0.0f)
					{
						PlayAudio(SOUNDTYPE::ST_MONSTERDEATH);
						SetSFXVolume(Global::GetInstance()->volumeGlobal);
					}
				}
			}
		}
		// remove attack if it hits the ground level
		if (attack->boundingBox.min.y <= -58.0f)
		{
			attack->oldPos.x = attack->posCurr.x;
			attack->oldPos.y = attack->posCurr.y;
			attack->type = NONE;
			// draw the particles explosion
			particleState = 1;
			particletimer = 0.5f;
		}
	}
	// if the particle state is set to true enter this scope
	if (particleState)
	{
		// update the current particles on screen
		UpdateParticleState(particleState, attack, 100);
	}
}

// renders the player's ultimate attack
void RenderUlt(Attack* attack)
{
	if (attack->type == BOMB)
	{
		// Drawing player object
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		// No texture for object 1
		AEGfxTextureSet(aimbox, 1.0f, 1.0f);
		// set transparency
		AEGfxSetTransparency(1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransform(attack->transform.m);
		AEGfxMeshDraw(bombUlt, AE_GFX_MDM_TRIANGLES);
	}
	else if (attack->type == LAUNCH)
	{
		// Drawing player object
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		// No texture for object 1
		AEGfxTextureSet(launchUlt, 1.0f, 1.0f);
		// set transparency
		AEGfxSetTransparency(1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransform(attack->transform.m);
		AEGfxMeshDraw(bombUlt, AE_GFX_MDM_TRIANGLES);
	}
	// renders the particles onto the screen
	DrawParticle();
}

// update the physics and movement of the player's attack
void UpdateAttack(Attack* attack)
{
	// local matrix and vector declarations to be operated on
	AEMtx33 trans, rot, scale, reflection;
	AEVec2 reflect;
	// set the reflection matrix with the values of reflection y-axis matrix
	reflect.x = -1;
	reflect.y = 0;
	AEMtx33SetCol(&reflection, 0, &reflect);
	reflect.x = 0;
	reflect.y = 1;
	AEMtx33SetCol(&reflection, 1, &reflect);
	// since this is a 2 matrix, set the 3rd col to be 0
	reflect.x = 0;
	reflect.y = 0;
	AEMtx33SetCol(&reflection, 2, &reflect);

	if (attack->type == BASIC || attack->type == REGEN)
	{
		// Compute the scaling matrix
		AEMtx33Scale(&scale, attack->scale + 60.0f, attack->scale + 60.0f);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, right);
		// Compute the translation matrix
		AEMtx33Trans(&trans, attack->posCurr.x + 50.0f, attack->posCurr.y);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		// store the multiplication result of rotation and translation matrix into trans
		AEMtx33Concat(&trans, &trans, &rot);
		// store the multiplication of the scaled object and translated object into transform
		AEMtx33Concat(&trans, &trans, &scale);
		attack->transform = trans;
	}
	// enter this scope if the current attack type is a basic attack
	if (attack->type == BASIC)
	{
		// updates the current position of the basic attack along the x-axis with respect to g_dt
		attack->posCurr.x += attack->velCurr.x * Global::GetInstance()->g_dt;
		// checks for collision between the attack and the boss
		if (CollisionIntersection_RectRect(Global::GetInstance()->boss_lvll1.boundingBox, Global::GetInstance()->boss_lvll1.velCurr, attack->boundingBox, attack->velCurr))
		{
			for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
			{
				Particle* pInst = particle + i;

				// skip non-active object
				if ((pInst->flag & FLAG_ACTIVE) == 0)
					continue;

				DestroyParticle(pInst);
			}
			if (Global::GetInstance()->boss_skills)
			{
				attack->type = NONE; // resets the attack to NONE
			}
			Global::GetInstance()->boss_lvll1.HealthCurr -= 50; // deducts the boss's health
			Global::GetInstance()->boss_lvll1.posCurr.x += 500 * Global::GetInstance()->g_dt; // pushback the boss
		}
		// loop through the enemy list object instances
		for (auto& obj : Global::GetInstance()->Enemy_List)
		{
			// check if the current object is an enemy tower and enter this scope
			if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_TOWER)
			{
				// check for collision between the basic attack and the enemy tower
				if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr, attack->boundingBox, attack->velCurr))
				{
					obj->HealthCurr -= 30; // deduct the tower's health
					Global::GetInstance()->Explosion_list.push_back(Explosion(obj)); // adds explosion texture upon collision
					attack->type = NONE; // resets the current attack type
					if (Global::GetInstance()->volumeGlobal > 0.0f)
					{
						PlayAudio(SOUNDTYPE::ST_MONSTERDEATH);
						SetSFXVolume(Global::GetInstance()->volumeGlobal);
					}
				}
			}
			// check if the current object is an enemy minions of, duck, penguin or ostrich
			else if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_DUCK
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_PENGIUN
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_OSTRICH)
			{
				// deduct tower health if enemy minion collides
				// break to only destroy one minion
				if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr, attack->boundingBox, attack->velCurr))
				{
					obj->HealthCurr -= 50; // deduct the minions health
					player.mana += 1; // give 1 bloodlust if hit
					Global::GetInstance()->Explosion_list.push_back(Explosion(obj)); // adds explosion texture upon collision
					if (Global::GetInstance()->volumeGlobal > 0.0f)
					{
						PlayAudio(SOUNDTYPE::ST_MONSTERDEATH);
						SetSFXVolume(Global::GetInstance()->volumeGlobal);
					}
					attack->type = NONE; // resets the current attack type
					break;
				}
			}
		}
	}
	// checks if the current attack type is regen and enter this scope if true
	else if (attack->type == REGEN)
	{
		// updates the current position of the attack along the x-axis
		attack->posCurr.x += attack->velCurr.x * Global::GetInstance()->g_dt;
		// checks for collision between the boss and the attack
		if (CollisionIntersection_RectRect(Global::GetInstance()->boss_lvll1.boundingBox, Global::GetInstance()->boss_lvll1.velCurr, attack->boundingBox, attack->velCurr))
		{
		    for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		    {
		        Particle* pInst = particle + i;

		        // skip non-active object
		        if ((pInst->flag & FLAG_ACTIVE) == 0)
		            continue;

		        DestroyParticle(pInst);
		    }
			if (Global::GetInstance()->boss_skills)
			{
				attack->type = NONE; // resets the attack type
			}
		    Global::GetInstance()->boss_lvll1.HealthCurr -= 50; // deduct the boss's health upon collision
		}
		// loop through the enemy list objects instances
		for (auto& obj : Global::GetInstance()->Enemy_List)
		{
			// check if the current object is an enemy tower and enter this scope if true
			if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_TOWER)
			{
				// check for collision between attack and the enemy tower
				if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr, attack->boundingBox, attack->velCurr))
				{
					if (Global::GetInstance()->volumeGlobal > 0.0f)
					{
						PlayAudio(SOUNDTYPE::ST_MONSTERDEATH);
						SetSFXVolume(Global::GetInstance()->volumeGlobal);
					}
					obj->HealthCurr -= 10; // deduct the tower's health
					Global::GetInstance()->Explosion_list.push_back(Explosion(obj)); // adds the explosion image
					attack->type = NONE; // resets the attack type
				}
			}
			// checks for the current object type if it is an enemy duck, penguin or ostrich
			else if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_DUCK
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_PENGIUN
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_OSTRICH)
			{
				// deduct tower health if enemy minion collides
				// break to only destroy one minion
				// check for collision between enemy minions and attack
				if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr, attack->boundingBox, attack->velCurr))
				{
					if (Global::GetInstance()->volumeGlobal > 0.0f)
					{
						PlayAudio(SOUNDTYPE::ST_MONSTERDEATH);
						SetSFXVolume(Global::GetInstance()->volumeGlobal);
					}
					// get the current position of the minion object and store to a pointer
					f_oldpos = &obj->posCurr.x;
					// copy the old position of the current minion object and store in oldpos
					oldpos = obj->posCurr.x;
					// copy the old position of the current minion object and store in oldposy
					oldposy = obj->posCurr.y;
					obj->HealthCurr -= 20; // deduct the current minion health
					player.mana += 2; // give 2 bloodlust to the player
					freeze = true; // set freeze to true
					Global::GetInstance()->Explosion_list.push_back(Explosion(obj)); // add explosion texture upon collision
					attack->type = NONE; // resets the player attack
				}
			}
		}
	}
	// destory the particles when attack type is none
	else if (attack->type == NONE)
	{
		for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			Particle* pInst = particle + i;

			// skip non-active object
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			DestroyParticle(pInst);
		}
	}
	// when freeze is set to true enter this scope
	if (freeze)
	{
		// get the copy of the minions old position from the freeze attack
		// and store in the f_oldpos pointer to be fixed for 5 seconds
		// disable the movement of the minion object that was stored
		// using the address
		*f_oldpos = oldpos;
		player.timer += Global::GetInstance()->g_dt; // update the timer for the attack

		// when timer reaches 5 seconds enter this scope
		if (player.timer >= 5.0f)
		{
			// reset the values of the timer, attack and position to be used later
			player.timer = 0.0f;
			freeze = false;
			oldpos = 0.0f;
			f_oldpos = nullptr;
			// and destory all particles of the freeze attack
			for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
			{
				Particle* pInst = particle + i;

				// skip non-active object
				if ((pInst->flag & FLAG_ACTIVE) == 0)
					continue;

				DestroyParticle(pInst);
			}
		}
	}

	/* detect enemy minions bullet collision with player update behaviour */;
	auto& BulletList = Global::GetInstance()->BulletList;
	int index = 0;
	// loop through all the bullet list of the enemy
	for (auto& obj : Global::GetInstance()->BulletList)
	{
		// check if the object is an enemy
		if (obj.enemy)
		{
			// check if the object is an enemy bullet
			if (obj.pObject->type == TYPE_OBJECT::TYPE_OBJECT_BULLET)
			{
				// check for collision between the enemy bullet and the player
				if (CollisionIntersection_RectRect(obj.boundingBox, obj.velCurr, player.boundingBox, player.velCurr))
				{
					BulletList.erase(BulletList.begin() + index); // delete the bullet upon collision
					if (Global::GetInstance()->volumeGlobal > 0.0f)
					{
						PlayAudio(SOUNDTYPE::ST_SPACE);
						SetSFXVolume(Global::GetInstance()->volumeGlobal);
					}
					Global::GetInstance()->Explosion_list.push_back(Explosion(&player)); // add explosion texture
					player.HealthCurr -= 10; // deduct the player's health
				}
			}
		}	
	}
	++index;
}

// render of the player's basic attack
void DrawAttack(Attack* attack)
{
	if (attack->type == BASIC)
	{
		SpawnParticle(attack, ATTACK, PARTICLE1, 5);
		UpdateParticle();
		timerbox += Global::GetInstance()->g_dt;
		// Drawing player object
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		// set player texture
		timer += Global::GetInstance()->g_dt;
		// change texture - to make it animated
		if (timer <= 0.3f)
			// set player texture
			AEGfxTextureSet(basicTex4, 1.0f, 1.0f);
		else if (timer <= 0.6f && timer > 0.3f)
			// set player texture
			AEGfxTextureSet(basicTex1, 1.0f, 1.0f);
		else if (timer <= 0.9 && timer)
			// set player texture
			AEGfxTextureSet(basicTex2, 1.0f, 1.0f);
		else if (timer <= 1.2f)
			// set player texture
			AEGfxTextureSet(basicTex3, 1.0f, 1.0f);
		else if (timer <= 1.5f)
		{
			// set player texture
			AEGfxTextureSet(basicTex4, 1.0f, 1.0f);
			// reset timer
			timer = 0.0f;
		}
		// set transparency
		AEGfxSetTransparency(1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransform(attack->transform.m);
		AEGfxMeshDraw(bombUlt, AE_GFX_MDM_TRIANGLES);
	}
}

// check the health bar and live of player
// also used to update all the bars, 
// player hp bar, bird seed bar, bloodlust bar, tower health bar
void CheckLives()
{
	
	if (Global::GetInstance()->pEnemy_Tower != nullptr)
	{
		auto& obj = Global::GetInstance()->player;
		auto& obj_tower = Global::GetInstance()->pEnemy_Tower;
		auto& obj_birdseed = Global::GetInstance()->BirdSeed;
		int diff = 0, diff_tower = 0, diff_mana = 0, diff_birdseed = 0;

		if (AEInputCheckCurr(AEVK_3))
		{
			obj.mana += 1;
			obj_birdseed += 10;
			if (obj_birdseed >= 999)
				obj_birdseed = 999;
		}
		if (AEInputCheckCurr(AEVK_4))
		{
			obj.HealthCurr -= 10;
		}

		/* update player and tower health */
		diff = prevHealth - obj.HealthCurr; // get the difference of player's health each loop
		diff_tower = prevTowerHP - obj_tower->HealthCurr; // get the difference of tower's health each loop
		diff_mana = obj.mana - manaBarHP; // get the difference of bloodlust's count each loop
		diff_birdseed = obj_birdseed - seedBarHP; // get the difference of birdseed's count each loop

		// enter this scope if bloodlust is >= 0
		if (obj.mana >= 0)
		{
			// adjust the scale of the mesh and position accordingly when it updates
			bhealth_scalex += (diff_mana * 108) * Global::GetInstance()->g_dt;
			bhealth_posx += (diff_mana * 108) / 2 * Global::GetInstance()->g_dt;
			// reassign the manaBarHP with current bloodlust count to be operated on
			manaBarHP = obj.mana;
		}
		// enter this scope if obj_birdseed is >= 0
		if (obj_birdseed >= 0)
		{
			// adjust the scale of the mesh and position accordingly when it updates
			shealth_scalex += (diff_birdseed * 10.8f) * Global::GetInstance()->g_dt;
			shealth_posx += (diff_birdseed * 10.8f) / 2 * Global::GetInstance()->g_dt;
			// reassign the seedBarHP with current obj_birdseed count to be operated on
			seedBarHP = obj_birdseed;
		}
		// enter this scope if obj.HealthCurr > 0 && diff > 0
		if (obj.HealthCurr > 0 && diff > 0)
		{
			// adjust the scale of the mesh and position accordingly when it updates
			health_scalex -= (diff * 12.5f) * Global::GetInstance()->g_dt;
			health_posx -= (diff * 12.5f) / 2 * Global::GetInstance()->g_dt;
			// reassign the prevHealth with current obj.HealthCurr count to be operated on
			prevHealth = obj.HealthCurr;
		}
		// enter this scope if obj_tower->HealthCurr > 0 && diff_tower > 0
		if (obj_tower->HealthCurr > 0 && diff_tower > 0)
		{
			// adjust the scale of the mesh and position accordingly when it updates
			thealth_scalex -= (diff_tower * 12.5f) * Global::GetInstance()->g_dt;
			thealth_posx -= (diff_tower * 12.5f) / 2 * Global::GetInstance()->g_dt;
			// reassign the prevTowerHP with current obj_tower->HealthCurr count to be operated on
			prevTowerHP = obj_tower->HealthCurr;
		}
		// reassign the object variables just to be sure
		prevHealth = obj.HealthCurr;
		prevTowerHP = obj_tower->HealthCurr;
		manaBarHP = obj.mana;
	}
	
}

// load the health bar MESHES AND TEXTURE
void LoadHealth()
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFF00FF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in manaBar
	manaBar = AEGfxMeshEnd();
	AE_ASSERT_MESG(manaBar, "Failed to manaBar mesh !!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFF00FF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in seedBar
	seedBar = AEGfxMeshEnd();
	AE_ASSERT_MESG(seedBar, "Failed to seedBar mesh!!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFF00FF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in towerHealth
	towerHealth = AEGfxMeshEnd();
	AE_ASSERT_MESG(towerHealth, "Failed to towerHealth mesh!!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in towerHealth_white
	towerHealth_white = AEGfxMeshEnd();
	AE_ASSERT_MESG(towerHealth_white, "Failed to towerHealth_white mesh!!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFF00FF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in playerHealth
	playerHealth = AEGfxMeshEnd();
	AE_ASSERT_MESG(playerHealth, "Failed to playerHealth mesh!!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	// Saving the mesh (list of triangles) in playerHealth_white
	playerHealth_white = AEGfxMeshEnd();
	AE_ASSERT_MESG(playerHealth_white, "Failed to playerHealth_white mesh!!");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	//Saving the mesh (list of triangles) in playerMeshRight
	healthbarMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(healthbarMesh, "Failed to create healthbarMesh!!");

	healthbar = AEGfxTextureLoad(".\\Resources\\UI\\healthbar.png");
	AE_ASSERT_MESG(healthbar, "Failed to create healthbar texture!");
}

// unload all meshes and textures relating to the barss
void FreeHealth()
{
	AEGfxMeshFree(playerHealth);
	AEGfxMeshFree(playerHealth_white);
	AEGfxMeshFree(towerHealth);
	AEGfxMeshFree(towerHealth_white);
	AEGfxMeshFree(manaBar);
	AEGfxMeshFree(seedBar);
	AEGfxMeshFree(healthbarMesh);
	AEGfxTextureUnload(healthbar);
	AEGfxMeshFree(cooldown_time);
}

// render the health bar and all the bars onto the screen
void DrawHealth()
{
	/***************DRAWING HEALTH BAR TOP SCREEN****************************/
	AEMtx33 trans6, scale6;
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(healthbar, 0.0f, 0.0f);

	//The following code will adjust the size of the buttons
	AEMtx33Scale(&scale6, AEGetWindowWidth() * 0.9f, AEGetWindowHeight() * 0.38f);
	AEMtx33Trans(&trans6, Global::GetInstance()->camera.axis.x, 255.0f + Global::GetInstance()->camera.axis.y);
	AEMtx33Concat(&trans6, &trans6, &scale6);
	AEGfxSetTransform(trans6.m);

	AEGfxMeshDraw(healthbarMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransparency(1.0f);
	/***************DRAWING HEALTH BAR TOP SCREEN****************************/

	/***************DRAWING BIRDSEEDS BAR ON UI****************************/
	// local matrix and vector declarations to be operated on
	AEMtx33 trans7, scale7;
	// Compute the scaling matrix
	AEMtx33Scale(&scale7, 170.0f, 20.0f);
	// Compute the translation matrix
	AEMtx33Trans(&trans7, 265.0f + camera.axis.x, shealth_posy);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans7, &trans7, &scale7);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans7.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(playerHealth_white, AE_GFX_MDM_TRIANGLES);

	// local matrix and vector declarations to be operated on
	AEMtx33 trans8, scale8;
	// Compute the scaling matrix
	AEMtx33Scale(&scale8, shealth_scalex, shealth_scaley);
	// Compute the translation matrix
	AEMtx33Trans(&trans8, shealth_posx + camera.axis.x, -160.0f);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans8, &trans8, &scale8);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans8.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(seedBar, AE_GFX_MDM_TRIANGLES);
	/***************DRAWING BIRDSEEDS BAR ON UI****************************/

	/***************DRAWING BLOODLUST BAR ON UI****************************/
	// local matrix and vector declarations to be operated on
	AEMtx33 trans5, scale5;
	// Compute the scaling matrix
	AEMtx33Scale(&scale5, 170.0f, 20.0f);
	// Compute the translation matrix
	AEMtx33Trans(&trans5, 265.0f + camera.axis.x, -115.0f);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans5, &trans5, &scale5);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans5.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(playerHealth_white, AE_GFX_MDM_TRIANGLES);

	/* drawing bloodlust bar */
	// local matrix and vector declarations to be operated on
	AEMtx33 trans4, scale4;
	// Compute the scaling matrix
	AEMtx33Scale(&scale4, bhealth_scalex, bhealth_scaley);
	// Compute the translation matrix
	AEMtx33Trans(&trans4, bhealth_posx + camera.axis.x, bhealth_posy);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans4, &trans4, &scale4);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans4.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(manaBar, AE_GFX_MDM_TRIANGLES);
	/***************DRAWING BLOODLUST BAR ON UI****************************/

	/***************DRAWING PLAYER HEALTH BAR ON TOP SCREEN****************************/
	/* drawing player health */
	// local matrix and vector declarations to be operated on
	AEMtx33 trans1, scale1;
	// Compute the scaling matrix
	AEMtx33Scale(&scale1, 210.0f, 15.0f);
	// Compute the translation matrix
	AEMtx33Trans(&trans1, 140.0f + camera.axis.x, 262.0f);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans1, &trans1, &scale1);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans1.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(playerHealth_white, AE_GFX_MDM_TRIANGLES);

	// local matrix and vector declarations to be operated on
	AEMtx33 trans, scale;
	// Compute the scaling matrix
	AEMtx33Scale(&scale, health_scalex, health_scaley);
	// Compute the translation matrix
	AEMtx33Trans(&trans, health_posx + camera.axis.x, health_posy);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans, &trans, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans.m);
	AEGfxSetTintColor(0.0f, 1.0f, 1.0f, 0.7f);
	AEGfxMeshDraw(playerHealth, AE_GFX_MDM_TRIANGLES);
	/***************DRAWING PLAYER HEALTH BAR ON TOP SCREEN****************************/


	/***************DRAWING TOWER HEALTH TOP SCREEN************************************/
	// local matrix and vector declarations to be operated on
	AEMtx33 trans2, scale2;
	// Compute the scaling matrix
	AEMtx33Scale(&scale2, 160.0f, 14.0f);
	// Compute the translation matrix
	AEMtx33Trans(&trans2, 165.0f + camera.axis.x, 261.0f);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans2, &trans2, &scale2);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans2.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(towerHealth_white, AE_GFX_MDM_TRIANGLES);

	// local matrix and vector declarations to be operated on
	AEMtx33 trans3, scale3;
	// Compute the scaling matrix
	AEMtx33Scale(&scale3, thealth_scalex, thealth_scaley);
	// Compute the translation matrix
	AEMtx33Trans(&trans3, thealth_posx + camera.axis.x, thealth_posy);
	// store the multiplication of the scaled object and translated object into transform
	AEMtx33Concat(&trans3, &trans3, &scale3);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(trans3.m);
	AEGfxSetTintColor(1.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(towerHealth, AE_GFX_MDM_TRIANGLES);
	/***************DRAWING TOWER HEALTH TOP SCREEN************************************/


	// Display cooldown timer
	// Ready checker for L
	char strBufferL[100];
	memset(strBufferL, 0, 100 * sizeof(char));

	if (attackTimer[2].timer > 0)
	{
		// local matrix and vector declarations to be operated on
		//AEMtx33 trans7, scale7;
		// Compute the scaling matrix
		AEMtx33Scale(&scale7, 70.0f, 70.0f);
		// Compute the translation matrix
		AEMtx33Trans(&trans7, 292.0f + camera.axis.x, -235.0f);
		// store the multiplication of the scaled object and translated object into transform
		AEMtx33Concat(&trans7, &trans7, &scale7);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(trans7.m);
		AEGfxSetTintColor(0.0f, 0.0f, 1.0f, 0.5f);
		AEGfxMeshDraw(cooldown_time, AE_GFX_MDM_TRIANGLES);

		sprintf_s(strBufferL, "%d", 10 - (int)attackTimer[2].timer);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Global::GetInstance()->fontId_large, strBufferL, 0.70f, -0.82f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Display real-time values for player's bloodlust, health, enemy tower health, birdseeds
	char strBuffer9[100];
	memset(strBuffer9, 0, 100 * sizeof(char));
	sprintf_s(strBuffer9, "%ld/100", Global::GetInstance()->player.mana);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Global::GetInstance()->fontId, strBuffer9, 0.71f, -0.41f, 1.5f, 0.0f, 0.0f, 1.0f);

	sprintf_s(strBuffer9, "%d/1000", Global::GetInstance()->player.HealthCurr);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Global::GetInstance()->fontId, strBuffer9, -0.30f, 0.85f, 1.5f, 0.0f, 0.0f, 0.0f);

	sprintf_s(strBuffer9, "%ld/1000", Global::GetInstance()->pEnemy_Tower->HealthCurr);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Global::GetInstance()->fontId, strBuffer9, 0.40f, 0.85f, 1.5f, 0.0f, 0.0f, 0.0f);

	sprintf_s(strBuffer9, "%d / 1000", Global::GetInstance()->BirdSeed);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Global::GetInstance()->fontId, strBuffer9, 0.65f, -0.56f, 1.5f, 0.0f, 0.0f, 1.f);
}

// update the player's bloodlust accordingly
void UpdateMana(Attack& attack)
{
	if (attack.type == BASIC)
		player.mana -= attack.cost.cbasic;
	else if (attack.type == REGEN)
		player.mana -= attack.cost.cregen;
	else if (attack.type == LAUNCH)
		player.mana -= attack.cost.cultimate;

}

// camera function to update the camera position accordingly
void CameraPos(Camera* cameral, f32 x, f32 y)
{
	//centers camera on provided x/y coords
	cameral->axis.x = x - cameral->width / 2;
	cameral->axis.y = y - cameral->height / 2;

	// keep camera in bounds
	if (levelWidth != 0 && levelHeight != 0)
	{
		if (cameral->axis.x < 0)
			cameral->axis.x = 0;
		if (cameral->axis.y < 0)
			cameral->axis.y = 0;
		if (cameral->axis.x > levelWidth - cameral->width)
			cameral->axis.x = levelWidth - cameral->width;
		if (cameral->axis.y > levelHeight - cameral->height)
			cameral->axis.y = levelHeight - cameral->height;
	}
	AEGfxSetCamPosition(cameral->axis.x, cameral->axis.y);
}

// function to process main menu input using keyboard
void MainMenu_ProcessInput(char& selection, float& btn_y)
{
	if (AEInputCheckReleased(AEVK_UP))
	{
		selection--;
		btn_y += 105.0f;
	}

	if (AEInputCheckReleased(AEVK_DOWN))
	{
		selection++;
		btn_y -= 105.0f;
	}

	btn_y = selection > 3 ? 78.0f : btn_y;
	btn_y = selection < 0 ? -235.0f : btn_y;
	selection = selection > 3 ? 0 : selection;
	selection = selection < 0 ? 3 : selection;
}

// function to process the main menu input using mouse
void MainMenu_Mouse(char& selection, f32& btn_y, s32 x, s32 y)
{
	if ((x >= 304 && x <= 490) && (y >= 190 && y <= 273))
	{
		selection = 0;
		btn_y = 80.0f;
		if (AEInputCheckReleased(AEVK_LBUTTON) & 1)
		{
			// display the level select screen
			Global::GetInstance()->isLevelSelect = true;
			Global::GetInstance()->level_select = 1;
		}
	}
	if ((x >= 319 && x <= 475) && (y >= 283 && y <= 370))
	{
		selection = 1;
		btn_y = -28.0f;
		if (AEInputCheckReleased(AEVK_LBUTTON) & 1)
		{
			// change the state -  OPTIONS
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_OPTIONS;
		}
	}
	if ((x >= 322 && x <= 470) && (y >= 381 && y <= 460))
	{
		selection = 2;
		btn_y = -132.0f;
		if (AEInputCheckReleased(AEVK_LBUTTON) & 1)
		{
			// change the state - CREDITS
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_INSTRUCTIONS;
		}
	}
	if ((x >= 333 && x <= 459) && (y >= 475 && y <= 542))
	{
		selection = 3;
		btn_y = -235.0f;
		if (AEInputCheckReleased(AEVK_LBUTTON) & 1)
		{
			// change the state - CREDITS
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_CREDITS;
		}
	}
	if ((x >= 697 && x <= 795) && (y >= 550 && y <= 580))
	{
		selection = 4;
		if (AEInputCheckReleased(AEVK_LBUTTON) & 1)
		{
			// Display confirmation
			Global::GetInstance()->isQuit = true;
		}
	}
}

// render the texture and meshes being passed fullscreen
void MainMenu_Render(AEGfxTexture* mainMenu, AEGfxVertexList* menu)
{
	AEGfxSetCamPosition(0.0f, 0.0f);
	// Drawing mainMenu
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	// Set position for object 1
	AEGfxSetPosition(0.0f, 0.0f);
	// set mainmenu texture
	AEGfxTextureSet(mainMenu, 1.0f, 1.0f);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Drawing the mesh (list of triangles)
	AEMtx33 scale, trans, transform;
	AEMtx33Scale(&scale, (f32)AEGetWindowWidth() * 1.0f, (f32)AEGetWindowHeight() * 1.0f);
	AEMtx33Trans(&trans, 0.0f, 0.0f);
	AEMtx33Concat(&transform, &trans, &scale);
	AEGfxSetTransform(transform.m);
	// render menu image
	AEGfxMeshDraw(menu, AE_GFX_MDM_TRIANGLES);
}

// render the meshes and texture being passed for main menu buttons
void menuButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, float& btn_x, float& btn_y, float scalex, float scaley)
{
	// Drawing menuButton
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	// Set position for object 1
	AEGfxSetPosition(btn_x, btn_y);
	// set the button textures for menu button
	AEGfxTextureSet(menuButton, 1.0f, 1.0f);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// set transparency
	AEGfxSetTransparency(1.0f);
	// Drawing the mesh (list of triangles)
	AEMtx33 scale_btn, trans_btn, transform_btn;
	AEMtx33Scale(&scale_btn, (f32)AEGetWindowWidth() * scalex, (f32)AEGetWindowHeight() * scaley);
	AEMtx33Trans(&trans_btn, btn_x, btn_y);
	AEMtx33Concat(&transform_btn, &trans_btn, &scale_btn);
	AEGfxSetTransform(transform_btn.m);
	AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);
}

// render a mesh to overlay on the main menu screen
void otherButton_Render(AEGfxVertexList* button, float x, float y, float scalex, float scaley)
{
	// Drawing button3 - (first) - No tint
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//blend to bg
	AEGfxSetBlendMode(AE_GFX_BM_ADD);
	// No tint
	AEGfxSetTintColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Drawing the mesh (list of triangles)
	AEMtx33 scale_btn, trans_btn, transform_btn;
	AEMtx33Scale(&scale_btn, (f32)AEGetWindowWidth() * scalex, (f32)AEGetWindowHeight() * scaley);
	AEMtx33Trans(&trans_btn, x, y);
	AEMtx33Concat(&transform_btn, &trans_btn, &scale_btn);
	AEGfxSetTransform(transform_btn.m);
	// Set Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);

}

// render the attack greyed when hovered over
void attackButton_Render(AEGfxVertexList* pMesh, AEGfxTexture* button_attack)
{
	AEMtx33 scale, trans, transform;

	//Create UI
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(button_attack, 0.0f, 0.0f);

	//The following code will adjust the size of the buttons
	AEMtx33Scale(&scale, 14.5f, 14.2f);
	AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x, Global::GetInstance()->camera.axis.y - 112.0f);
	//AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x, -112.0f);
	AEMtx33Concat(&transform, &trans, &scale);
	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransparency(1.0f);
}

// unload the mesh and textures related to the player
void FreePlayer()
{
	AEGfxMeshFree(playerMesh);
	playerMesh = nullptr;
	AEGfxTextureUnload(playerRight);
	playerRight = nullptr;
	AEGfxTextureUnload(playerLeft);
	playerLeft = nullptr;
	AEGfxTextureUnload(playerIdle);
	playerIdle = nullptr;
}

// unload the textures related to the attack
void FreeAttack()
{
	AEGfxTextureUnload(basicTex1);
	basicTex1 = nullptr;
	AEGfxTextureUnload(basicTex2);
	basicTex2 = nullptr;
	AEGfxTextureUnload(basicTex3);
	basicTex3 = nullptr;
	AEGfxTextureUnload(basicTex4);
	basicTex4 = nullptr;
}

// unload the textures related to the attack
void FreeRenderUlt()
{
	AEGfxMeshFree(bombUlt);
	bombUlt = nullptr;
	AEGfxTextureUnload(launchUlt);
	launchUlt = nullptr;
	AEGfxTextureUnload(aimbox);
	aimbox = nullptr;
}

// unload the textures related to the main menu
void FreeMenu(AEGfxVertexList* menu, AEGfxVertexList* button)
{
	AEGfxMeshFree(menu);
	menu = nullptr;
	AEGfxMeshFree(button);
	button = nullptr;
}

// unload the textures related to the main menu
void UnloadMenu(AEGfxTexture* mainMenu, AEGfxTexture* playButton, AEGfxTexture* optionsButton, AEGfxTexture* creditsButton, AEGfxTexture* instructionButton)
{
	AEGfxTextureUnload(mainMenu);
	mainMenu = nullptr;
	AEGfxTextureUnload(playButton);
	playButton = nullptr;
	AEGfxTextureUnload(optionsButton);
	optionsButton = nullptr;
	AEGfxTextureUnload(creditsButton);
	creditsButton = nullptr;
	AEGfxTextureUnload(instructionButton);
	instructionButton = nullptr;
}