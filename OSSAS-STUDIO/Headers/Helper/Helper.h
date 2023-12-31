/******************************************************************************
* \file			Helper.h
* \brief		Contain all the helper related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once
#include "GameState/Global.h"
#include "AEEngine.h"

/*****************************************************************************/
/*!
 \fn struct Helper
 \brief fuction helper struct
*/
/*****************************************************************************/
struct Helper
{
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
	static AEVec2 Create_2d_Vector(float x, float y);
	/*****************************************************************************/
	/*!
	 \fn static void Level1_Load();
	 \brief loading all the things needed for the game
	*/
	/*****************************************************************************/
	static void Level1_Load();
	/*****************************************************************************/
	/*!
	 \fn static void Level1_Init();
	 \brief init all the things needed for the game
	*/
	/*****************************************************************************/
	static void Level1_Init();
	/*****************************************************************************/
	/*!
	 \fn static void Updating_GameObjInst(ObjectList& list);
	 \brief updating the things needed for the game loop
	 \param ObjectList& list
	 the list that need to be updated
	*/
	/*****************************************************************************/
	static void Updating_GameObjInst(ObjectList& list);
	/*****************************************************************************/
	/*!
	 \fn static void Draw_GameObjInst(ObjectList& list);
	 \brief Drawing the things needed for the game loop
	 \param ObjectList& list
	 the list that need to be rendered
	*/
	/*****************************************************************************/
	static void Draw_GameObjInst(ObjectList& list);
	/*****************************************************************************/
	/*!
	 \fn static void Free_GameObjInst(ObjectList& list);
	 \brief Unloading the things needed for the game loop
	 \param ObjectList& list
	 the list that need to be unloaded
	*/
	/*****************************************************************************/
	static void Free_GameObjInst(ObjectList& list);
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
	static void AssigningObj(Objects& list, bool enemy);
	/*****************************************************************************/
	/*!
	 \fn static void UnloadingObj(Objects& list);
	 \brief clearing all the things in the obj
	 \param ObjectList& list
	 the list that need to be unloaded
	*/
	/*****************************************************************************/
	static void UnloadingObj(Objects& list);
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
	static void ObjCreater(AEVec2 pPos, bool enemy, GameObjInst* target,
		TYPE_OBJECT Object);
	/*****************************************************************************/
	/*!
	 \fn static void update_Bullet();
	 \brief update the position and matrix
	*/
	/*****************************************************************************/
	static void Update_Bullet();
	/*****************************************************************************/
	/*!
	 \fn static void updateBullet();
	 \brief update the position
	*/
	/*****************************************************************************/
	static void Draw_Bullet();
	/*****************************************************************************/
	/*!
	 \fn static void Button_Input();
	 \brief geting the button input
	 button used
	 esc: escape the game
	 q	: spawn duck
	 w	: spawn OSTRICH
	 e	: spawn PENGIUN
	 r	: spawn MINER
	 p	: pause game
	 b	: show detail
	 F11: full screen
	*/
	/*****************************************************************************/
	static void Button_Input();
	/*****************************************************************************/
	/*!
	 \fn static void DeleteMinions(ObjectList& list);
	 \brief deleting all minons type
	 \parm	ObjectList& list
	 list that is going to be delete from
	*/
	/*****************************************************************************/
	static void Delete_Minions(ObjectList& list);
	// static varable
	static float ally_y;
	static float enemy_y;
	/*****************************************************************************/
	/*!
	 \fn static float get_Ally_Y_pos();
	 \brief making sure that each ally minons has differents in level
	*/
	/*****************************************************************************/
	static float get_Ally_Y_pos();
	/*****************************************************************************/
	/*!
	 \fn static float get_Enemy_Y_pos();
	 \brief making sure that each enemy minons has differents in level
	*/
	/*****************************************************************************/
	static float get_Enemy_Y_pos();
	/*****************************************************************************/
	/*!
	 \fn static void sort_list();
	 \brief sorting out all minons base on height
	*/
	/*****************************************************************************/
	static void Sort_List();
	/*****************************************************************************/
	/*!
	 \fn static void create_miner();
	 \brief addes miner to the list
	*/
	/*****************************************************************************/
	static void Create_miner();
	/*****************************************************************************/
	/*!
	 \fn static void Draw_Health_Bar();
	 \brief Draw all health bars
	*/
	/*****************************************************************************/
	static void Draw_Health_Bar();
	/*****************************************************************************/
	/*!
	 \fn static void Update_Health_Bar();
	 \brief Update all health bars
	*/
	/*****************************************************************************/
	static void Update_Health_List();
	/*****************************************************************************/
	/*!
	 \fn static u32 Get_Colour_Num(COLOUR _colour);
	 \brief getting the colour number
	*/
	/*****************************************************************************/
	static u32 Get_Colour_Num(COLOUR _colour);
	/*****************************************************************************/
	/*!
	 \fn static void Delete_Health_list();
	 \brief delete all health bars
	*/
	/*****************************************************************************/
	static void Delete_Health_list();
	/*****************************************************************************/
	/*!
	 \fn static void Free_Health_list();
	 \brief unloading all health bars
	*/
	/*****************************************************************************/
	static void Free_Health_list();
	/*****************************************************************************/
	/*!
	 \fn static void Update_Explosions();
	 \brief update all explosion
	*/
	/*****************************************************************************/
	static void Update_Explosions();
	/*****************************************************************************/
	/*!
	 \fn static void Draw_Explosions();
	 \brief Draw all explosion
	*/
	/*****************************************************************************/
	static void Draw_Explosions();
	/*****************************************************************************/
	/*!
	 \fn static void Free_Explosions();
	 \brief Unloading all explosion
	*/
	/*****************************************************************************/
	static void Free_Explosions();
	/*****************************************************************************/
	/*!
	 \fn static void Create_Health_Mesh();
	 \brief creating the health mesh needed for the heslth bar
	*/
	/*****************************************************************************/
	static void Create_Health_Mesh();
	/*****************************************************************************/
	/*!
	 \fn static void Delete_Health_Mesh();
	 \brief delete all mesh for health
	*/
	/*****************************************************************************/
	static void Delete_Health_Mesh();
	/*****************************************************************************/
	/*!
	 \fn static void Create_Explosion();
	 \brief create mesh and texture for explosion
	*/
	/*****************************************************************************/
	static void Create_Explosion();
	/*****************************************************************************/
	/*!
	 \fn static void Delete_Explosion();
	 \brief Delete mesh and texture for explosion
	*/
	/*****************************************************************************/
	static void Delete_Explosion();
};