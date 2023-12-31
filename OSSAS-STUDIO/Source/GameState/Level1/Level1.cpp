/******************************************************************************
* \file			Level1.cpp
* \brief		Contain all the level1 related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include "GameState/Level1/Level1.h"
#include "AEEngine.h"
#include "GameState/GameObj/GameObj.h"
#include "GameState/Global.h"
#include "GameState/GameStateMgr.h"
#include <string>
#include "UI/ui.h"
#include "Player/Player.h"
#include "Helper/Helper.h"
#include <iostream>
#include "GameState/Wallpaper/wallpaper.h"
#include "Boss/boss.h"
#include "GameState/Level1/GameFlow/Gameflow.h"
#include"Audio/AudioManager.h"
#include "UI/tutorial.h"

#define GROUND_LEVEL -50.0f

/*****Shaikh's variables*****/
Attack attack;
s32* attack_x = nullptr, * attack_y = nullptr;
s32 mouse_y, mouse_x;
/*****End of Shaikh's variables*****/
AEGfxTexture* VictoryScreen = nullptr;
AEGfxVertexList* Victory = nullptr;
AEGfxTexture* VictoryRestart = nullptr;
AEGfxVertexList* VictoryButton = nullptr;
AEGfxTexture* VictoryBacktoMM = nullptr;

AEGfxTexture* DefeatScreen = nullptr;
AEGfxVertexList* Defeat = nullptr;
AEGfxTexture* DefeatRestart = nullptr;
AEGfxVertexList* DefeatButton = nullptr;
AEGfxTexture* DefeatBacktoMM = nullptr;
// end of Ryan's variables


void GameState_Level1Load()
{
	if (Global::GetInstance()->level_select == 2)
	{
		Global::GetInstance()->IsPaused = false;
		Global::GetInstance()->tutorial_done = true;
	}

	if (Global::GetInstance()->tutorial_done)
	{
		Global::GetInstance()->IsPaused = false;
	}
	else
	{
		Global::GetInstance()->IsPaused = true;
	}
	// aps

	button_load();
	if (Global::GetInstance()->level_select != 2)
		tutorial_load();

	boss_load();
	LoadBullet();

	///*****START OF: Shaikh's code - create player mesh & texture*****/
	LoadHealth();
	LoadParticle();
	LoadAttack();
	LoadPlayer();
	/*****END OF: Shaikh's code - create player mesh & texture*****/

	// APS code start
	/*Global::GetInstance()->pHero_Tower = nullptr;*/
	Global::GetInstance()->pEnemy_Tower = nullptr;
	Helper::Level1_Load();
	Global::GetInstance()->miner_limit = 3;

	// delete later
	if (Global::GetInstance()->gameplay)
		GameFlow::Level1_GameFlow();

	Helper::Create_Health_Mesh();
	Helper::Create_Explosion();
	// APS code end

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	load_bg1();

	// end of Ryan's code
}

void GameState_Level1Init()
{
	Global::GetInstance()->boss_skills = false;
	Global::GetInstance()->miner_limit = 3;
	Global::GetInstance()->BirdSeed = 100;
	Global::GetInstance()->minerInst = 0;
	Global::GetInstance()->miner_stop = 100.0f;
	Global::GetInstance()->Hero_Minon_Spawn_Pos =
		Helper::Create_2d_Vector(-450.0f, 100.0f);
	GameFlow::touch = false;
	GameFlow::half_life = false;
	Global::GetInstance()->camera.axis = Helper::Create_2d_Vector(0.0f, 0.0f);

	// start of Ryan's code
	InitAudio();
	PlayAudio(SOUNDTYPE::ST_S1BACKGROUND);
	// end of Ryan's code

	// APS code start
	Helper::Level1_Init();
	/*Global::GetInstance()->IsPaused = false;*/
	// APS code end

	//Level fix
	Global::GetInstance()->level_select_copy = Global::GetInstance()->level_select;

	//load font at main.cpp
	/*****START OF: Shaikh's code - player values*****/
	AEGfxSetCamPosition(0.0f, 0.0f);
	InitPlayer(&attack);
	/*****END OF: Shaikh's code - player values*****/

	//syazwan
	boss_init();
	button_init();
	counter_init();

}

void GameState_Level1Update()
{
	// Mute check

	if (AEInputCheckCurr(AEVK_EQUAL))
	{
		Global::GetInstance()->volumeGlobal += 0.1f * Global::GetInstance()->g_dt;
		if (Global::GetInstance()->volumeGlobal >= 1)
		{
			Global::GetInstance()->volumeGlobal = 1;
		}
	}

	else if (AEInputCheckCurr(AEVK_MINUS))
	{
		Global::GetInstance()->volumeGlobal -= 0.1f * Global::GetInstance()->g_dt;
		if (Global::GetInstance()->volumeGlobal <= 0)
		{
			Global::GetInstance()->volumeGlobal = 0;
		}
	}
	// start of Ryan's code
	SetBGVolume(Global::GetInstance()->volumeGlobal);
	// end of Ryan's code

	Helper::Button_Input();
	if (Global::GetInstance()->IsPaused && Global::GetInstance()->tutorial_done)
	{
		update_pause();
	}
	if (Global::GetInstance()->IsPaused)
	{
		return;
	}

	if (IsIconic(AESysGetWindowHandle()) || !GetFocus())
	{
		Global::GetInstance()->IsPaused = true;
	}
	/*****START OF: Shaikh's code - player values*****/
	AEVec2 attackPos;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	attack_x = &mouse_x;
	attack_y = &mouse_y;
	AEVec2Set(&attackPos, (f32)*attack_x, (f32)*attack_y);
	MovePlayerNew();
	UpdatePlayer();
	PlayerAttack(attack, attackPos);
	UpdateAttack(&attack);
	UpdateUlt(&attack);

	HoverButton(*attack_x, *attack_y);
	/*****END OF: Shaikh's code - player values*****/

	// APS code start
	Helper::Updating_GameObjInst(Global::GetInstance()->Ally_List);
	Helper::Updating_GameObjInst(Global::GetInstance()->Enemy_List);

	if (Global::GetInstance()->BirdSeed > 1000)
	{
		Global::GetInstance()->BirdSeed = 1000; 
	}
	/*}*/


	if (Global::GetInstance()->pEnemy_Tower == nullptr ||
		Global::GetInstance()->pEnemy_Tower->HealthCurr <= 0)
	{
		gGameStateNext = GS_VICTORY;
		return;
	}

	if (Global::GetInstance()->pEnemy_Tower->HealthCurr <= 500 && Global::GetInstance()->level_select == 2)
	{
		//printf("bossSummon\n");
		Global::GetInstance()->boss_skills = true;
	}

	if (Global::GetInstance()->player.HealthCurr <= 0)
	{
		gGameStateNext = GS_DEFEAT;
		return;
	}

	Helper::Update_Bullet();

	Helper::Update_Health_List();
	Helper::Update_Explosions();
	GameFlow::GameFlow_update();
	// APS code end


	//Syazwan Boss
	//tutorial_update();

	// delete later

	if (Global::GetInstance()->boss_skills && Global::GetInstance()->level_select == 2)
	{
		//printf("Boss\n");
		update_boss();

		if (Global::GetInstance()->health_bar_boss == false)
		{
			Global::GetInstance()->Health_List.push_back(Health_bar(
				&Global::GetInstance()->boss_lvll1));
			Global::GetInstance()->health_bar_boss = true;
		}
	}

	debug_controls(); // syazwans debug controls for inf health and mana
}

void GameState_Level1Draw()
{
	if (Global::GetInstance()->pEnemy_Tower == nullptr ||
		Global::GetInstance()->player.HealthCurr <= 0)
	{
		return;
	}

	//Ryan's code
	draw_bg1();

	/*****START OF: Shaikh's code - player values*****/
	DrawPlayer();
	DrawAttack(&attack);
	DrawRegen(&attack);
	RenderUlt(&attack);
	/*****END OF: Shaikh's code - player values*****/
	if (Global::GetInstance()->boss_skills && Global::GetInstance()->level_select == 2)
		draw_boss();

	// APS code start
	Helper::Draw_GameObjInst(Global::GetInstance()->Ally_List);
	Helper::Draw_GameObjInst(Global::GetInstance()->Enemy_List);
	Helper::Draw_Bullet();
	Helper::Draw_Health_Bar();
	Helper::Draw_Explosions();
	// APS code end 

	// Start of syazwans cooler code
	// delete later
	draw_button();
	DrawButtonTint();
	//tutorial_draw();

	/*****START OF: Shaikh's code - player values*****/
	DrawHealth();
	/*****END OF: Shaikh's code - player values*****/

	if (!Global::GetInstance()->tutorial_done && Global::GetInstance()->level_select != 2)
		tutorial_update();

	if (Global::GetInstance()->IsPaused && Global::GetInstance()->tutorial_done)
	{
		draw_pause();
	}
}

void GameState_Level1Free()
{
	//free button - syazwan
	FreeCoolDown();
	// syazwans cool code
	FreeBossBullet();
	Global::GetInstance()->IsPaused = false;
	// delete later

	Helper::Free_Explosions();
	Helper::Free_Health_list();
	Helper::Free_GameObjInst(Global::GetInstance()->Ally_List);
	Helper::Free_GameObjInst(Global::GetInstance()->Enemy_List);
	GameFlow::Free_Events();
}

void GameState_Level1Unload()
{
	// APS code start
	Helper::UnloadingObj(Global::GetInstance()->Ally_Obj);
	Helper::UnloadingObj(Global::GetInstance()->Enemy_Obj);
	Helper::Delete_Health_Mesh();
	Helper::Delete_Explosion();
	// APS code end

	//syazwan
	free_button();
	unload_button();
	free_boss();
	// delete later
	unload_boss();
	UnloadBullet();
	if (Global::GetInstance()->level_select != 2)
		unload_tutorial();

	//Start of Ryan's code
	unload_bg1();
	FreeAudio();
	if (Global::GetInstance()->level_select != 2)
		free_tutorial();
	//End of Ryan's code

	///*****START OF: Shaikh's code unload*****/
	UnloadParticle();
	UnloadRegen();
	FreePlayer();
	FreeAttack();
	FreeRenderUlt();
	FreeHealth();
	///*****END OF: Shaikh's code unload*****/
}