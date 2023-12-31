/******************************************************************************/
/*!
\file		Credits.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      To be used in GS_CREDITS only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "GameState/Credits/Credits.h"
#include "AEEngine.h"
#include "Gamestate/GameStateMgr.h"
#include "Player/Player.h"
#include "GameState/Global.h"
#include "Audio/AudioManager.h"
#include "UI/tutorial.h" //remove later

// variable declarations to be used only for mainmenu state
AEGfxTexture* CreditsScreen;
AEGfxTexture* CreditsScreen2;
AEGfxVertexList* Credits;
AEGfxTexture* CreditsLeft;
AEGfxVertexList* CreditsButton;
AEGfxTexture* CreditsRight;
AEGfxTexture* CreditsLeft_Hover;
AEGfxTexture* CreditsRight_Hover;
// variable declarations to control the credits button and user selection
static char selection = 0;
static float btn_x = 0.0f, btn_y = 0.0f;

bool leftbtn_hover_credits;
bool leftbtn_hover_credits_left;

s32 mouse_y2 = 0, mouse_x2 = 0;

void GameState_CreditsLoad()
{
	// start adding mesh for the credits background
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
	Credits = AEGfxMeshEnd();
	AE_ASSERT_MESG(Credits, "Failed to create mesh menu!!");

	// start adding mesh for the Credits background
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
	CreditsButton = AEGfxMeshEnd();
	AE_ASSERT_MESG(CreditsButton, "Failed to create mesh menu!!");

	// Loading required textures to use later for meshing
	CreditsScreen = AEGfxTextureLoad("Resources\\Credits\\bird wars-last flight_creditspg1.png");
	AE_ASSERT_MESG(CreditsScreen, "Failed to create mainMenu texture!");

	CreditsScreen2 = AEGfxTextureLoad("Resources\\Credits\\bird wars-last flight_creditspg2.png");
	AE_ASSERT_MESG(CreditsScreen2, "Failed to create mainMenu texture!");

	CreditsLeft = AEGfxTextureLoad("Resources\\Credits\\leftarrow.png");
	AE_ASSERT_MESG(CreditsLeft, "Failed to create Button_play_greyed texture!");

	CreditsRight = AEGfxTextureLoad("Resources\\Credits\\rightarrow.png");
	AE_ASSERT_MESG(CreditsRight, "Failed to create utton_option_greyed texture!");

	CreditsLeft_Hover = AEGfxTextureLoad("Resources\\Credits\\leftarrow_hover.png");
	AE_ASSERT_MESG(CreditsLeft_Hover, "Failed to create Button_play_greyed texture!");

	CreditsRight_Hover = AEGfxTextureLoad("Resources\\Credits\\rightarrow_hover.png");
	AE_ASSERT_MESG(CreditsRight_Hover, "Failed to create utton_option_greyed texture!");
}

void GameState_CreditsInit()
{
	// initialize the button position
	// btn_x does not change position
	btn_y = -250.0f;
	btn_x = -300.0f;
	leftbtn_hover_credits = false;
	leftbtn_hover_credits_left = false;
	if (!Global::GetInstance()->mute_check)
	{
		InitAudio();
		PlayAudio(SOUNDTYPE::ST_MMBACKGROUND);
	}
	else
	{
		SetBGVolume(0.0f);
	}
}

void GameState_CreditsUpdate()
{
	SetBGVolume(Global::GetInstance()->volumeGlobal);

	fullscreenCheck();
	if (AEInputCheckReleased(AEVK_ESCAPE))
	{
		gGameStatePrev = gGameStateCurr;
		gGameStateNext = GS_MAINMENU;
	}
	// Updates and inputs the player controls after winning or losing
	Credits_ProcessInput(selection, btn_x);
	if (AEInputCheckReleased(AEVK_RETURN) & 1)
	{
		switch (selection)
		{
		case 0:
			// changing gamestate 1 -  PLAY
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_MAINMENU;
			break;
		case 1:
			// changing gamestate 2 -  RESTART
			gGameStatePrev = gGameStateCurr;
			gGameStateNext = GS_LEVEL1;
			break;
		default:
			break;
		}
	}

	if (selection == 0)
	{
		if ((mouse_x2 >= 123 && mouse_x2 <= 278) && (mouse_y2 >= 511 && mouse_y2 <= 562))
		{
			leftbtn_hover_credits = true;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				// change the state - page 2
				selection = 1;
			}
		}
		else if ((mouse_x2 >= 521 && mouse_x2 <= 678) && (mouse_y2 >= 511 && mouse_y2 <= 562))
		{
			leftbtn_hover_credits_left = true;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				// change the state - page 1
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_MAINMENU;
			}
		}
		else
		{
			leftbtn_hover_credits = false;
			leftbtn_hover_credits_left = false;
		}	
	}
	else if (selection == 1)
	{
		if ((mouse_x2 >= 521 && mouse_x2 <= 678) && (mouse_y2 >= 511 && mouse_y2 <= 562))
		{
			leftbtn_hover_credits = true;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				// change the state - page 1
				selection = 0;
			}
		}
		else
			leftbtn_hover_credits = false;
	}
	AEInputGetCursorPosition(&mouse_x2, &mouse_y2);
}
void GameState_CreditsDraw()
{
	float btn_x1 = 200.0f;
	switch (selection)
	{
	case 0:
		MainMenu_Render(CreditsScreen, Credits);
		creditsButton_Render(CreditsRight, CreditsButton, btn_x, btn_y);
		
		creditsButton_Render(CreditsLeft, CreditsButton, btn_x1, btn_y);

		if (leftbtn_hover_credits)
		{
			creditsButton_Render(CreditsRight_Hover, CreditsButton, btn_x, btn_y);
		}
		if (leftbtn_hover_credits_left)
		{
			creditsButton_Render(CreditsLeft_Hover, CreditsButton, btn_x1, btn_y);
		}
		break;

	case 1:
		MainMenu_Render(CreditsScreen2, Credits);
		creditsButton_Render(CreditsLeft, CreditsButton, btn_x, btn_y);

		if (leftbtn_hover_credits)
		{
			creditsButton_Render(CreditsLeft_Hover, CreditsButton, btn_x, btn_y);
		}
		break;

	default:
		break;
	}
	
}

void creditsButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, float& btnX, float& btnY)
{
	// Drawing menuButton
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	// Set position for object 1
	//AEGfxSetPosition(btnX, btnY);
	// set the button textures for menu button
	AEGfxTextureSet(menuButton, 1.0f, 1.0f);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// set transparency
	AEGfxSetTransparency(1.0f);
	// Drawing the mesh (list of triangles)
	AEMtx33 scale_btn, trans_btn, transform_btn;
	AEMtx33Scale(&scale_btn, (f32)AEGetWindowWidth() * 0.5f, (f32)AEGetWindowHeight() * 0.5f);
	AEMtx33Trans(&trans_btn, btnX, btnY);
	AEMtx33Concat(&transform_btn, &trans_btn, &scale_btn);
	AEGfxSetTransform(transform_btn.m);
	AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);
}

void GameState_CreditsFree()
{
	// free all meshes that are loaded in the game
	FreeCredits(Credits, CreditsButton);
}

void GameState_CreditsUnload()
{
	// unloads all textures in GS_VICTORY
	UnloadCredits(CreditsScreen, CreditsScreen2, CreditsLeft, CreditsRight, CreditsLeft_Hover, CreditsRight_Hover);
}

// ----- Functions that are used in Defeat.cpp -----
void FreeCredits(AEGfxVertexList* Credit, AEGfxVertexList* CreditButtons)
{
	// Function that frees the various mashes in the game that were previously declared and initialized
	AEGfxMeshFree(Credit);
	Credit = nullptr;
	AEGfxMeshFree(CreditButtons);
	CreditButtons = nullptr;
}

void UnloadCredits(AEGfxTexture* CreditScreen, AEGfxTexture* CreditScreens2, 
					AEGfxTexture* CreditLeft, AEGfxTexture* CreditRight,
					AEGfxTexture* CreditLeft_Hover, AEGfxTexture* CreditRight_Hover)
{
	// Function that unloads the various textures in the game that were previously declared and initialized
	AEGfxTextureUnload(CreditScreen);
	CreditScreen = nullptr;
	AEGfxTextureUnload(CreditScreens2);
	CreditScreens2 = nullptr;
	AEGfxTextureUnload(CreditLeft);
	CreditLeft = nullptr;
	AEGfxTextureUnload(CreditRight);
	CreditRight = nullptr;
	AEGfxTextureUnload(CreditLeft_Hover);
	CreditLeft_Hover = nullptr;
	AEGfxTextureUnload(CreditRight_Hover);
	CreditRight_Hover = nullptr;
}

void Credits_ProcessInput(char& input, float& btnX)
{
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
	if (selection >= 1)
		selection = 1;
	else if (selection <= 0)
		selection = 0;

	btnX = input > 0 ? 200 : btnX;
	btnX = input < 1 ? -200 : btnX;
	input = input > 1 ? 0 : input;
	input = input < 0 ? 1 : input;
}