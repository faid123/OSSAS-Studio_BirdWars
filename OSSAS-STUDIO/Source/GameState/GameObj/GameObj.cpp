/******************************************************************************
* \file			GameObj.cpp
* \brief		Contain all the Gameobj related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include <iostream>
#include "GameState/GameObj/GameObj.h"
#include "GameState/Global.h"
#include <algorithm>
#include "Helper/Helper.h"

#define GROUND_LEVEL -50.0f
#define WIN_MAX_WIDTH 400.0f
#define WIN_MAX_HEIGHT 300.0f

float Helper::ally_y = -3.0f;
float Helper::enemy_y = -3.0f;

/*****************************************************************************/
/*!
 \fn GameObjInst();
 \brief basic Constructor
*/
/*****************************************************************************/
GameObjInst::GameObjInst() :
	pObject{ nullptr },
	posCurr{ 0,0 },
	velCurr{ 0,0 },
	transform{ 0 },
	boundingBox{ 0 },
	AttackRange{ 0 },
	innerState{ INNER_STATE::INNER_STATE_ON_ENTER },
	timer{ 0.0f },
	enemy{ false },
	Start_Attacking{ false },
	HealthCurr{ 0 },
	m_pTarget{ nullptr },
	moving{ false },
	hit{ false }
{
}
/*****************************************************************************/
/*!
 \fn GameObjInst(AEVec2 pPos, bool enemy, GameObjInst* target);
 \brief implementing Constructor
 \param AEVec2 pPos
 the pos the object is on
 \param bool enemy
 indicating if its an enemy
 \param GameObjInst* target
 giving the object a target
*/
/*****************************************************************************/
GameObjInst::GameObjInst(AEVec2 pPos, bool _enemy, GameObjInst* target) :
	pObject{ nullptr },
	posCurr{ pPos },
	velCurr{ 0,0 },
	transform{ 0 },
	boundingBox{ 0 },
	AttackRange{ 0 },
	innerState{ INNER_STATE::INNER_STATE_ON_ENTER },
	timer{ 0.0f },
	lastBulletShotTime{ 0.f },
	enemy{ _enemy },
	Start_Attacking{ false },
	HealthCurr{ 0 },
	m_pTarget{ target },
	moving{ false },
	hit{ false }
{
}
/*****************************************************************************/
/*!
 \fn ~GameObjInst();
 \brief basic Destructor
*/
/*****************************************************************************/
GameObjInst::~GameObjInst()
{
}
/*****************************************************************************/
/*!
 \fn void Calculate_Position(float Mutiply_By);
 \brief Calculating the position in the game
 \param float Mutiply_By
 the deta time to find its position
*/
/*****************************************************************************/
void GameObjInst::Calculate_Position(float Mutiply_By)
{
	posCurr.x += (velCurr.x * Mutiply_By);
	posCurr.y += (velCurr.y * Mutiply_By);
}
/*****************************************************************************/
/*!
 \fn void Calculate_BoundingBox();
 \brief Calculating the BoundingBox for collision
*/
/*****************************************************************************/
void GameObjInst::Calculate_BoundingBox()
{
	if (pObject->type == TYPE_OBJECT::TYPE_OBJECT_TOWER)
	{
		boundingBox.min.x = pObject->Scale.x * -0.2f + posCurr.x;
		boundingBox.min.y = pObject->Scale.y * -0.5f + posCurr.y;
		boundingBox.max.x = pObject->Scale.x * 0.2f + posCurr.x;
		boundingBox.max.y = pObject->Scale.y * 0.5f + posCurr.y;
	}
	else
	{
		boundingBox.min.x = pObject->Scale.x * -0.5f + posCurr.x;
		boundingBox.min.y = pObject->Scale.y * -0.5f + posCurr.y;
		boundingBox.max.x = pObject->Scale.x * 0.5f + posCurr.x;
		boundingBox.max.y = pObject->Scale.y * 0.5f + posCurr.y;
	}
}
/*****************************************************************************/
/*!
 \fn bool Within_AttackRange(GameObjInst& rhs);
 \brief check if the enemy is inside the range of object
 \param GameObjInst& rhs
 the object to be checked against
*/
/*****************************************************************************/
bool GameObjInst::Within_AttackRange(GameObjInst& rhs)
{
	if (&rhs == &Global::GetInstance()->player)
	{
		if ((AEVec2Distance(&posCurr, &rhs.posCurr) - pObject->Scale.x / 2.0f -
			150.0f / 2.0f) < pObject->Attack_Range)
			return true;
		else
		{
			return false;
		}
	}
	else if (&rhs == &Global::GetInstance()->boss_lvll1)
	{
		if ((AEVec2Distance(&posCurr, &rhs.posCurr) - pObject->Scale.x / 2.0f -
			150.0f / 2.0f) < pObject->Attack_Range)
			return true;
		else
		{
			return false;
		}
	}
	
	if ((AEVec2Distance(&posCurr, &rhs.posCurr) - pObject->Scale.x / 2.0f -
		rhs.pObject->Scale.x / 2.0f) < pObject->Attack_Range)
	{
		if (rhs.pObject->type == TYPE_OBJECT::TYPE_OBJECT_TOWER)
		{
			if ((AEVec2Distance(&posCurr, &rhs.posCurr) - pObject->Scale.x / 2.0f -
				rhs.pObject->Scale.x / 5.0f) < pObject->Attack_Range)
				return true;

			return false;
		}
		else
			return true;
	}
	else
	{
		return false;
	}
}

GameObjInst* GameObjInst::GetClosestEnemy()
{
	GameObjInst* pClosestEnemy = nullptr;
	float			ClosestDistance = 10000.f;

	for (auto& Obj : getEnemyList())
	{
		if (Obj->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND)
		{
			// need to make sure its the closer one
			if (Within_AttackRange(*Obj) && Obj->HealthCurr > 0)
			{
				if (Obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_TOWER &&
					enemy)
					continue;
				float distance = AEVec2Distance(&posCurr, &Obj->posCurr) - pObject->Scale.x / 2.0f - Obj->pObject->Scale.x / 2.0f;

				if (distance < ClosestDistance)
				{
					ClosestDistance = distance;
					pClosestEnemy = Obj;
				}
			}
		}
	}
	if (enemy)
	{
		auto& player = Global::GetInstance()->player;
		if (Within_AttackRange(player))
		{
			float distance = AEVec2Distance(&posCurr, &player.posCurr) - pObject->Scale.x / 2.0f - 150.0f / 2.0f;

			if (distance < ClosestDistance)
			{
				if (player.posCurr.x > posCurr.x)
					return pClosestEnemy;

				ClosestDistance = distance;
				pClosestEnemy = &player;
			}
		}
	}
	else
	{
		auto& Boss = Global::GetInstance()->boss_lvll1;
		if (Within_AttackRange(Boss))
		{
			float distance = AEVec2Distance(&posCurr, &Boss.posCurr) - pObject->Scale.x / 2.0f - 150.0f / 2.0f;

			if (distance < ClosestDistance)
			{
				if (Boss.posCurr.x > posCurr.x)
					return pClosestEnemy;

				ClosestDistance = distance;
				pClosestEnemy = &Boss;
			}
		}
	}
	return pClosestEnemy;
}


/*****************************************************************************/
/*!
 \fn void Attack_Sequence();
 \brief the entire attacking sequence for the obj
*/
/*****************************************************************************/
void GameObjInst::Attack_Sequence()
{
	// AI Update
	switch (innerState)
	{
	case INNER_STATE::INNER_STATE_ON_ENTER:
	{
		if (HealthCurr > 0)
			innerState = INNER_STATE::INNER_STATE_ON_UPDATE;
	}
	break;
	case INNER_STATE::INNER_STATE_ON_UPDATE:
	{
		m_pTarget = GetClosestEnemy();

		velCurr.x = pObject->Movement_Speed;
		moving = true;
		Start_Attacking = false;

		if ((m_pTarget != nullptr) && (Within_AttackRange(*m_pTarget)))
			innerState = INNER_STATE::INNER_STATE_ATTACK;
	}
	break;
	case INNER_STATE::INNER_STATE_ATTACK:
	{
		m_pTarget = GetClosestEnemy();

		velCurr.x = 0.0f;
		moving = false;
		Start_Attacking = true;

		if (m_pTarget != nullptr)
		{
			if (m_pTarget->HealthCurr <= 0)
			{
				m_pTarget = nullptr;
				innerState = INNER_STATE::INNER_STATE_ON_UPDATE;
			}
		}
		else
			innerState = INNER_STATE::INNER_STATE_ON_UPDATE;

	}
	break;
	case INNER_STATE::INNER_STATE_IDOL:
		break;
	default:
		break;
	}
}
/*****************************************************************************/
/*!
 \fn void show_detail(s8 fontId);
 \brief for debuging
 \param s8 fontId
 the type of fornt used to show the debuging details
*/
/*****************************************************************************/
void GameObjInst::show_detail(s8 fontId)
{
	AEVec2 cameraPos = Global::GetInstance()->camera.axis;
	if (posCurr.x <= (cameraPos.x + WIN_MAX_WIDTH) &&
		posCurr.x >= (cameraPos.x - WIN_MAX_WIDTH))
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		char health[100];
		memset(health, 0, 100 * sizeof(char));
		sprintf_s(health, "Health:  %i", HealthCurr);
		AEGfxPrint(fontId, health, (posCurr.x - 20.0f - cameraPos.x) / WIN_MAX_WIDTH,
			(posCurr.y + pObject->Scale.y / 2 + 12.0f + 10.0f) / WIN_MAX_HEIGHT,
			1.0f, 1.f, 1.f, 1.f);

		char position[100];
		memset(position, 0, 100 * sizeof(char));
		sprintf_s(position, "position: x: %.1f  y: %.1f", posCurr.x, posCurr.y);
		AEGfxPrint(fontId, position, (posCurr.x - 20.0f - cameraPos.x) / WIN_MAX_WIDTH,
			(posCurr.y + pObject->Scale.y / 2 + 24.0f + 10.0f) / WIN_MAX_HEIGHT,
			1.0f, 1.f, 1.f, 1.f);

		char target[100];
		memset(target, 0, 100 * sizeof(char));
		if (m_pTarget == nullptr)
			sprintf_s(target, "No target");
		/*else
		{
			sprintf_s(target, "Lock On");

			char target_num[100];
			memset(target_num, 0, 100 * sizeof(char));
			sprintf_s(target_num, "object num:  %i", static_cast<int>(m_pTarget->pObject->type));
			AEGfxPrint(fontId, target_num, (posCurr.x - 20.0f - cameraPos.x) / WIN_MAX_WIDTH,
				(posCurr.y + pObject->Scale.y / 2 + 36.0f + 10.0f) / WIN_MAX_HEIGHT,
				1.0f, 1.f, 1.f, 1.f);

			char enemyIndecator[100];
			memset(enemyIndecator, 0, 100 * sizeof(char));
			sprintf_s(enemyIndecator, "enemy:  %i", static_cast<int>(m_pTarget->enemy));
			AEGfxPrint(fontId, enemyIndecator, (posCurr.x - 20.0f - cameraPos.x) / WIN_MAX_WIDTH,
				(posCurr.y + pObject->Scale.y / 2 + 48.0f + 10.0f) / WIN_MAX_HEIGHT, 1.0f, 1.f, 1.f, 1.f);


		}*/

		AEGfxPrint(fontId, target, (posCurr.x - 20.0f - cameraPos.x) / WIN_MAX_WIDTH,
			(posCurr.y + pObject->Scale.y / 2 + 10.0f) / WIN_MAX_HEIGHT, 1.0f, 1.f, 1.f, 1.f);
	}


}
/*****************************************************************************/
/*!
 \fn std::vector< GameObj >& getEnemyObj();
 \brief get the list of enemy object list
*/
/*****************************************************************************/
std::vector<GameObj>& GameObjInst::getEnemyObj()
{
	if (enemy)
		return Global::GetInstance()->Ally_Obj;
	else
		return Global::GetInstance()->Enemy_Obj;
}
/*****************************************************************************/
/*!
 \fn std::vector< GameObjInst* >& getEnemyList();
 \brief get the list of enemy object inst list
*/
/*****************************************************************************/
std::vector<GameObjInst*>& GameObjInst::getEnemyList()
{
	if (enemy)
		return Global::GetInstance()->Ally_List;
	else
		return Global::GetInstance()->Enemy_List;
}
/*****************************************************************************/
/*!
 \fn void CreateBullet();
 \brief when start attack is true create bullet
*/
/*****************************************************************************/
void GameObjInst::CreateBullet()
{
	if (Start_Attacking)
	{
		double currentTime = Global::GetInstance()->g_appTime;

		// I shoot a bullet every 2s
		if (currentTime - lastBulletShotTime < 2.0f)
			return;

		// Record last time we shot a bullet
		lastBulletShotTime = static_cast<float>(currentTime);

		if (enemy)
		{
			AEVec2 pos = Helper::Create_2d_Vector(posCurr.x - pObject->Scale.x / 2.0f, posCurr.y);
			Bullet temp(pos, enemy, m_pTarget, pos, pObject->Attack_Range, &pObject->Attack_Power);
			temp.pObject = &Global::GetInstance()->Enemy_Obj[static_cast<int>(TYPE_OBJECT::TYPE_OBJECT_BULLET)];
			temp.velCurr = Helper::Create_2d_Vector(temp.pObject->Movement_Speed, 0.0f);
			Global::GetInstance()->BulletList.push_back(temp);

		}
		else
		{
			AEVec2 pos = Helper::Create_2d_Vector(posCurr.x + pObject->Scale.x / 2.0f, posCurr.y);
			Bullet temp(pos, enemy, m_pTarget, pos, pObject->Attack_Range, &pObject->Attack_Power);
			temp.pObject = &Global::GetInstance()->Ally_Obj[static_cast<int>(TYPE_OBJECT::TYPE_OBJECT_BULLET)];
			temp.velCurr = Helper::Create_2d_Vector(temp.pObject->Movement_Speed, 0.0f);
			Global::GetInstance()->BulletList.push_back(temp);
		}
	}
}
/*****************************************************************************/
/*!
 \fn bool NeedNewEnemy()
 \brief checking if the object need an enemy
*/
/*****************************************************************************/
bool GameObjInst::NeedNewEnemy()
{
	if (m_pTarget == nullptr)
		return true;
	if (Within_AttackRange(*m_pTarget))
		return true;
	else
		return false;
}
/*****************************************************************************/
/*!
 \fn void AssignEnemy();
 \brief Assigning an enemy for the object
*/
/*****************************************************************************/
void GameObjInst::AssignEnemy()
{
	for (auto& Obj : getEnemyList())
	{
		if (Obj->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND)
		{
			// need to make sure its the closer one
			if (Within_AttackRange(*Obj) && Obj->HealthCurr > 0)
			{
				float temp_dis = AEVec2Distance(&posCurr, &Obj->posCurr) - pObject->Scale.x / 2.0f -
					Obj->pObject->Scale.x / 2.0f;
				if (m_pTarget != nullptr)
				{
					float target_dis = AEVec2Distance(&posCurr, &m_pTarget->posCurr) - pObject->Scale.x / 2.0f -
						m_pTarget->pObject->Scale.x / 2.0f;
					if (target_dis > temp_dis)
					{
						m_pTarget = Obj;
					}
				}
				else
					m_pTarget = Obj;
			}
		}
	}
}
/*****************************************************************************/
/*!
 \fn GameObj();
 \brief basic Constructor
*/
/*****************************************************************************/
GameObj::GameObj() :
	type{ TYPE_OBJECT::TYPE_OBJECT_NONE },
	pMesh{ nullptr },
	texture{ nullptr },
	texture2{ nullptr },
	texture3{ nullptr },
	texture_RIP{ nullptr },
	Health{ 0 },
	Attack_Power{ 0 },
	Attack_Range{ 0 },
	Movement_Speed{ 0 },
	Upgrade{ 0 },
	Scale{ 0.0f,0.0f },
	Enemy{ false },
	Cost{ 0 }
{
}
/*****************************************************************************/
/*!
 \fn GameObj();
 \brief implementing Constructor
 \param TYPE_OBJECT _type
 the type of object it is
 \param COLOUR _colour
 what colour you when to render for the object
 \param std::string texture_files
 the location and name of the texture files
 \param int _Health
 the standard health of the object
 \param int _Power
 the attack power of the object
 \param float _Attack_Range
 how far can the object attack can hit
 \param float _Movement_Speed
 how fast the object move
 \param AEVec2 _scale
 how big the object is
 \param bool _enemy
 to make sure they are facing each other
*/
/*****************************************************************************/
GameObj::GameObj(TYPE_OBJECT _type, COLOUR _colour,
	std::string texture_files, std::string texture_files2,
	std::string texture_files3, std::string texture_files_RIP,
	int _Health, int _Power,
	float _Attack_Range, float _Movement_Speed, AEVec2 _scale, bool _enemy, int _cost) :
	type{ _type },
	texture{ nullptr },
	texture2{ nullptr },
	texture3{ nullptr },
	texture_RIP{ nullptr },
	Health{ _Health },
	Attack_Power{ _Power },
	Attack_Range{ _Attack_Range },
	Movement_Speed{ _Movement_Speed },
	Upgrade{ 0 },
	Scale{ _scale },
	Enemy{ _enemy },
	Cost{ _cost }
{
	u32 colourNum = Helper::Get_Colour_Num(_colour);

	AEGfxMeshStart();
	if (!Enemy)
	{
		AEGfxTriAdd(
			-0.5f, -0.5f, colourNum, 0.0f, 1.0f,
			0.5f, -0.5f, colourNum, 1.0f, 1.0f,
			-0.5f, 0.5f, colourNum, 0.0f, 0.0f);

		AEGfxTriAdd(
			-0.5f, 0.5f, colourNum, 0.0f, 0.0f,
			0.5f, -0.5f, colourNum, 1.0f, 1.0f,
			0.5f, 0.5f, colourNum, 1.0f, 0.0f);

		pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pMesh, "fail to create object!!");
	}
	// making sure they face each other
	else
	{
		AEGfxTriAdd(
			0.5f, -0.5f, colourNum, 0.0f, 1.0f,
			-0.5f, -0.5f, colourNum, 1.0f, 1.0f,
			0.5f, 0.5f, colourNum, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.5f, 0.5f, colourNum, 0.0f, 0.0f,
			-0.5f, -0.5f, colourNum, 1.0f, 1.0f,
			-0.5f, 0.5f, colourNum, 1.0f, 0.0f);
		pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pMesh, "fail to create object!!");
	}
	// when the is a texture file
	if (!texture_files.empty())
	{
		std::string texture_png("Resources\\");
		std::string temp(texture_png);
		temp += texture_files;
		texture = AEGfxTextureLoad(temp.c_str());
		AE_ASSERT_MESG(texture, "Failed to create player texture!");
		if (!texture_files2.empty())
		{
			std::string temp2(texture_png);
			temp2 += texture_files2;
			texture2 = AEGfxTextureLoad(temp2.c_str());
			AE_ASSERT_MESG(texture2, "Failed to create player texture2!");

			std::string temp3(texture_png);
			temp3 += texture_files3;
			texture3 = AEGfxTextureLoad(temp3.c_str());
			AE_ASSERT_MESG(texture3, "Failed to create player texture3!");

			std::string temp_RIP(texture_png);
			temp_RIP += texture_files_RIP;
			texture_RIP = AEGfxTextureLoad(temp_RIP.c_str());
			AE_ASSERT_MESG(texture_RIP, "Failed to create player texture_RIP!");
		}
	}
}
/*****************************************************************************/
/*!
 \fn GameObj(const GameObj& rhs);
 \brief copy Constructor
 \param const GameObj& rhs
 the value to copy from
*/
/*****************************************************************************/
GameObj::GameObj(const GameObj& rhs) :
	type{ rhs.type },
	pMesh{ rhs.pMesh },
	texture{ rhs.texture },
	texture2{ rhs.texture2 },
	texture3{ rhs.texture3 },
	texture_RIP{ rhs.texture_RIP },
	Health{ rhs.Health },
	Attack_Power{ rhs.Attack_Power },
	Attack_Range{ rhs.Attack_Range },
	Movement_Speed{ rhs.Movement_Speed },
	Upgrade{ rhs.Upgrade },
	Scale{ rhs.Scale },
	Enemy{ rhs.Enemy },
	Cost{ rhs.Cost }
{
}
/*****************************************************************************/
/*!
 \fn GameObj& operator=(GameObj& rhs);
 \brief copy assignment
 \param GameObj& rhs
 the value to copy from
*/
/*****************************************************************************/
GameObj& GameObj::operator=(GameObj& rhs)
{
	if (this != &rhs)
	{
		type = rhs.type;
		pMesh = rhs.pMesh;
		texture = rhs.texture;
		texture2 = rhs.texture2;
		texture3 = rhs.texture3;
		texture_RIP = rhs.texture_RIP;
		Health = rhs.Health;
		Attack_Power = rhs.Attack_Power;
		Attack_Range = rhs.Attack_Range;
		Movement_Speed = rhs.Movement_Speed;
		Upgrade = rhs.Upgrade;
		Scale = rhs.Scale;
		Enemy = rhs.Enemy;
		Cost = rhs.Cost;
	}
	return *this;
}
GameObj::~GameObj()
{
}
/*****************************************************************************/
/*!
 \fn Bullet();
 \brief basic Constructor
*/
/*****************************************************************************/
Bullet::Bullet() :
	Start_Position{ 0.0f,0.0f },
	Range{ 0.0f },
	Power{ nullptr }
{
}
/*****************************************************************************/
/*!
 \fn Bullet(AEVec2 pPos, bool enemy, GameObjInst* target, AEVec2 _Start_Position, float _range, int* _power);
 \brief implementing Constructor
 \param AEVec2 pPos
 the pos the object is on
 \param bool enemy
 indicating if its an enemy
 \param GameObjInst* target
 giving the object a target
 \param AEVec2 _Start_Position
 geting the starting position to know where to end
 \param float _range
 the place it will end
 \param int* _power
 taking the attack power of the object
*/
/*****************************************************************************/
Bullet::Bullet(AEVec2 pPos, bool _enemy, GameObjInst* target, AEVec2 _Start_Position, float _range, int* _power) :
	Start_Position{ _Start_Position },
	Range{ _range },
	Power{ _power }
{
	enemy = _enemy;
	posCurr = pPos;
	m_pTarget = target;
}
/*****************************************************************************/
/*!
 \fn ~GameObjInst();
 \brief basic Destructor
*/
/*****************************************************************************/
Bullet::~Bullet()
{
}
/*****************************************************************************/
/*!
 \fn Miner();
 \brief basic Constructor
*/
/*****************************************************************************/
Miner::Miner() :
	stop_position{ 0.0f },
	inner_timer{ 0.0f }
{
}
/*****************************************************************************/
/*!
 \fn Miner(AEVec2 pos);
 \brief implementing Constructor
 \param AEVec2 Pos
*/
/*****************************************************************************/
Miner::Miner(AEVec2 pos) :
	stop_position{ pos.x + Global::GetInstance()->miner_stop + 100.0f },
	inner_timer{ 0.0f }
{
	posCurr = pos;
	Global::GetInstance()->miner_stop += 110.0f;
}
/*****************************************************************************/
/*!
 \fn void increase_birdseed();
 \brief increase birdseed when the timer is up
*/
/*****************************************************************************/
void Miner::increase_birdseed()
{
	Global::GetInstance()->BirdSeed += 30;
}
/*****************************************************************************/
/*!
 \fn Health_bar();
 \brief basic Constructor
*/
/*****************************************************************************/
Health_bar::Health_bar() :
	Host{ nullptr },
	Health{ nullptr },
	BackGround{ nullptr },
	Health_Pos{ Helper::Create_2d_Vector(0.0f,0.0f) },
	Health_Scale{ Helper::Create_2d_Vector(0.0f,0.0f) },
	Background_Pos{ Helper::Create_2d_Vector(0.0f,0.0f) },
	Background_Scale{ Helper::Create_2d_Vector(0.0f,0.0f) },
	Health_transform{ 0 },
	Background_transform{ 0 },
	Pre_Health{ 0 }
{
}
/*****************************************************************************/
/*!
 \fn Health_bar(GameObjInst* __Host, GameObj* _Health, GameObj* _Deduct_Health,
	AEVec2 _Pos_Health, AEVec2 _Pos_Deduct);
 \brief implementing Constructor
 \param GameObjInst* __Host
 the instance that is going on top of
*/
/*****************************************************************************/
Health_bar::Health_bar(GameObjInst* _Host) :
	Host{ _Host },
	Health{ Global::GetInstance()->Health_Bar_Mesh.Health },
	BackGround{ Global::GetInstance()->Health_Bar_Mesh.BackGround },
	Health_transform{ 0 },
	Background_transform{ 0 },
	Pre_Health{ 0 }
{
	if (Host == &Global::GetInstance()->boss_lvll1)
	{
		Health_Pos = Helper::Create_2d_Vector(Host->posCurr.x,
			Host->posCurr.y + 150.0f / 2.0f + 5.0f);
		Background_Pos = Health_Pos;
		Health_Scale = Helper::Create_2d_Vector(150.0f,
			150.0f / 20.0f);
		Background_Scale = Health_Scale;
	}
	else
	{
		Health_Pos = Helper::Create_2d_Vector(Host->posCurr.x,
			Host->posCurr.y + Host->pObject->Scale.y / 2.0f + 5.0f);
		Background_Pos = Health_Pos;
		Health_Scale = Helper::Create_2d_Vector(Host->pObject->Scale.x,
			Host->pObject->Scale.y / 20.0f);
		Background_Scale = Health_Scale;
	}
}
/*****************************************************************************/
/*!
 \fn ~Health_bar();
 \brief basic Destructor
*/
/*****************************************************************************/
Health_bar::~Health_bar()
{
}
/*****************************************************************************/
/*!
 \fn update();
 \brief update health bar position and scaling
*/
/*****************************************************************************/
void Health_bar::update()
{
	if (Host == nullptr)
		return;

	float health_pos_x, health_scale_x;

	if (Pre_Health != Host->HealthCurr)
	{
		if (Host->HealthCurr < 0)
		{
			Host = nullptr;
			return;
		}
		else
		{
			// health
			if (Host == &Global::GetInstance()->boss_lvll1)
			{
				health_scale_x = ((float)Host->HealthCurr / 1000) *
					Health_Scale.x;

				health_pos_x = Host->posCurr.x - (150.0f / 2.0f) +
					(health_scale_x / 2.0f);
			}
			else
			{
				health_scale_x = ((float)Host->HealthCurr / (float)Host->pObject->Health) * // TODO : problem here
					Health_Scale.x;

				health_pos_x = Host->posCurr.x - (Host->pObject->Scale.x / 2.0f) +
					(health_scale_x / 2.0f);

			}
		}
	}
	else
	{
		if (Host == &Global::GetInstance()->boss_lvll1)
		{
			health_scale_x = Health_Scale.x;

			health_pos_x = Host->posCurr.x - (150.0f / 2.0f) +
				(health_scale_x / 2.0f);
		}
		else
		{
			health_scale_x = Health_Scale.x;

			health_pos_x = Host->posCurr.x - (Host->pObject->Scale.x / 2.0f) +
				(health_scale_x / 2.0f);
		}


	}
	Health_Pos.x = health_pos_x;
	Health_Scale.x = health_scale_x;

	AEMtx33 scale, trans;
	AEMtx33Scale(&scale, Health_Scale.x, Health_Scale.y);
	AEMtx33Trans(&trans, Health_Pos.x, Health_Pos.y);
	AEMtx33Concat(&Health_transform, &trans, &scale);
	// background
	Background_Pos.x = Host->posCurr.x;

	AEMtx33 scale1, trans1;
	AEMtx33Scale(&scale1, Background_Scale.x, Background_Scale.y);
	AEMtx33Trans(&trans1, Background_Pos.x, Background_Pos.y);
	AEMtx33Concat(&Background_transform, &trans1, &scale1);
	Pre_Health = Host->HealthCurr;

}
/*****************************************************************************/
/*!
 \fn Explosion();
 \brief basic Constructor
*/
/*****************************************************************************/
Explosion::Explosion() :
	Host{ nullptr },
	Vertex{ nullptr },
	Texture{ nullptr },
	Pos{ nullptr },
	Scale{ 0 },
	Transform_Matrix{ 0 },
	Timer{ 0.0f }
{
}
/*****************************************************************************/
/*!
 \fn Explosion(GameObjInst* __Host);
 \brief implementing Constructor
 \param GameObjInst* __Host
 the instance that is going on top of
*/
/*****************************************************************************/
Explosion::Explosion(GameObjInst* Host) :
	Host{ Host },
	Pos{ &Host->posCurr },
	Scale{ 0 },
	Transform_Matrix{ 0 },
	Timer{ (float)Global::GetInstance()->g_appTime }
{
	if (Host == &Global::GetInstance()->player)
	{
		Scale = Helper::Create_2d_Vector(Global::GetInstance()->player.scale,
			Global::GetInstance()->player.scale);
	}
	else
	{
		Scale = Host->pObject->Scale;
	}
	if (Host->enemy)
	{
		Vertex = Global::GetInstance()->Expolosion_Mesh[0].Vertex;
		Texture = Global::GetInstance()->Expolosion_Mesh[0].Texture;
	}
	else
	{
		Vertex = Global::GetInstance()->Expolosion_Mesh[1].Vertex;
		Texture = Global::GetInstance()->Expolosion_Mesh[1].Texture;
	}
}
/*****************************************************************************/
/*!
 \fn void update();
 \brief update explosion matrix
*/
/*****************************************************************************/
void Explosion::update()
{
	if (Host != nullptr)
	{
		AEMtx33 scale, trans;
		AEMtx33Scale(&scale, Scale.x, Scale.y);
		AEMtx33Trans(&trans, Pos->x, Pos->y);
		AEMtx33Concat(&Transform_Matrix, &trans, &scale);
	}
}
/*****************************************************************************/
/*!
 \fn void draw();
 \brief draw explosion
*/
/*****************************************************************************/
void Explosion::draw()
{
	if (Host != nullptr)
	{
		if (Texture)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
			AEGfxTextureSet(Texture, 0.0f, 0.0f); // problem
			AEGfxSetTransparency(1.0f);
		}
		else
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}
		AEGfxSetTransform(Transform_Matrix.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(Vertex, AE_GFX_MDM_TRIANGLES);
	}
}
/*****************************************************************************/
/*!
 \fn Health_mesh();
 \brief basic Constructor
*/
/*****************************************************************************/
Health_mesh::Health_mesh() :
	Health{ nullptr },
	BackGround{ nullptr }
{
}
/*****************************************************************************/
/*!
 \fn Explosion_mesh();
 \brief basic Constructor
*/
/*****************************************************************************/
Explosion_mesh::Explosion_mesh() :
	Vertex{ nullptr },
	Texture{ nullptr }
{
}
/*****************************************************************************/
/*!
 \fn Explosion_mesh& operator=(Explosion_mesh& rhs);
 \brief copy assignment
 \param Explosion_mesh& rhs
 the value to copy from
*/
/*****************************************************************************/
Explosion_mesh& Explosion_mesh::operator=(Explosion_mesh& rhs)
{
	if (this != &rhs)
	{
		Vertex = rhs.Vertex;
		Texture = rhs.Texture;
	}
	return*this;
}
