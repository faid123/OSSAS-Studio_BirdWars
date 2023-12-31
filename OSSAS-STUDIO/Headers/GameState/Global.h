/******************************************************************************
\file		Global.h
\author 	Ang Pheng Siang , phengsiang.ang , 440004119
\par    	email: phengsiang.ang\@digipen.edu
\date   	June 16, 2020
\brief		Contain all the Global related function

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
/******************************************************************************/
#pragma once

#include "GameState/GameObj/GameObj.h"
#include "Player/Player.h"
#include "GameState/Level1/GameFlow/Gameflow.h"
#include "Boss/boss.h"

/*****************************************************************************/
/*!
 \fn class Global
 \brief thing that are shared amoung the others files
*/
/*****************************************************************************/
class Global
{
private:
	static Global* m_Instance;

	Global() :
		IsPaused{ false },
		g_dt{ 0.0f },
		g_appTime{ 0.0f },
		fontId{ 0 },
		Hero_Minon_Spawn_Pos{ 0 },
		pEnemy_Tower{ nullptr },
		miner_stop{ 100.0f },
		full_screen{ true },
		debug_details{ false },

		Player_skills{ true },
		minons_skills{ false },
		boss_skills{ false },
		gameplay{ true },


		camera{ -350,50,{0,0} },
		BirdSeed{ 100 },
		minerInst{ 0 },
		miner_limit{ 0 },
		vertex_Boss{ nullptr },
		texture_boss{ nullptr },
		vertex_Boss1{ nullptr },
		texture_boss1{ nullptr },
		vertex_Bullet{ nullptr },
		texture_Bullet{ nullptr },

		//Tutorial slides
		vertex_slide1{ nullptr },
		texture_slide1{ nullptr },

		//pause
		PauseMenuMesh{ nullptr },
		PauseMenuTexture{ nullptr },
		ResumeMesh{ nullptr },
		ResumeTexture{ nullptr },
		MainMenuMesh{ nullptr },
		MainMenuTexture{ nullptr },
		RestartMesh{ nullptr },
		RestartTexture{ nullptr },
		pauseCase{ 1 },

		tutorial_done{ false },
		health_bar_boss{ false },
		//level{ 1 },
		mouse_spawn{ 0 },
		fullscreen_check{ false },
		mute_check{ false },
		level_select{ 2 },
		isOption{ false },
		isConfirm{ false },

		//Audio
		volumeGlobal{ 0.5 },

		//Level FIX
		level_select_copy{ 0 }

	{}


public:

	// to access the data in global
	static Global* GetInstance()
	{
		if (m_Instance == nullptr)
			m_Instance = new Global();

		return m_Instance;
	}
	// deleting all the thing in it
	static void deleteInstance()
	{
		delete m_Instance;
	}

	// the entire created Ally list
	ObjectList	Ally_List;
	// the entire created Enemy list
	ObjectList	Enemy_List;
	// the entire created Ally objects
	Objects		Ally_Obj;
	// the entire created Enemy objects
	Objects		Enemy_Obj;
	// the entire bullet objects
	Bullets		BulletList;
	// pausing the game
	bool IsPaused;
	// deta time of the game
	float g_dt;
	// total time in game
	double g_appTime;
	// fornt of the text rendered
	s8 fontId;
	s8 fontId_large;
	// frequent use obj
	GameObjInst* pHero_Tower;
	AEVec2 Hero_Minon_Spawn_Pos;
	GameObjInst* pEnemy_Tower;
	// miner stop
	float miner_stop;
	bool full_screen;

	Health_mesh Health_Bar_Mesh;
	Health_Bars Health_List;
	bool debug_details;
	Explosion_meshs Expolosion_Mesh;
	Explosions Explosion_list;
	Events Event_list;
	// for alpha
	bool Player_skills;
	bool minons_skills;
	bool boss_skills;
	bool gameplay;

	/***Shaikh's code***/
	// the camera data struct
	Camera camera;
	// the player data struct
	Player player;
	// for quit confirmation check
	bool isQuit;
	char level_select;
	bool isLevelSelect;
	/***Shaikh's code***/

	//Syazwan
	Boss boss_lvll1;

	//Syazwan
	int BirdSeed;
	int minerInst;
	int miner_limit;
	int mouse_spawn;

	//UI
	AEGfxVertexList* vertex_Boss = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_boss; // Pointer to Texture (Image)
	AEGfxVertexList* vertex_Boss1 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_boss1; // Pointer to Texture (Image)

	//UI
	AEGfxVertexList* vertex_Bullet = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_Bullet; // Pointer to Texture (Image)

	//UI - Tutorial
	AEGfxVertexList* vertex_slide1 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slide1; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slide2 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slide2; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slide3 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slide3; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slide4 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slide4; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slide5 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slide5; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slide6 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slide6; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slide7 = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slide7; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slideRegen = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slideRegen; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slideBasic = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slideBasic; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_slideAdv = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_slideAdv; // Pointer to Texture (Image)

	AEGfxVertexList* vertex_bossTut = 0;  // mesh3 is a rectangle
	AEGfxTexture* texture_bossTut; // Pointer to Texture (Image)

	AEGfxVertexList* PauseMenuMesh;
	AEGfxTexture* PauseMenuTexture;

	AEGfxVertexList* ResumeMesh;
	AEGfxTexture* ResumeTexture;

	AEGfxVertexList* MainMenuMesh;
	AEGfxTexture* MainMenuTexture;

	AEGfxVertexList* RestartMesh;
	AEGfxTexture* RestartTexture;
	AEGfxTexture* restartGame;
	AEGfxTexture* option_pause;

	int pauseCase;
	bool fullscreen_check = false;
	bool mute_check = false;
	bool tutorial_done;
	bool health_bar_boss;
	bool isOption;
	bool isConfirm;
	//int level;

	//Audio
	float volumeGlobal;

	//Level select FIX
	char level_select_copy;
};
