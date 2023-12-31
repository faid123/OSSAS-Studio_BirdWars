/******************************************************************************/
/*!
\file		Options.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
          100% code contribution
\date   	23 Jul, 2020
\brief      To be used in GS_OPTIONS only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "GameState/Options/Options.h"
#include "AEEngine.h"
#include "Gamestate/GameStateMgr.h"
#include "Player/Player.h"
#include "GameState/Global.h"
#include "Audio/AudioManager.h"
#include "UI/ui.h" //remove later
#include "UI/tutorial.h"

void GameState_OptionsLoad()
{
	load_options();
}


void GameState_OptionsInit()
{
	init_options();
}

void GameState_OptionsUpdate()
{
	update_options();
}

void GameState_OptionsDraw()
{
	draw_options();
}

void GameState_OptionsFree()
{
	
}

void GameState_OptionsUnload()
{
	unload_options();
}