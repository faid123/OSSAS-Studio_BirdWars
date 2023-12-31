/******************************************************************************
* \file			GameStateList.h
* \brief		Contain enum of state
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#ifndef CS230_GAME_STATE_LIST_H_
#define CS230_GAME_STATE_LIST_H_

// ---------------------------------------------------------------------------

// game state list

enum
{
	// list of all game states 
	GS_LEVEL1 = 0,
	GS_MAINMENU,
	GS_VICTORY,
	GS_DEFEAT,
	GS_OPTIONS,
	GS_INSTRUCTIONS,
	GS_PRECREDITS,
	GS_CREDITS,
	GS_CLEARED,
	GS_WIN,
	GS_LOSE,
	// special game state. Do not change
	GS_RESTART,
	GS_QUIT,
	GS_NUM
};

// ---------------------------------------------------------------------------

#endif // CS230_GAME_STATE_LIST_H_