/******************************************************************************
* \file			Helper.cpp
* \brief		Contain all the helper related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include "Helper/Helper.h"
#include "GameState/GameStateMgr.h"
#include "Audio/AudioManager.h"
#include "UI/tutorial.h"

#define GROUND_LEVEL -50.0f
#define WIN_MAX_WIDTH 400.0f
#define WIN_MAX_HEIGHT 300.0f
 /*****************************************************************************/
 /*!
  \fn static AEVec2 Create_2d_Vector(float x, float y);
  \brief creating a AEVec2 with 2 floats
  \param float x
  x position
  \param float y
  y position
 */
 /*****************************************************************************/
AEVec2 Helper::Create_2d_Vector(float x, float y)
{
	AEVec2 temp;
	temp.x = x;
	temp.y = y;
	return temp;
}
/*****************************************************************************/
/*!
 \fn static void Level1_Load();
 \brief loading all the things needed for the game
*/
/*****************************************************************************/
void Helper::Level1_Load()
{
	AssigningObj(Global::GetInstance()->Ally_Obj, false);
	AssigningObj(Global::GetInstance()->Enemy_Obj, true);
}
/*****************************************************************************/
/*!
 \fn static void Level1_Init();
 \brief init all the things needed for the game
*/
/*****************************************************************************/
void Helper::Level1_Init()
{
	// hero tower
	Helper::ObjCreater(Create_2d_Vector(-300.0f, 100.0f), false, nullptr,
		TYPE_OBJECT::TYPE_OBJECT_TOWER);
	Global::GetInstance()->pHero_Tower = Global::GetInstance()->Ally_List[0];
	// enemy tower
	Helper::ObjCreater(Create_2d_Vector(1950.0f, 100.0f), true, nullptr,
		TYPE_OBJECT::TYPE_OBJECT_TOWER);
	Global::GetInstance()->pEnemy_Tower = Global::GetInstance()->Enemy_List[0];
}
/*****************************************************************************/
/*!
 \fn static void Updating_GameObjInst(ObjectList& list);
 \brief updating the things needed for the game loop
 \param ObjectList& list
 the list that need to be updated
*/
/*****************************************************************************/
void Helper::Updating_GameObjInst(ObjectList& list)
{
	ObjectList* enemy = nullptr;
	if (list == Global::GetInstance()->Ally_List)
		enemy = &Global::GetInstance()->Enemy_List;
	else
		enemy = &Global::GetInstance()->Ally_List;

	// updating obj
	for (auto& obj : list)
	{
		if (obj->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND)
		{
			obj->Calculate_Position(Global::GetInstance()->g_dt);
			obj->Calculate_BoundingBox();
			if (obj->pObject->type != TYPE_OBJECT::TYPE_OBJECT_BULLET
				&& obj->HealthCurr > 0)
				obj->Attack_Sequence();
			obj->CreateBullet();
		}
		if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_MINER)
		{

			Miner* temp_miner = static_cast<Miner*>(obj);
			if (temp_miner->posCurr.x >= temp_miner->stop_position &&
				temp_miner->moving)
			{
				temp_miner->velCurr = Create_2d_Vector(0.0f, 0.0f);
				obj->moving = false;
			}
			if (!temp_miner->moving)
			{
				temp_miner->inner_timer += Global::GetInstance()->g_dt;
				if (temp_miner->inner_timer >= 3.0f)
				{
					temp_miner->increase_birdseed();
					temp_miner->inner_timer = 0.0f;
				}
			}

		}
	}
	// deleting things
	// C++ 98 with iterators
	for (auto it = list.begin(); it != list.end();)
	{
		auto* ptrObj = (*it);
		bool deleteMe = false;

		// losing condition
		if (ptrObj->enemy)
		{
			if (CollisionIntersection_RectRect(ptrObj->boundingBox, ptrObj->velCurr,
				Global::GetInstance()->pHero_Tower->boundingBox,
				Global::GetInstance()->pHero_Tower->velCurr))
			{
				gGameStateNext = GS_DEFEAT;
				return;
			}
		}

		// Bullets Collision Check
		auto& bullet_list = Global::GetInstance()->BulletList;
		for (auto iterator = bullet_list.begin(); iterator != Global::GetInstance()->BulletList.end();)
		{
			auto& bullet = (*iterator);
			if (CollisionIntersection_RectRect(ptrObj->boundingBox, ptrObj->velCurr,
				bullet.boundingBox, bullet.velCurr) &&
				bullet.m_pTarget == ptrObj)
			{
				ptrObj->HealthCurr -= *bullet.Power;

				iterator = bullet_list.erase(iterator);
				PlayAudio(SOUNDTYPE::ST_MONSTERDEATH);
				Global::GetInstance()->Explosion_list.push_back(Explosion(ptrObj));

			}
			else
				++iterator;
		}
		// health
		if (ptrObj->HealthCurr <= 0)
		{
			ptrObj->moving = false;

			for (auto& Health : Global::GetInstance()->Health_List)
				if (Health.Host == ptrObj) Health.Host = nullptr;

			for (auto& explosion : Global::GetInstance()->Explosion_list)
				if (explosion.Host == ptrObj) explosion.Host = nullptr;

			// to clear all the target aiming at a deleting obj
			for (auto& obj : *enemy)
			{
				if (obj->m_pTarget == ptrObj)
				{
					obj->m_pTarget = nullptr;
				}
				obj->velCurr = Create_2d_Vector(0.0f, 0.0f);
			}

			if (ptrObj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_TOWER)
			{
				if (ptrObj->enemy)
					Global::GetInstance()->pEnemy_Tower = nullptr;
				deleteMe = true;

			}
			else if (ptrObj->timer > 0.3f)
			{
				if (ptrObj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_MINER)
				{
					--Global::GetInstance()->minerInst;
					Global::GetInstance()->miner_stop -= 100.0f;
				}
				if (ptrObj->enemy)
					Global::GetInstance()->BirdSeed += 20;

				deleteMe = true;
			}
		}

		if (deleteMe)
		{
			it = list.erase(it);
			delete ptrObj;
		}
		else
			++it;
	}


	//Computing the transformation matrices of the game object instances
	for (auto& obj : list)
	{
		obj->posCurr.x = obj->posCurr.x + obj->velCurr.x;

		AEMtx33 scale, trans;

		AEMtx33Scale(&scale, obj->pObject->Scale.x, obj->pObject->Scale.y);
		AEMtx33Trans(&trans, obj->posCurr.x, obj->posCurr.y);
		AEMtx33Concat(&obj->transform, &trans, &scale);
	}
}
/*****************************************************************************/
/*!
 \fn static void Draw_GameObjInst(ObjectList& list);
 \brief Drawing the things needed for the game loop
 \param ObjectList& list
 the list that need to be rendered
*/
/*****************************************************************************/
void Helper::Draw_GameObjInst(ObjectList& list)
{
	for (auto& obj : list)
	{
		if (!Global::GetInstance()->IsPaused)
		{
			if (obj->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND &&
				obj->pObject->type != TYPE_OBJECT::TYPE_OBJECT_BULLET)
			{
				if (Global::GetInstance()->debug_details)
					obj->show_detail(Global::GetInstance()->fontId);
				if (obj->moving || obj->HealthCurr <= 0)
					obj->timer += Global::GetInstance()->g_dt;
				else
					obj->timer = 0.0f;

				if (obj->timer > 0.6f)
					obj->timer = 0;
			}

		}

		if (obj->pObject->texture != nullptr)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);


			AEGfxTexture* pCurrentTexture = obj->pObject->texture;

			if (obj->moving)
			{
				if ((obj->timer > 0.2f && obj->timer < 0.4f) &&
					(obj->pObject->texture2 != nullptr))
					pCurrentTexture = obj->pObject->texture2;

				if ((obj->timer > 0.4f) && (obj->pObject->texture3 != nullptr))
					pCurrentTexture = obj->pObject->texture3;

			}
			if (obj->HealthCurr <= 0 &&
				(obj->pObject->texture_RIP != nullptr))
				pCurrentTexture = obj->pObject->texture_RIP;

			AEGfxTextureSet(pCurrentTexture, 0.0f, 0.0f);
			AEGfxSetTransparency(1.0f);
		}
		else
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}
		AEGfxSetTransform(obj->transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(obj->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}
}
/*****************************************************************************/
/*!
 \fn static void Free_GameObjInst(ObjectList& list);
 \brief Unloading the things needed for the game loop
 \param ObjectList& list
 the list that need to be unloaded
*/
/*****************************************************************************/
void Helper::Free_GameObjInst(ObjectList& list)
{
	for (auto& obj : list)
		delete obj;
	list.clear();
}
/*****************************************************************************/
/*!
 \fn static void AssigningObj(Objects& list, bool enemy);
 \brief push back a new created GameObj into the GameObj list
 \param ObjectList& list
 the list that is going to be updated
 \param bool enemy
 to tell if its an enemey
*/
/*****************************************************************************/
void Helper::AssigningObj(Objects& list, bool enemy)
{
	auto& sGameObjList = list;
	int enemycounter = 1;
	if (enemy)
	{
		enemycounter = -1;
	}

	GameObj none(TYPE_OBJECT::TYPE_OBJECT_NONE, COLOUR::WHITE,
		"", "", "", "",
		0, 0, 0.0f, 0.0f, Create_2d_Vector(0.0f, 0.0f), enemy, 0);
	sGameObjList.push_back(none);

	GameObj tower(TYPE_OBJECT::TYPE_OBJECT_TOWER, COLOUR::BLUE,
		"Tower\\GameTower.png", "", "", "",
		1000, 0, 0.0f, 0.0f, Create_2d_Vector(300.0f, 300.0f), enemy, 0);
	sGameObjList.push_back(tower);

	sGameObjList.push_back(
		{ TYPE_OBJECT::TYPE_OBJECT_HERO, COLOUR::RED,
		"Player\\PlayerFrame1.png", "Player\\PlayerFrame2.png"
		, "Player\\PlayerFrame3.png", "Minions\\tombstone.PNG",
		500, 50, 25.0f, 0.0f * enemycounter, Create_2d_Vector(50.0f, 50.0f), enemy ,0 }
	);

	GameObj Minion(TYPE_OBJECT::TYPE_OBJECT_DUCK, COLOUR::GREEN,
		"Minions\\DuckMinionFrame1.png", "Minions\\DuckMinionFrame2.png"
		, "Minions\\DuckMinionFrame3.png", "Minions\\tombstone.PNG",
		100, 50, 70.0f, 1.0f * enemycounter, Create_2d_Vector(100.0f, 100.0f), enemy, 100);
	sGameObjList.push_back(Minion);

	GameObj Ground(TYPE_OBJECT::TYPE_OBJECT_GROUND, COLOUR::WHITE,
		"", "", "", "",
		1, 0, 0.0f, 0.0f, Create_2d_Vector(50.0f, 50.0f), enemy, 0);
	sGameObjList.push_back(Ground);

	GameObj Bullet(TYPE_OBJECT::TYPE_OBJECT_BULLET, COLOUR::WHITE,
		"fx\\feather.png", "", "", "",
		1, 0, 0.0f, 10.0f * enemycounter, Create_2d_Vector(50.0f, 50.0f), enemy, 0);
	sGameObjList.push_back(Bullet);

	GameObj OSTRICH(TYPE_OBJECT::TYPE_OBJECT_OSTRICH, COLOUR::WHITE,
		"Minions\\OstrichMinionFrame1.png", "Minions\\OstrichMinionFrame2.png"
		, "Minions\\OstrichMinionFrame3.png", "Minions\\tombstone.PNG",
		100, 20, 150.0f, 1.0f * enemycounter, Create_2d_Vector(125.0f, 125.0f), enemy, 200);
	sGameObjList.push_back(OSTRICH);

	GameObj PENGIUN(TYPE_OBJECT::TYPE_OBJECT_PENGIUN, COLOUR::WHITE,
		"Minions\\PenguinMinionFrame1.png", "Minions\\PenguinMinionFrame2.png"
		, "Minions\\PenguinMinionFrame3.png", "Minions\\tombstone.PNG",
		200, 30, 30.0f, 0.8f * enemycounter, Create_2d_Vector(100.0f, 100.0f), enemy, 150);
	sGameObjList.push_back(PENGIUN);

	GameObj MINER(TYPE_OBJECT::TYPE_OBJECT_MINER, COLOUR::WHITE,
		"Minions\\Miner.png", "", "", "",
		100, 0, 0.0f, 1.0f * enemycounter, Create_2d_Vector(100.0f, 100.0f), enemy, 50);
	sGameObjList.push_back(MINER);
}
/*****************************************************************************/
/*!
 \fn static void UnloadingObj(Objects& list);
 \brief clearing all the things in the obj
 \param ObjectList& list
 the list that need to be unloaded
*/
/*****************************************************************************/
void Helper::UnloadingObj(Objects& list)
{
	auto& sGameObjList = list;
	// free all CREATED mesh
	for (auto& obj : sGameObjList)
	{
		AEGfxMeshFree(obj.pMesh);
		if (obj.texture != nullptr)
			AEGfxTextureUnload(obj.texture);
		if (obj.texture2 != nullptr)
			AEGfxTextureUnload(obj.texture2);
		if (obj.texture3 != nullptr)
			AEGfxTextureUnload(obj.texture3);
		if (obj.texture_RIP != nullptr)
			AEGfxTextureUnload(obj.texture_RIP);
	}
	list.clear();
}
/*****************************************************************************/
/*!
 \fn static void ObjCreater(AEVec2 pPos, bool enemy, GameObjInst* target,
	TYPE_OBJECT Object);
 \brief push back a new created GameObjInst into the GameObjInst list
 \param AEVec2 pPos
 the pos the object is on
 \param bool enemy
 indicating if its an enemy
 \param GameObjInst* target
 giving the object a target
 \param TYPE_OBJECT Object
 what object it is
*/
/*****************************************************************************/
void Helper::ObjCreater(AEVec2 pPos, bool enemy, GameObjInst* target, TYPE_OBJECT Object)
{
	Objects* Obj = nullptr;
	ObjectList* ObjList = nullptr;
	if (enemy)
	{
		Obj = &Global::GetInstance()->Enemy_Obj;
		ObjList = &Global::GetInstance()->Enemy_List;
	}
	else
	{
		Obj = &Global::GetInstance()->Ally_Obj;
		ObjList = &Global::GetInstance()->Ally_List;
	}

	GameObjInst* Temp = new GameObjInst(pPos, enemy, target);

	Temp->pObject = &(*Obj)[static_cast<int>(Object)];
	Temp->HealthCurr = Temp->pObject->Health;
	(*ObjList).push_back(Temp);
	if (Object == TYPE_OBJECT::TYPE_OBJECT_DUCK ||
		Object == TYPE_OBJECT::TYPE_OBJECT_OSTRICH ||
		Object == TYPE_OBJECT::TYPE_OBJECT_PENGIUN ||
		Object == TYPE_OBJECT::TYPE_OBJECT_MINER)
		Global::GetInstance()->Health_List.push_back(Health_bar(Temp));
}
/*****************************************************************************/
/*!
 \fn static void updateBullet();
 \brief update the position and matrix
*/
/*****************************************************************************/
void Helper::Update_Bullet()
{
	auto& BulletList = Global::GetInstance()->BulletList;
	int index = 0;
	for (auto& bullet : BulletList)
	{
		if (AEVec2Distance(&bullet.posCurr, &bullet.Start_Position) >= bullet.Range)
		{
			BulletList.erase(BulletList.begin() + index);
			continue;
		}
		++index;
	}
	for (auto& bullet : BulletList)
	{
		bullet.Calculate_Position(Global::GetInstance()->g_dt);
		bullet.posCurr.x = bullet.posCurr.x + bullet.velCurr.x;
		bullet.Calculate_BoundingBox();

		AEMtx33 scale, trans;

		AEMtx33Scale(&scale, bullet.pObject->Scale.x, bullet.pObject->Scale.y);
		AEMtx33Trans(&trans, bullet.posCurr.x, bullet.posCurr.y);
		AEMtx33Concat(&bullet.transform, &trans, &scale);
	}

}
/*****************************************************************************/
/*!
 \fn static void updateBullet();
 \brief update the position
*/
/*****************************************************************************/
void Helper::Draw_Bullet()
{
	for (auto& bullet : Global::GetInstance()->BulletList)
	{
		if (bullet.pObject->texture)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
			AEGfxTextureSet(bullet.pObject->texture, 0.0f, 0.0f);
			AEGfxSetTransparency(1.0f);
		}
		else
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}
		AEGfxSetTransform(bullet.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(bullet.pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}
}
/*****************************************************************************/
/*!
 \fn static void ButtonInput();
 \brief geting the button input
 button used
 esc: escape the game
 q	: spawn duck
 w	: spawn OSTRICH
 e	: spawn PENGIUN
 r	: spawn MINER
 p	: pause game
 b	: show details
 F11: full screen
*/
/*****************************************************************************/
void Helper::Button_Input()
{
	/*auto& pHero_Tower = Global::GetInstance()->pHero_Tower;*/
	auto& pEnemy_Tower = Global::GetInstance()->pEnemy_Tower;
	auto& Birdseed = Global::GetInstance()->BirdSeed;
	auto& SpawnPos = Global::GetInstance()->Hero_Minon_Spawn_Pos;
	// pause
	if (AEInputCheckReleased(AEVK_ESCAPE) && Global::GetInstance()->tutorial_done)
	{
		Global::GetInstance()->isConfirm = false;
		Global::GetInstance()->isOption = false;
		Global::GetInstance()->IsPaused = !Global::GetInstance()->IsPaused;
	}
	fullscreenCheck();
	if (AEInputCheckReleased(AEVK_0))
		Global::GetInstance()->debug_details = !Global::GetInstance()->debug_details;

	if (Global::GetInstance()->IsPaused)
		return;
	//debuging
	if (Global::GetInstance()->player.HealthCurr > 0 && pEnemy_Tower != nullptr)
	{
		if ((AEInputCheckReleased(AEVK_Q) || Global::GetInstance()->mouse_spawn == 1) && Birdseed >= 100)
		{
			Helper::ObjCreater(Helper::Create_2d_Vector(SpawnPos.x,
				SpawnPos.y - 100.0f + Helper::get_Ally_Y_pos()),
				false, nullptr, TYPE_OBJECT::TYPE_OBJECT_DUCK);
			Birdseed -= 100;

			if ((Global::GetInstance()->minons_skills || Global::GetInstance()->Player_skills) &&
				!Global::GetInstance()->gameplay)
				Helper::ObjCreater(Helper::Create_2d_Vector(pEnemy_Tower->posCurr.x,
					pEnemy_Tower->posCurr.y - 100.0f + Helper::get_Enemy_Y_pos()),
					true, nullptr, TYPE_OBJECT::TYPE_OBJECT_DUCK);

			Sort_List();
		}
		if ((AEInputCheckReleased(AEVK_W) || Global::GetInstance()->mouse_spawn == 2) && Birdseed >= 200)
		{
			Helper::ObjCreater(Helper::Create_2d_Vector(SpawnPos.x,
				(GROUND_LEVEL + 125.0f / 2.0f) + Helper::get_Ally_Y_pos()),
				false, nullptr, TYPE_OBJECT::TYPE_OBJECT_OSTRICH);
			Birdseed -= 200;

			if ((Global::GetInstance()->minons_skills || Global::GetInstance()->Player_skills) &&
				!Global::GetInstance()->gameplay)
				Helper::ObjCreater(Helper::Create_2d_Vector(pEnemy_Tower->posCurr.x,
				(GROUND_LEVEL + 125.0f / 2.0f) + Helper::get_Enemy_Y_pos()),
					true, nullptr, TYPE_OBJECT::TYPE_OBJECT_OSTRICH);
			Sort_List();
		}
		if ((AEInputCheckReleased(AEVK_E) || Global::GetInstance()->mouse_spawn == 3) && Birdseed >= 150)
		{
			Helper::ObjCreater(Helper::Create_2d_Vector(SpawnPos.x,
				SpawnPos.y - 100.0f + Helper::get_Ally_Y_pos()),
				false, nullptr, TYPE_OBJECT::TYPE_OBJECT_PENGIUN);
			Birdseed -= 150;

			if ((Global::GetInstance()->minons_skills || Global::GetInstance()->Player_skills) &&
				!Global::GetInstance()->gameplay)
				Helper::ObjCreater(Helper::Create_2d_Vector(pEnemy_Tower->posCurr.x,
					pEnemy_Tower->posCurr.y - 100.0f + Helper::get_Enemy_Y_pos()),
					true, nullptr, TYPE_OBJECT::TYPE_OBJECT_PENGIUN);
			Sort_List();
		}

		if ((AEInputCheckReleased(AEVK_R) || Global::GetInstance()->mouse_spawn == 4) && Birdseed >= 50 &&
			(Global::GetInstance()->minerInst < Global::GetInstance()->miner_limit))
		{
			Create_miner();
			Birdseed -= 50;
			++Global::GetInstance()->minerInst;
		}
		Global::GetInstance()->mouse_spawn = 0;
	}

#ifdef _DEBUG
	// kill all minions
	if (AEInputCheckReleased(AEVK_9))
	{
		Delete_Health_list();
		Delete_Minions(Global::GetInstance()->Ally_List);
		Delete_Minions(Global::GetInstance()->Enemy_List);
	}
	if (AEInputCheckReleased(AEVK_6))
	{
		for (auto& obj : Global::GetInstance()->Enemy_List)
		{
			if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_DUCK
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_PENGIUN
				|| obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_OSTRICH)
			{
				obj->HealthCurr -= 500;
			}
		}
	}
#endif // DEBUG


}
/*****************************************************************************/
/*!
 \fn static void DeleteMinions(ObjectList& list);
 \brief deleting all minons type
 \parm	ObjectList& list
 list that is going to be delete from
*/
/*****************************************************************************/
void Helper::Delete_Minions(ObjectList& list)
{
	ObjectList* enemy = &Global::GetInstance()->Enemy_List;
	if (list == Global::GetInstance()->Enemy_List)
		enemy = &Global::GetInstance()->Ally_List;

	int counter = 0;
	for (auto& _GameObjInst : list)
	{
		if (_GameObjInst->pObject->type != TYPE_OBJECT::TYPE_OBJECT_TOWER &&
			_GameObjInst->pObject->type != TYPE_OBJECT::TYPE_OBJECT_HERO &&
			_GameObjInst->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND)
		{
			for (auto& obj : *enemy)
			{
				if (obj->m_pTarget == _GameObjInst)
					obj->m_pTarget = nullptr;
			}
			if (_GameObjInst->pObject->type == TYPE_OBJECT::TYPE_OBJECT_MINER)
			{
				--Global::GetInstance()->minerInst;
				Global::GetInstance()->miner_stop -= 50.0f;
			}
			delete list[counter];
			list.erase(list.begin() + counter);
			continue;
		}
		++counter;
	}
}
/*****************************************************************************/
/*!
 \fn static float get_Ally_Y_pos();
 \brief making sure that each ally minons has differents in level
*/
/*****************************************************************************/
float Helper::get_Ally_Y_pos()
{
	if (ally_y <= 40.0f)
		ally_y += 8.0f;
	else
		ally_y = 0;
	return ally_y;
}
/*****************************************************************************/
/*!
 \fn static float get_Enemy_Y_pos();
 \brief making sure that each enemy minons has differents in level
*/
/*****************************************************************************/
float Helper::get_Enemy_Y_pos()
{
	if (enemy_y <= 40.0f)
		enemy_y += 8.0f;
	else
		enemy_y = 0;
	return enemy_y;
}
/*****************************************************************************/
/*!
 \fn static void sort_list();
 \brief sorting out all minons base on height
*/
/*****************************************************************************/
void Helper::Sort_List()
{
	auto& Ally_List = Global::GetInstance()->Ally_List;
	auto& Enemy_List = Global::GetInstance()->Enemy_List;
	for (auto i = Ally_List.begin(); i != Ally_List.end(); ++i)
	{
		for (auto u = Ally_List.begin(); u != Ally_List.end(); ++u)
		{
			if ((*u)->posCurr.y < (*i)->posCurr.y &&
				(*u)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_TOWER &&
				(*u)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND &&
				(*i)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_TOWER &&
				(*i)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND)
			{
				std::iter_swap(i, u);
			}
		}
	}
	for (auto i = Enemy_List.begin(); i != Enemy_List.end(); ++i)
	{
		for (auto u = Enemy_List.begin(); u != Enemy_List.end(); ++u)
		{
			if ((*u)->posCurr.y < (*i)->posCurr.y &&
				(*u)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_TOWER &&
				(*u)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND &&
				(*i)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_TOWER &&
				(*i)->pObject->type != TYPE_OBJECT::TYPE_OBJECT_GROUND)
			{
				std::iter_swap(i, u);
			}
		}
	}
}
/*****************************************************************************/
/*!
 \fn static void create_miner();
 \brief addes miner to the list
*/
/*****************************************************************************/
void Helper::Create_miner()
{
	auto& SpawnPos = Global::GetInstance()->Hero_Minon_Spawn_Pos;
	Miner* temp = new Miner(
		Create_2d_Vector(SpawnPos.x, SpawnPos.y - 100.0f));
	temp->pObject =
		&Global::GetInstance()->Ally_Obj[static_cast<int>
		(TYPE_OBJECT::TYPE_OBJECT_MINER)];
	temp->HealthCurr = temp->pObject->Health;
	Global::GetInstance()->Ally_List.push_back(temp);
	Global::GetInstance()->Health_List.push_back(temp);
}
/*****************************************************************************/
/*!
 \fn static void Draw_Health_Bar();
 \brief Draw all health bars
*/
/*****************************************************************************/
void Helper::Draw_Health_Bar()
{
	for (auto& Health_bar : Global::GetInstance()->Health_List)
	{
		if (Health_bar.Host)
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxSetTransform(Health_bar.Background_transform.m);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxMeshDraw(Health_bar.BackGround, AE_GFX_MDM_TRIANGLES);

			AEGfxSetTransform(Health_bar.Health_transform.m);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxMeshDraw(Health_bar.Health, AE_GFX_MDM_TRIANGLES);
		}
	}
}
/*****************************************************************************/
/*!
 \fn static void Update_Health_Bar();
 \brief Update all health bars
*/
/*****************************************************************************/
void Helper::Update_Health_List()
{
	int counter = 0;
	auto& Health_list = Global::GetInstance()->Health_List;
	for (auto& Health_bar : Health_list)
	{
		Health_bar.update();
		if (Health_bar.Host == nullptr || Health_bar.Pre_Health <= 0)
		{
			Health_list.erase(Health_list.begin() + counter);
			continue;
		}
		++counter;
	}
}
/*****************************************************************************/
/*!
 \fn static u32 Get_Colour_Num(COLOUR _colour);
 \brief getting the colour number
*/
/*****************************************************************************/
u32 Helper::Get_Colour_Num(COLOUR _colour)
{
	switch (_colour)
	{
	case COLOUR::RED:
		return 0xFFFF0000;
	case COLOUR::BLUE:
		return 0xFF0000FF;
	case COLOUR::GREEN:
		return 0xFF00FF00;
	case COLOUR::WHITE:
		return 0xFFFFFFFF;
	case COLOUR::BLACK:
		return 0xFF000000;
	default:
		break;
	}
	return 0;
}
/*****************************************************************************/
/*!
 \fn static void Delete_Health_list();
 \brief delete all health bars
*/
/*****************************************************************************/
void Helper::Delete_Health_list()
{
	int counter = 0;
	auto& Health_list = Global::GetInstance()->Health_List;
	for (auto& Health : Health_list)
	{
		if (Health.Host == &Global::GetInstance()->boss_lvll1)
		{
			++counter;
			continue;
		}
		if (Health.Host->pObject->type != TYPE_OBJECT::TYPE_OBJECT_TOWER)
		{
			Health_list.erase(Health_list.begin() + counter);
			continue;
		}
		++counter;
	}
}
/*****************************************************************************/
/*!
 \fn static void Free_Health_list();
 \brief unloading all health bars
*/
/*****************************************************************************/
void Helper::Free_Health_list()
{
	Global::GetInstance()->Health_List.clear();
}
/*****************************************************************************/
/*!
 \fn static void Update_Explosions();
 \brief update all explosion
*/
/*****************************************************************************/
void Helper::Update_Explosions()
{
	auto& Explosion_list = Global::GetInstance()->Explosion_list;
	auto& Current_time = Global::GetInstance()->g_appTime;
	int counter = 0;
	for (auto& explosion : Explosion_list)
	{
		if (explosion.Host == nullptr ||
			(float)Current_time - explosion.Timer > 0.5f)
		{
			Explosion_list.erase(Explosion_list.begin() + counter);
			continue;
		}
		explosion.update();
		++counter;
	}
}
/*****************************************************************************/
/*!
 \fn static void Draw_Explosions();
 \brief Draw all explosion
*/
/*****************************************************************************/
void Helper::Draw_Explosions()
{
	for (auto& explosion : Global::GetInstance()->Explosion_list)
	{
		explosion.draw();
	}
}
/*****************************************************************************/
/*!
 \fn static void Unloading_Explosions();
 \brief Unloading all explosion
*/
/*****************************************************************************/
void Helper::Free_Explosions()
{
	Global::GetInstance()->Explosion_list.clear();
}
/*****************************************************************************/
/*!
 \fn static void Create_Health_Mesh();
 \brief creating the health mesh needed for the heslth bar
*/
/*****************************************************************************/
void Helper::Create_Health_Mesh()
{
	u32 colourNum = Helper::Get_Colour_Num(COLOUR::GREEN);

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, colourNum, 0.0f, 1.0f,
		0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		-0.5f, 0.5f, colourNum, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, colourNum, 0.0f, 0.0f,
		0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		0.5f, 0.5f, colourNum, 1.0f, 0.0f);

	Global::GetInstance()->Health_Bar_Mesh.Health = AEGfxMeshEnd();
	AE_ASSERT_MESG(Global::GetInstance()->Health_Bar_Mesh.Health,
		"fail to create Health_Mesh!!");

	colourNum = Helper::Get_Colour_Num(COLOUR::WHITE);
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, colourNum, 0.0f, 1.0f,
		0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		-0.5f, 0.5f, colourNum, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, colourNum, 0.0f, 0.0f,
		0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		0.5f, 0.5f, colourNum, 1.0f, 0.0f);

	Global::GetInstance()->Health_Bar_Mesh.BackGround = AEGfxMeshEnd();
	AE_ASSERT_MESG(Global::GetInstance()->Health_Bar_Mesh.BackGround,
		"fail to create Health_Mesh!!");
}
/*****************************************************************************/
/*!
 \fn static void Delete_Health_Mesh();
 \brief delete all mesh for health
*/
/*****************************************************************************/
void Helper::Delete_Health_Mesh()
{
	if (Global::GetInstance()->Health_Bar_Mesh.Health)
		AEGfxMeshFree(Global::GetInstance()->Health_Bar_Mesh.Health);
	if (Global::GetInstance()->Health_Bar_Mesh.BackGround)
		AEGfxMeshFree(Global::GetInstance()->Health_Bar_Mesh.BackGround);
}
/*****************************************************************************/
/*!
 \fn static void Create_Explosion();
 \brief create mesh and texture for explosion
*/
/*****************************************************************************/
void Helper::Create_Explosion()
{
	u32 colourNum = Helper::Get_Colour_Num(COLOUR::WHITE);
	Explosion_mesh temp, temp2;
	AEGfxMeshStart();
	// for left side // enemy 0
	AEGfxTriAdd(
		-0.5f, -0.5f, colourNum, 0.0f, 1.0f,
		0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		-0.5f, 0.5f, colourNum, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, colourNum, 0.0f, 0.0f,
		0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		0.5f, 0.5f, colourNum, 1.0f, 0.0f);

	temp.Vertex = AEGfxMeshEnd();
	AE_ASSERT_MESG(temp.Vertex, "fail to create object!!");
	temp.Texture = AEGfxTextureLoad("Resources\\fx\\explosion.png");
	AE_ASSERT_MESG(temp.Texture, "Failed to create Exposion texture!");
	Global::GetInstance()->Expolosion_Mesh.push_back(temp);

	// right side friendly// friend 1
	AEGfxMeshStart();
	AEGfxTriAdd(
		0.5f, -0.5f, colourNum, 0.0f, 1.0f,
		-0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		0.5f, 0.5f, colourNum, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, 0.5f, colourNum, 0.0f, 0.0f,
		-0.5f, -0.5f, colourNum, 1.0f, 1.0f,
		-0.5f, 0.5f, colourNum, 1.0f, 0.0f);
	temp2.Vertex = AEGfxMeshEnd();
	AE_ASSERT_MESG(temp2.Vertex, "fail to create object!!");

	temp2.Texture = AEGfxTextureLoad("Resources\\fx\\explosion.png");
	AE_ASSERT_MESG(temp2.Texture, "Failed to create Exposion texture!");
	Global::GetInstance()->Expolosion_Mesh.push_back(temp2);
}
/*****************************************************************************/
/*!
 \fn static void Delete_Explosion();
 \brief Delete mesh and texture for explosion
*/
/*****************************************************************************/
void Helper::Delete_Explosion()
{
	for (auto& explosion : Global::GetInstance()->Expolosion_Mesh)
	{
		if (explosion.Vertex)
			AEGfxMeshFree(explosion.Vertex);
		if (explosion.Texture)
			AEGfxTextureUnload(explosion.Texture);
	}
	Global::GetInstance()->Expolosion_Mesh.clear();
}
