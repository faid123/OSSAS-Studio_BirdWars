/******************************************************************************/
/*!
\file		Defeat.h
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
          100% code contribution
\date   	20 Jun, 2020
\brief      Header files for defeat.cpp.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "AEEngine.h"

void GameState_DefeatLoad();
void GameState_DefeatInit();
void GameState_DefeatUpdate();
void GameState_DefeatDraw();
void GameState_DefeatFree();
void GameState_DefeatUnload();
void FreeDefeat(AEGfxVertexList* Defeats, AEGfxVertexList* DefeatButtons);
void UnloadDefeat(AEGfxTexture* DefeatScreens, AEGfxTexture* DefeatRestarts, AEGfxTexture* DefeatBacktoMMs);
void Defeat_ProcessInput(char& input, float& btnX);
void defeatButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, float& btn_x, float& btn_y);
