/******************************************************************************/
/*!
\file		Credits.h
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      header files for Credits.cpp.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "AEEngine.h"

void GameState_CreditsLoad();
void GameState_CreditsInit();
void GameState_CreditsUpdate();
void GameState_CreditsDraw();
void GameState_CreditsFree();
void GameState_CreditsUnload();
void FreeCredits(AEGfxVertexList* Credits, AEGfxVertexList* CreditsButtons);
void UnloadCredits(AEGfxTexture* CreditsScreen, AEGfxTexture* CreditsScreens2, 
                    AEGfxTexture* CreditsLeft, AEGfxTexture* CreditsRight,
                    AEGfxTexture* CreditLeft_Hover, AEGfxTexture* CreditRight_Hover);
void Credits_ProcessInput(char& input, float& btnX);
void creditsButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, float& btnX, float& btnY);
