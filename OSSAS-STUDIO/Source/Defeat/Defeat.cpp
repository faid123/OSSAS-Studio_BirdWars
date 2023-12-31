/******************************************************************************/
/*!
\file		Defeat.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      To be used in GS_DEFEAT only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Defeat/Defeat.h"
#include "AEEngine.h"
#include "Gamestate/GameStateMgr.h"
#include "Player/Player.h"
#include "GameState/Global.h"
#include "UI/tutorial.h"

// variable declarations to be used only for mainmenu state
extern AEGfxTexture* DefeatScreen;
extern AEGfxVertexList* Defeat;
extern AEGfxTexture* DefeatRestart;
extern AEGfxVertexList* DefeatButton;
extern AEGfxTexture* DefeatBacktoMM;

// variable declarations to control the mainmenu button and user selection
static char selection = 0;
static float btn_x = 0.0f, btn_y = 0.0f;
s32 mouse_y6 = 0, mouse_x6 = 0;

void GameState_DefeatLoad()
{
	// start adding mesh for the main menu background
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	// store the mesh of two triangles in menu vertexlist
	Defeat = AEGfxMeshEnd();
	AE_ASSERT_MESG(Defeat, "Failed to create mesh menu!!");

	// start adding mesh for the main menu background
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	// Storing mesh of triangles in list
	DefeatButton = AEGfxMeshEnd();
	AE_ASSERT_MESG(DefeatButton, "Failed to create mesh menu!!");

	// Loading required textures to use later for meshing
	DefeatScreen = AEGfxTextureLoad("Resources\\VictoryDefeat\\fail.jpg");
	AE_ASSERT_MESG(DefeatScreen, "Failed to create mainMenu texture!");

	DefeatRestart = AEGfxTextureLoad("Resources\\VictoryDefeat\\restartFail.png");
	AE_ASSERT_MESG(DefeatRestart, "Failed to create Button_play_greyed texture!");

	DefeatBacktoMM = AEGfxTextureLoad("Resources\\VictoryDefeat\\failMenu.png");
	AE_ASSERT_MESG(DefeatBacktoMM, "Failed to create utton_option_greyed texture!");
}

void GameState_DefeatInit()
{
	// initialize the button position
	// btn_x does not change position
	btn_y = 0;
	btn_x = 50;
}

void GameState_DefeatUpdate()
{
	fullscreenCheck();
	// Updates and inputs the player controls after winning or losing
	Defeat_ProcessInput(selection, btn_x);
	if (AEInputCheckReleased(AEVK_RETURN) & 1 || AEInputCheckReleased(AEVK_LBUTTON))
	{
		switch (selection)
		{
		case 0:
			// changing gamestate 1 -  PLAY
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_LEVEL1;
			Global::GetInstance()->level_select = 
				Global::GetInstance()->level_select_copy;
			break;
		case 1:
			// changing gamestate 2 -  RESTART
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_MAINMENU;
			break;
		default:
			break;
		}
	}
	AEInputGetCursorPosition(&mouse_x6, &mouse_y6);
	if ((mouse_x6 >= 100 && mouse_x6 <= 291) && (mouse_y6 >= 81 && mouse_y6 <= 188))
	{
		selection = 0;
	}
	else if ((mouse_x6 >= 426 && mouse_x6 <= 752) && (mouse_y6 >= 78 && mouse_y6 <= 201))
	{
		selection = 1;
	}
}
void GameState_DefeatDraw()
{
	MainMenu_Render(DefeatScreen, Defeat);
	switch (selection)
	{
	case 0:
		defeatButton_Render(DefeatRestart, DefeatButton, btn_x, btn_y);
		break;
	case 1:
		defeatButton_Render(DefeatBacktoMM, DefeatButton, btn_x, btn_y);
		break;
	default:
		break;
	}
}

void defeatButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, float& btnX, float& btnY)
{
	// Drawing menuButton
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	// Set position for object 1
	AEGfxSetPosition(btnX, btnY);
	// set the button textures for menu button
	AEGfxTextureSet(menuButton, 1.0f, 1.0f);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// set transparency
	AEGfxSetTransparency(1.0f);
	// Drawing the mesh (list of triangles)
	AEMtx33 scale_btn, trans_btn, transform_btn;
	AEMtx33Scale(&scale_btn, (f32)AEGetWindowWidth() * 1.0f, (f32)AEGetWindowHeight() * 1.0f);
	AEMtx33Trans(&trans_btn, btnX, btnY);
	AEMtx33Concat(&transform_btn, &trans_btn, &scale_btn);
	AEGfxSetTransform(transform_btn.m);
	AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);
}

void GameState_DefeatFree()
{
	
}

void GameState_DefeatUnload()
{
	// free all meshes that are loaded in the game
	FreeDefeat(Defeat, DefeatButton);
	// unloads all textures in GS_VICTORY
	UnloadDefeat(DefeatScreen, DefeatRestart, DefeatBacktoMM);
}

// ----- Functions that are used in Defeat.cpp -----
void FreeDefeat(AEGfxVertexList* Defeats, AEGfxVertexList* DefeatButtons)
{
	// Function that frees the various mashes in the game that were previously declared and initialized
	AEGfxMeshFree(Defeats);
	Defeats = nullptr;
	AEGfxMeshFree(DefeatButtons);
	DefeatButtons = nullptr;
}

void UnloadDefeat(AEGfxTexture* DefeatScreens, AEGfxTexture* DefeatRestarts, AEGfxTexture* DefeatBacktoMMs)
{
	// Function that unloads the various textures in the game that were previously declared and initialized
	AEGfxTextureUnload(DefeatScreens);
	DefeatScreens = nullptr;
	AEGfxTextureUnload(DefeatRestarts);
	DefeatRestarts = nullptr;
	AEGfxTextureUnload(DefeatBacktoMMs);
	DefeatBacktoMMs = nullptr;
}

void Defeat_ProcessInput(char& input, float& btnX)
{
	if (AEInputCheckCurr(AEVK_ESCAPE) & 1)
		gGameStateNext = GS_QUIT;
	if (AEInputCheckReleased(AEVK_LEFT))
	{
		selection--;
		btnX -= 50;
	}
	if (AEInputCheckReleased(AEVK_RIGHT))
	{
		selection++;
		btnX += 10;
	}

	btnX = input > 0 ? 0 : btnX;
	btnX = input < 1 ? 0 : btnX;
	input = input > 1 ? 0 : input;
	input = input < 0 ? 1 : input;
}