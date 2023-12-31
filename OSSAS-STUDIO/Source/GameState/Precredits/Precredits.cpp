/******************************************************************************/
/*!
\file		Precredits.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
          100% code contribution
\date   	8 Aug, 2020
\brief      To be used in GS_PRECREDITS only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "GameState/Precredits/Precredits.h"
#include "AEEngine.h"
#include "Gamestate/GameStateMgr.h"
#include "Audio/AudioManager.h" 
#include "GameState/Global.h"
#include "UI/tutorial.h"

// variable declarations to be used only for instructions state
AEGfxTexture* logo = nullptr;
AEGfxVertexList* logoMesh = nullptr;

void GameState_PCLoad()
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
	logoMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(logoMesh, "Failed to create mesh digipen logo!!");

	// load all the required textures to be used later with the meshes
	logo = AEGfxTextureLoad("Resources\\Digipen\\DigiPen.png");
	AE_ASSERT_MESG(logo, "Failed to create mainMenu texture!");
}

void GameState_PCInit()
{

}

void GameState_PCUpdate()
{
	fullscreenCheck();
	if (Global::GetInstance()->g_appTime >= 3.0f)
	{
		gGameStatePrev = gGameStateCurr;
		gGameStateNext = GS_MAINMENU;
	}
}

void GameState_PCDraw()
{
	AEGfxSetCamPosition(0.0f, 0.0f);
	// Drawing mainMenu
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	// Set position for object 1
	AEGfxSetPosition(0.0f, 0.0f);
	// set mainmenu texture
	AEGfxTextureSet(logo, 1.0f, 1.0f);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Drawing the mesh (list of triangles)
	AEMtx33 scale10, trans10, transform10;
	AEMtx33Scale(&scale10, (f32)AEGetWindowWidth() * 1.0f, (f32)AEGetWindowHeight() * 1.0f);
	AEMtx33Trans(&trans10, 0.0f, 0.0f);
	AEMtx33Concat(&transform10, &trans10, &scale10);
	AEGfxSetTransform(transform10.m);
	// render menu image
	AEGfxMeshDraw(logoMesh, AE_GFX_MDM_TRIANGLES);
}

void GameState_PCFree()
{
	AEGfxMeshFree(logoMesh);
}

void GameState_PCUnload()
{
	AEGfxTextureUnload(logo);
}