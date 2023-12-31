/******************************************************************************/
/*!
\file		Instructions.cpp
\author 	Shaikh Faid Bin Omar, shaikhfaid.binomar, 390003019
			100% Code Contribution
\par    	email: shaikhfaid.binomar@digipen.edu
\date   	June 13, 2020
\brief      To be used in GS_INSTRUCTIONS only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "GameState/Instructions/Instructions.h"
#include "AEEngine.h"
#include "Gamestate/GameStateMgr.h"
#include "Audio/AudioManager.h"
#include "GameState/Global.h"
#include "UI/tutorial.h"

// variable declarations to be used only for instructions state
AEGfxTexture* instructions_page = nullptr;
AEGfxVertexList* instructions = nullptr;
AEGfxTexture* leftbtn = nullptr;
AEGfxVertexList* leftbtnMesh = nullptr;

s32 mouse_y1 = 0, mouse_x1 = 0;
bool leftbtn_hover;

void GameState_InstructionsLoad()
{
	// start adding mesh for the instructions page use
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
	instructions = AEGfxMeshEnd();
	AE_ASSERT_MESG(instructions, "Failed to create mesh instructions!!");

	// load all the required textures to be used later with the meshes
	instructions_page = AEGfxTextureLoad("Resources\\Instructions\\instructions_page.png");
	AE_ASSERT_MESG(instructions_page, "Failed to create instructions_page texture!");

	// start adding mesh for the button use
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
	leftbtnMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(leftbtnMesh, "Failed to create leftbuttonmesh!!");

	// load all the required textures to be used later with the meshes
	leftbtn = AEGfxTextureLoad("Resources\\Instructions\\leftbutton.png");
	AE_ASSERT_MESG(leftbtn, "Failed to create leftbutton texture!");
}

void GameState_InstructionsInit()
{
	// current hover of back button set to false
	leftbtn_hover = false;
	// checking for audio settings
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

void GameState_InstructionsUpdate()
{
	// set volume according to audio settings
	SetBGVolume(Global::GetInstance()->volumeGlobal);
	// checking for fullscreen enabled
	fullscreenCheck();

	// enter this scope when escape key is pressed
	if (AEInputCheckReleased(AEVK_ESCAPE))
	{
		// change the state - MAIN MENU
		gGameStateNext = gGameStatePrev;
		gGameStatePrev = gGameStateCurr;
	}
	// enter this scope when mouse axis is in bounds
	if ((mouse_x1 >= 73 && mouse_x1 <= 224) && (mouse_y1 >= 475 && mouse_y1 <= 561))
	{
		// set variable to true
		leftbtn_hover = true;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			// change the state - MAIN MENU
			gGameStateNext = gGameStatePrev;
			gGameStatePrev = gGameStateCurr;
		}
	}
	else
		leftbtn_hover = false;

	// get the mouse cursor position in windows
	AEInputGetCursorPosition(&mouse_x1, &mouse_y1);

}

void GameState_InstructionsDraw()
{
	// render the instructions page background texture
	MainMenu_Render(instructions_page, instructions);
	// when the mouse hovers over the instruction axis
	// enter this scope if leftbtn_hover var is set to true
	if (leftbtn_hover)
	{
		// declare the scaling, translation and transformation
		AEMtx33 scale, trans, transform;
		// set the texture mode, blending mode, render mode, color, and texture
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxTextureSet(leftbtn, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		// scale the texture accordingly
		AEMtx33Scale(&scale, 400.0f, 500.0f);
		// translate the coordinates
		AEMtx33Trans(&trans, -250.0f, -240.0f);
		// multiply both matrix
		AEMtx33Concat(&transform, &trans, &scale);
		// transform it to global
		AEGfxSetTransform(transform.m);

		// draw the mesh on to the screen
		AEGfxMeshDraw(leftbtnMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);
	}
}

void GameState_InstructionsFree()
{

}

void GameState_InstructionsUnload()
{
	// unload all textures and meshes in GS_INSTRUCTIONS
	AEGfxMeshFree(instructions);
	AEGfxTextureUnload(instructions_page);
	AEGfxMeshFree(leftbtnMesh);
	AEGfxTextureUnload(leftbtn);
}