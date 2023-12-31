/******************************************************************************/
/*!
\file		MainMenu.cpp
\author 	Shaikh Faid Bin Omar, shaikhfaid.binomar, 390003019
			100% Code Contribution
\par    	email: shaikhfaid.binomar@digipen.edu
\date   	June 13, 2020
\brief      To be used in GS_MAINMENU only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "GameState/MainMenu/MainMenu.h"
#include "AEEngine.h"
#include "Gamestate/GameStateMgr.h"
#include "Player/Player.h"
#include "GameState/Global.h"
#include"Audio/AudioManager.h"
#include "UI/tutorial.h"

// variable declarations to be used only for mainmenu state
AEGfxTexture* mainMenu = nullptr;
AEGfxVertexList* menu = nullptr;
AEGfxTexture* playButton = nullptr;
AEGfxVertexList* button = nullptr;
AEGfxTexture* optionsButton = nullptr;
AEGfxTexture* creditsButton = nullptr;
AEGfxTexture* instructionButton = nullptr;
AEGfxTexture* quitTex = nullptr;
AEGfxTexture* yesTex = nullptr;
AEGfxTexture* noTex = nullptr;
AEGfxTexture* levelSelect = nullptr;
AEGfxTexture* level1btn = nullptr;
AEGfxTexture* level2btn = nullptr;
AEGfxTexture* goback = nullptr;
AEGfxTexture* locked = nullptr;

// variable declarations to control the mainmenu button and user selection
static char selection = 0;
static float btn_x = 0.0f, btn_y = 0.0f;
static char quit_select = 0;
s32 x, y;

void GameState_MainMenuLoad()
{
	// Creating the objects (Shapes)
	// Informing the library that we're about to start adding triangles

	// start adding mesh for the main menu button use
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	// store the mesh of two triangles in button vertexlist
	button = AEGfxMeshEnd();
	AE_ASSERT_MESG(button, "Failed to create mesh button!!");

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
	menu = AEGfxMeshEnd();
	AE_ASSERT_MESG(menu, "Failed to create mesh menu!!");

	// load all the required textures to be used later with the meshes
	mainMenu = AEGfxTextureLoad("Resources\\MainMenu\\mainMenu.png");
	AE_ASSERT_MESG(mainMenu, "Failed to create mainMenu texture!");

	playButton = AEGfxTextureLoad("Resources\\MainMenu\\Button_play_greyed.png");
	AE_ASSERT_MESG(playButton, "Failed to create Button_play_greyed texture!");

	optionsButton = AEGfxTextureLoad("Resources\\MainMenu\\Button_option_greyed.png");
	AE_ASSERT_MESG(optionsButton, "Failed to create utton_option_greyed texture!");

	creditsButton = AEGfxTextureLoad("Resources\\MainMenu\\Button_credits_greyed.png");
	AE_ASSERT_MESG(creditsButton, "Failed to create Button_credits_greyed texture!");

	instructionButton = AEGfxTextureLoad("Resources\\MainMenu\\Button_instruction_greyed.png");
	AE_ASSERT_MESG(instructionButton, "Failed to create Button_credits_greyed texture!");

	quitTex = AEGfxTextureLoad("Resources\\MainMenu\\quit_confirmation.png");
	AE_ASSERT_MESG(quitTex, "Failed to create quit_confirmation texture!");
	yesTex = AEGfxTextureLoad("Resources\\MainMenu\\yes.png");
	AE_ASSERT_MESG(yesTex, "Failed to create yes button texture!");
	noTex = AEGfxTextureLoad("Resources\\MainMenu\\no.png");
	AE_ASSERT_MESG(noTex, "Failed to create no button texture!");

	levelSelect = AEGfxTextureLoad("Resources\\MainMenu\\levelselect.png");
	AE_ASSERT_MESG(levelSelect, "Failed to create levelselect texture!");
	level1btn = AEGfxTextureLoad("Resources\\MainMenu\\level1_button.png");
	AE_ASSERT_MESG(level1btn, "Failed to create level1_button button texture!");
	level2btn = AEGfxTextureLoad("Resources\\MainMenu\\level2_button.png");
	AE_ASSERT_MESG(level2btn, "Failed to create level2_button button texture!");
	goback = AEGfxTextureLoad("Resources\\MainMenu\\goback.png");
	AE_ASSERT_MESG(goback, "Failed to create goback button texture!");
	locked = AEGfxTextureLoad("Resources\\MainMenu\\locked.png");
	AE_ASSERT_MESG(locked, "Failed to create locked button texture!");
}

void GameState_MainMenuInit()
{
	// initialize the button position
	// btn_x does not change position
	selection = 0;
	quit_select = 0;
	Global::GetInstance()->level_select = 0;
	Global::GetInstance()->isLevelSelect = false; // for level select screen
	btn_y = 80.0f;
	btn_x = 0.0f;
	Global::GetInstance()->isQuit = false; // for quit confirmation
	
	// checking for audio settings
	if (!Global::GetInstance()->mute_check)
	{
		InitAudio();
		PlayAudio(SOUNDTYPE::ST_MMBACKGROUND);
	}
	else
	{
		SetBGVolume(Global::GetInstance()->volumeGlobal);
	}
}

void GameState_MainMenuUpdate()
{
	// set volume according to audio settings
	SetBGVolume(Global::GetInstance()->volumeGlobal);
	// checking for fullscreen enabled
	fullscreenCheck();
	
	// enter this scope if its not Quit Game screen or Level Selection screen
	if (!Global::GetInstance()->isQuit && !Global::GetInstance()->isLevelSelect)
	{
		// get the mouse cursor position in windows
		AEInputGetCursorPosition(&x, &y);
		// process the input of users by keyboard
		MainMenu_ProcessInput(selection, btn_y);
		// process the input of users by mouseclicks
		MainMenu_Mouse(selection, btn_y, x, y);
		// set isQuit to true if esc key is pressed, prompt a confirmation screen
		if (AEInputCheckReleased(AEVK_ESCAPE) & 1)
			Global::GetInstance()->isQuit = true;

		// enter this scope if the keyboard return key is pressed
		if (AEInputCheckReleased(AEVK_RETURN) & 1)
		{
			// check the selection where button is
			switch (selection)
			{
			case 0:
				// change to Level select screen, same state
				Global::GetInstance()->isLevelSelect = true;
				Global::GetInstance()->level_select = 1;
				break;
			case 1:
				// change the state -  OPTIONS
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_OPTIONS;
				break;
			case 2:
				// change the state - CREDITS
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_INSTRUCTIONS;
				break;
			case 3:
				// change the state - CREDITS
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_CREDITS;
				break;
			case 4:
				// change to quit game screen, same state
				Global::GetInstance()->isQuit = true;
			default:
				break;
			}
		}
	}
	// enter this scope if bool for isQuit screen is set to true
	else if(Global::GetInstance()->isQuit && !Global::GetInstance()->isLevelSelect)
	{
		// RETURN TO MAIN MENU
		if ((AEInputCheckReleased(AEVK_ESCAPE) & 1))
		{
			Global::GetInstance()->isQuit = false;
		}
		// quit confirmation logic
		AEInputGetCursorPosition(&x, &y);
		// checks mouse cursor
		if ((x >= 169 && x <= 372) && (y >= 336 && y <= 446))
		{
			quit_select = 1; // button selection to yes
			// QUIT the game
			if ((AEInputCheckReleased(AEVK_LBUTTON) & 1))
			{
				gGameStateNext = GS_QUIT;
			}
		}
		// checks mouse cursor
		else if ((x >= 417 && x <= 620) && (y >= 336 && y <= 446))
		{
			quit_select = 2; // button selection to no
			// RETURN TO MAIN MENU
			if ((AEInputCheckReleased(AEVK_LBUTTON) & 1))
			{
				Global::GetInstance()->isQuit = false;
			}
		}
		// use arrow right or left keys to change button selection
		if (AEInputCheckReleased(AEVK_RIGHT))
		{
			quit_select++; // increment quit selection
		}
		else if (AEInputCheckReleased(AEVK_LEFT))
		{
			quit_select--; // decrement quit selection
		}
		// check for keyboard return input and enter this scope
		if ((AEInputCheckReleased(AEVK_RETURN) & 1))
		{
			// quit on selection button yes
			if (quit_select == 1)
				gGameStateNext = GS_QUIT;
			// close the quit screen on selection button no
			else if (quit_select == 2)
				Global::GetInstance()->isQuit = false;
		}
		// limit the quit selection button between 1 and 2 only
		quit_select = quit_select > 2 ? 1 : quit_select;
		quit_select = quit_select < 1 ? 2 : quit_select;
	}
	// enter this scope if bool for Level Select screen is set to true
	else if (!Global::GetInstance()->isQuit && Global::GetInstance()->isLevelSelect)
	{
		// RETURN TO MAIN MENU
		if ((AEInputCheckReleased(AEVK_ESCAPE) & 1))
		{
			Global::GetInstance()->isLevelSelect = false;
		}
		// level select logic
		AEInputGetCursorPosition(&x, &y);
		// checks mouse cursor
		if ((x >= 304 && x <= 490) && (y >= 203 && y <= 306))
		{
			Global::GetInstance()->level_select = 1;
			// go to level1 of the game
			if ((AEInputCheckReleased(AEVK_LBUTTON) & 1) || AEInputCheckReleased(AEVK_RETURN))
			{
				// proceed to level 1 of the game if button selection is on LEVEL 1
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_LEVEL1;
				Global::GetInstance()->level_select = 1;
			}
		}
		// checks mouse cursor
		else if ((x >= 304 && x <= 490) && (y >= 344 && y <= 443))
		{
			Global::GetInstance()->level_select = 2;
			// go to level2 of the game
			if (((AEInputCheckReleased(AEVK_LBUTTON) & 1) || AEInputCheckReleased(AEVK_RETURN)) && Global::GetInstance()->tutorial_done)
			{
				// proceed to level 1 of the game if button selection is on LEVEL 2 (same state different gameflow)
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_LEVEL1;
				Global::GetInstance()->level_select = 2;
			}
		}
		// checks mouse cursor
		else if ((x >= 73 && x <= 223) && (y >= 479 && y <= 564))
		{
			// the back button
			Global::GetInstance()->level_select = 3;
			// return to the main menu
			if ((AEInputCheckReleased(AEVK_LBUTTON) & 1) || AEInputCheckReleased(AEVK_RETURN))
			{
				// return to main menu if the back button is selected
				Global::GetInstance()->isLevelSelect = false;
				Global::GetInstance()->level_select = 0;
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_MAINMENU;
			}
		}
		// update the level select value if keyboard down is pressed
		if (AEInputCheckReleased(AEVK_DOWN))
		{
			Global::GetInstance()->level_select = 2;
		}
		// update the level select value if keyboard up is pressed
		else if (AEInputCheckReleased(AEVK_UP))
		{
			Global::GetInstance()->level_select = 1;
		}
		// process the input if return key is pressed in level select
		if (AEInputCheckReleased(AEVK_RETURN))
		{
			// check the current level select value
			switch (Global::GetInstance()->level_select)
			{
			case 1:
				// proceed to level 1 if level select button is at LEVEL 1
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_LEVEL1;
				break;
			case 2:
				// proceed to level 1 (diff gameflow) if level select button is at LEVEL 2
				if (Global::GetInstance()->tutorial_done)
				{
					gGameStatePrev = gGameStateCurr;
					gGameStateNext = GS_LEVEL1;
				}
				break;
			default:
				break;
			}
		}
	}
}

void GameState_MainMenuDraw()
{
		// render the mainmenu meshes and textures
		float btny = 0;
		// render the main menu background texture
		MainMenu_Render(mainMenu, menu);
		switch (selection)
		{
		case 0:
			// render the START GAME button texture
			menuButton_Render(playButton, button, btn_x, btn_y, 0.25f, 0.18f);
			break;
		case 1:
			// render the OPTION button texture
			menuButton_Render(optionsButton, button, btn_x, btn_y, 0.22f, 0.15f);
			break;
		case 2:
			// render the INSTRUCTION button texture
			btny += btn_y + 10.0f;
			menuButton_Render(instructionButton, button, btn_x, btny, 0.20f, 0.14f);
			break;
		case 3:
			// render the CREDITS button texture
			btny += btn_y + 22.0f;
			menuButton_Render(creditsButton, button, btn_x, btny, 0.18f, 0.13f);
			break;
		case 4:
			// render the QUIT GAME mesh
			otherButton_Render(button, 350.0f, -270.0f, 0.15f, 0.08f);
		default:
			break;
		}

	// enter this scope if isQuit bool is set to true
	if (Global::GetInstance()->isQuit)
	{
		MainMenu_Render(quitTex, menu); // change to quit screen render
		if (quit_select == 1)
		{
			// render yes button
			MainMenu_Render(yesTex, button);
		}
		else if (quit_select == 2)
		{
			// render no button
			MainMenu_Render(noTex, button);
		}
	}
	// enter this scope if isLevelSelect bool is set to true
	if (Global::GetInstance()->isLevelSelect)
	{
		MainMenu_Render(levelSelect, menu); // change to level selection screen render
		if (Global::GetInstance()->level_select == 1)
		{
			// render level1 button
			MainMenu_Render(level1btn, button);
		}
		else if (Global::GetInstance()->level_select == 2)
		{
			// render level2 button
			MainMenu_Render(level2btn, button);
			if (!Global::GetInstance()->tutorial_done)
			{
				// Drawing mainMenu
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
				// Set position for object 1
				AEGfxSetPosition(0.0f, 0.0f);
				// set mainmenu texture
				AEGfxTextureSet(locked, 1.0f, 1.0f);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				// Drawing the mesh (list of triangles)
				AEMtx33 scale, trans, transform;
				AEMtx33Scale(&scale, (f32)AEGetWindowWidth() * 0.3f, (f32)AEGetWindowHeight() * 0.35f);
				AEMtx33Trans(&trans, 0.0f, -105.0f);
				AEMtx33Concat(&transform, &trans, &scale);
				AEGfxSetTransform(transform.m);
				// render menu image
				AEGfxMeshDraw(button, AE_GFX_MDM_TRIANGLES);

				// display message if tutorial is not completed at level 1 and mouse is hovered over
				char strBuffer09[100];
				memset(strBuffer09, 0, 100 * sizeof(char));
				sprintf_s(strBuffer09, "COMPLETE TUTORIAL AT LEVEL 1 TO UNLOCK!");

				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxPrint(Global::GetInstance()->fontId, strBuffer09, -0.65f, -0.1f, 2.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		else if (Global::GetInstance()->level_select == 3)
		{
			// render back button
			MainMenu_Render(goback, button);
		}
	}
}

void GameState_MainMenuFree()
{
	// free main menu audio
	FreeAudio();
}

void GameState_MainMenuUnload()
{
	// unload all textures and meshes in GS_MAINMENU
	AEGfxTextureUnload(quitTex);
	AEGfxTextureUnload(yesTex);
	AEGfxTextureUnload(noTex);
	AEGfxTextureUnload(levelSelect);
	AEGfxTextureUnload(level1btn);
	AEGfxTextureUnload(level2btn);
	AEGfxTextureUnload(goback);
	AEGfxTextureUnload(locked);
	UnloadMenu(mainMenu, playButton, optionsButton, creditsButton, instructionButton);
	FreeMenu(menu, button);
}