/******************************************************************************/
/*!
\file		Victory.h
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      header files for victory cpp

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "AEEngine.h"

void GameState_VictoryLoad();
void GameState_VictoryInit();
void GameState_VictoryUpdate();
void GameState_VictoryDraw();
void GameState_VictoryFree();
void GameState_VictoryUnload();
void FreeVictory(AEGfxVertexList* Victorys, AEGfxVertexList* VictoryButtons);
void UnloadVictory(AEGfxTexture* VictoryScreens, AEGfxTexture* VictoryRestarts, AEGfxTexture* VictoryBacktoMMs);
void Victory_ProcessInput(char& input, float& btnX);
void victoryButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, float& btn_x, float& btn_y);
