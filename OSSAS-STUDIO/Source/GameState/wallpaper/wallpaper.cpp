/******************************************************************************/
/*!
\file		wallpaper.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
					100% code contribution
\date   	20 Jun, 2020
\brief      To be used in GS_LEVEL1 only, loading of background.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "AEEngine.h"
#include "UI/ui.h"
#include "GameState/Global.h"
#include "GameState/Wallpaper/wallpaper.h"


AEMtx33 scale, trans, transform;

AEGfxVertexList* background;
AEGfxTexture* Background;

void load_bg1()
{
	// Ryan's start code
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
	background = AEGfxMeshEnd();
	AE_ASSERT_MESG(background, "Failed to create mesh menu!!");

	Background = AEGfxTextureLoad("Resources\\Background\\LVL1Background.png");
	AE_ASSERT_MESG(Background, "Failed to create mainMenu texture!");
}

void draw_bg1()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(Background, 0.0f, 0.0f);

	//The following code will adjust the size of the background
	AEMtx33Scale(&scale, 4500.0f, 710.0f); //size 
	AEMtx33Trans(&trans, 0.0f , 0.0f);
	AEMtx33Concat(&transform, &trans, &scale);
	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(background, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransparency(1.0f);
}

void unload_bg1()
{
	AEGfxMeshFree(background);
	AEGfxTextureUnload(Background);
}