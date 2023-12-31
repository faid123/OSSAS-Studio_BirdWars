/******************************************************************************
* \file			GameStateMgr.cpp
* \brief		Contain all the gamestate related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include "GameState/GameStateMgr.h"
#include "GameState/Level1/Level1.h"
#include "GameState/MainMenu/MainMenu.h"
#include "Victory/Victory.h"
#include "Defeat/Defeat.h"
#include "GameState/Credits/Credits.h"
#include "GameState/Instructions/Instructions.h"
#include "GameState/Options/Options.h"
#include "GameState/Precredits/Precredits.h"

// ---------------------------------------------------------------------------
// globals

// variables to keep track the current, previous and next game state
unsigned int	gGameStateInit;
unsigned int	gGameStateCurr;
unsigned int	gGameStatePrev;
unsigned int	gGameStateNext;

// pointer to functions for game state life cycles functions
void (*GameStateLoad)() = 0;
void (*GameStateInit)() = 0;
void (*GameStateUpdate)() = 0;
void (*GameStateDraw)() = 0;
void (*GameStateFree)() = 0;
void (*GameStateUnload)() = 0;

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateMgrInit(unsigned int gameStateInit)
{
	// set the initial game state
	gGameStateInit = gameStateInit;

	// reset the current, previoud and next game
	gGameStateCurr =
		gGameStatePrev =
		gGameStateNext = gGameStateInit;

	// call the update to set the function pointers
	GameStateMgrUpdate();
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateMgrUpdate()
{
	if ((gGameStateCurr == GS_QUIT))
		return;

	switch (gGameStateCurr)
	{
	case GS_LEVEL1:
		GameStateLoad =		GameState_Level1Load;
		GameStateInit =		GameState_Level1Init;
		GameStateUpdate =	GameState_Level1Update;
		GameStateDraw =		GameState_Level1Draw;
		GameStateFree =		GameState_Level1Free;
		GameStateUnload =	GameState_Level1Unload;
		break;

	case GS_MAINMENU:
		GameStateLoad = GameState_MainMenuLoad;
		GameStateInit = GameState_MainMenuInit;
		GameStateUpdate = GameState_MainMenuUpdate;
		GameStateDraw = GameState_MainMenuDraw;
		GameStateFree = GameState_MainMenuFree;
		GameStateUnload = GameState_MainMenuUnload;
		break;
	
	case GS_VICTORY:
		GameStateLoad = GameState_VictoryLoad;
		GameStateInit = GameState_VictoryInit;
		GameStateUpdate = GameState_VictoryUpdate;
		GameStateDraw = GameState_VictoryDraw;
		GameStateFree = GameState_VictoryFree;
		GameStateUnload = GameState_VictoryUnload;
		break;

	case GS_DEFEAT:
		GameStateLoad = GameState_DefeatLoad;
		GameStateInit = GameState_DefeatInit;
		GameStateUpdate = GameState_DefeatUpdate;
		GameStateDraw = GameState_DefeatDraw;
		GameStateFree = GameState_DefeatFree;
		GameStateUnload = GameState_DefeatUnload;
		break;

	case GS_OPTIONS:
		GameStateLoad = GameState_OptionsLoad;
		GameStateInit = GameState_OptionsInit;
		GameStateUpdate = GameState_OptionsUpdate;
		GameStateDraw = GameState_OptionsDraw;
		GameStateFree = GameState_OptionsFree;
		GameStateUnload = GameState_OptionsUnload;
		break;

	case GS_INSTRUCTIONS:
		GameStateLoad = GameState_InstructionsLoad;
		GameStateInit = GameState_InstructionsInit;
		GameStateUpdate = GameState_InstructionsUpdate;
		GameStateDraw = GameState_InstructionsDraw;
		GameStateFree = GameState_InstructionsFree;
		GameStateUnload = GameState_InstructionsUnload;
		break;

	case GS_PRECREDITS:
		GameStateLoad = GameState_PCLoad;
		GameStateInit = GameState_PCInit;
		GameStateUpdate = GameState_PCUpdate;
		GameStateDraw = GameState_PCDraw;
		GameStateFree = GameState_PCFree;
		GameStateUnload = GameState_PCUnload;
		break;

	case GS_CREDITS:
		GameStateLoad = GameState_CreditsLoad;
		GameStateInit = GameState_CreditsInit;
		GameStateUpdate = GameState_CreditsUpdate;
		GameStateDraw = GameState_CreditsDraw;
		GameStateFree = GameState_CreditsFree;
		GameStateUnload = GameState_CreditsUnload;
		break;

	case GS_RESTART:
		gGameStateNext = gGameStatePrev;
		break;

	default:
		AE_FATAL_ERROR("invalid state!!");
	}
}
