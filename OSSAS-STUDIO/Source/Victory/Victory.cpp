/******************************************************************************/
/*!
\file		Victory.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      To be used in GS_VICTORY only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Victory/Victory.h"
#include "AEEngine.h"
#include "Gamestate/GameStateMgr.h"
#include "Player/Player.h"
#include "GameState/Global.h"
#include "UI/tutorial.h"

// variable declarations to be used only for mainmenu state
extern AEGfxTexture* VictoryScreen;
extern AEGfxVertexList* Victory;
extern AEGfxTexture* VictoryRestart;
extern AEGfxVertexList* VictoryButton;
extern AEGfxTexture* VictoryBacktoMM;
// variable declarations to control the mainmenu button and user selection
static char selection = 0;
static float btn_x = 0.0f, btn_y = 0.0f;
s32 mouse_y7 = 0, mouse_x7 = 0;

void GameState_VictoryLoad()
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
	Victory = AEGfxMeshEnd();
	AE_ASSERT_MESG(Victory, "Failed to create mesh menu!!");

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
	VictoryButton = AEGfxMeshEnd();
	AE_ASSERT_MESG(VictoryButton, "Failed to create mesh menu!!");

	// Loading required textures to use later for meshing
	VictoryScreen = AEGfxTextureLoad("Resources\\VictoryDefeat\\Victory_wButtons.png");
	AE_ASSERT_MESG(VictoryScreen, "Failed to create mainMenu texture!");

	VictoryRestart = AEGfxTextureLoad("Resources\\VictoryDefeat\\restart_button.png");
	AE_ASSERT_MESG(VictoryRestart, "Failed to create Button_play_greyed texture!");

	VictoryBacktoMM = AEGfxTextureLoad("Resources\\VictoryDefeat\\menu_Button.png");
	AE_ASSERT_MESG(VictoryBacktoMM, "Failed to create utton_option_greyed texture!");
}

void GameState_VictoryInit()
{
	// initialize the button position
	// btn_x does not change position
	btn_y = 0;
	btn_x = 50;
}
void GameState_VictoryUpdate()
{
	fullscreenCheck();
	// Updates and inputs the player controls after winning or losing
	Victory_ProcessInput(selection, btn_x);
	if (AEInputCheckReleased(AEVK_RETURN) & 1 || AEInputCheckReleased(AEVK_LBUTTON))
	{
		switch (selection)
		{
		case 0:
			// changing gamestate 1 -  RESTART
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_LEVEL1;
			Global::GetInstance()->level_select =
				Global::GetInstance()->level_select_copy;
			break;
		case 1:
			// changing gamestate 2 -  PLAY
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_MAINMENU;
			break;
		default:
			break;
		}
	}
	AEInputGetCursorPosition(&mouse_x7, &mouse_y7);
	if ((mouse_x7 >= 172 && mouse_x7 <= 398) && (mouse_y7 >= 39 && mouse_y7 <= 166))
	{
		selection = 0;
	}
	else if ((mouse_x7 >= 412 && mouse_x7 <= 640) && (mouse_y7 >= 26 && mouse_y7 <= 151))
	{
		selection = 1;
	}
}
void GameState_VictoryDraw()
{
	MainMenu_Render(VictoryScreen, Victory);
	switch (selection)
	{
	case 0:
		victoryButton_Render(VictoryRestart, VictoryButton, btn_x, btn_y);
		break;
	case 1:
		victoryButton_Render(VictoryBacktoMM, VictoryButton, btn_x, btn_y);
		break;
	default:
		break;
	}
}

void GameState_VictoryFree()
{
	
}

void GameState_VictoryUnload()
{
	// free all meshes that are loaded in the game
	FreeVictory(Victory, VictoryButton);
	// unloads all textures in GS_VICTORY
	UnloadVictory(VictoryScreen, VictoryRestart, VictoryBacktoMM);
}

// ----- Functions that are used in Victory.cpp -----

void FreeVictory(AEGfxVertexList* Victorys, AEGfxVertexList* VictoryButtons)
{
	// Function that frees the various mashes in the game that were previously declared and initialized
	AEGfxMeshFree(Victorys);
	Victorys = nullptr;
	AEGfxMeshFree(VictoryButtons);
	VictoryButtons = nullptr;
}
#if 0
void FreeVictory()
{
	// Function that frees the various mashes in the game that were previously declared and initialized
	AEGfxMeshFree(Victory);
	Victory = nullptr;
	AEGfxMeshFree(button);
	button = nullptr;
}
#endif

void UnloadVictory(AEGfxTexture* VictoryScreens, AEGfxTexture* VictoryRestarts, AEGfxTexture* VictoryBacktoMMs)
{
	// Function that unloads the various textures in the game that were previously declared and initialized
	VictoryButton = nullptr;
	AEGfxTextureUnload(VictoryScreens);
	VictoryScreen = nullptr;
	AEGfxTextureUnload(VictoryRestarts);
	VictoryRestarts = nullptr;
	AEGfxTextureUnload(VictoryBacktoMMs);
	VictoryBacktoMMs = nullptr;
}

void Victory_ProcessInput(char& input, float& btnX)
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

	btnX = input > 0 ? 10 : btnX;
	btnX = input < 1 ? -10 : btnX;
	input = input > 1 ? 0 : input;
	input = input < 0 ? 1 : input;
}

void victoryButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, float& btnX, float& btnY)
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