/******************************************************************************/
/*!
\file		ui.cpp
\author 	Nur Syazwan Bin Syamsuddin
			100% Code Contribution
\par    	nursyazwan.b@digipen.edu
\date   	20 Jun, 2020
\brief      This document contains the function instructions
			for the ui.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "AEEngine.h"
#include "UI/ui.h"
#include "UI/tutorial.h"
#include "GameState/Global.h"
#include "Player/Player.h"
#include "GameState/GameStateMgr.h"
#include "Audio/AudioManager.h"

// Declare Meshes for use
AEGfxVertexList* pMesh1;
AEGfxVertexList* pMesh2;
AEGfxVertexList* pMesh3;
AEGfxVertexList* pMesh4;
AEGfxVertexList* pMesh5;
AEGfxVertexList* pMesh6;

//Declare textures
AEGfxTexture* button1;
AEGfxTexture* button2;
AEGfxTexture* button3;
AEGfxTexture* button4;
AEGfxTexture* button5;
AEGfxTexture* button6;

AEGfxTexture* button_j;
AEGfxTexture* button_k;
AEGfxTexture* button_l;
AEGfxTexture* rightD;
AEGfxTexture* leftA;

AEGfxTexture* yesTexture;
AEGfxTexture* noTexture;

//Global Var Resources
int& BirdSeed = Global::GetInstance()->BirdSeed;
int& minerInst = Global::GetInstance()->minerInst;

float MinerTimer = 0;
float counterMiner = 1;

f32 ui_x = 0.0f, ui_y = 0.0f;
int selection = 0;
f32 btn_scaleX = 0;
f32 btn_scaley = 0;
f64 dt, currTime, prevTime;

s32 mouse_y3 = 0, mouse_x3 = 0;
float offset_y;
int isConfirmSelect = 0;

//Function to initialize button meshes
void button_load()
{
	load_options();
	//AEGfxMeshStart();

	//------------------------Button 1

	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	// Texture 1: From file
	button1 = AEGfxTextureLoad("Resources\\Buttons\\button1p.png");
	AE_ASSERT_MESG(button1, "Failed to create button1p!!");

	// ------------------------Button 2

	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	pMesh2 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh2, "Failed to create mesh 2!!");

	// Texture 1: From file
	button2 = AEGfxTextureLoad("Resources\\Buttons\\button2p.png");
	AE_ASSERT_MESG(button2, "Failed to create button2p!!");

	// ------------------------Button 3

	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	pMesh3 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh3, "Failed to create mesh 3!!");

	// Texture 1: From file
	button3 = AEGfxTextureLoad("Resources\\Buttons\\button3p.png");
	AE_ASSERT_MESG(button3, "Failed to create button3p!!");

	// -----------------------Button 4

	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	pMesh4 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh4, "Failed to create mesh 4!!");

	// Texture 1: From file
	button4 = AEGfxTextureLoad("Resources\\Buttons\\button4p.png");
	AE_ASSERT_MESG(button4, "Failed to create button4p!!");

	// -----------------------Miner Spawn

	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	pMesh5 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh5, "Failed to create mesh 5!!");

	// Texture 1: From file
	button5 = AEGfxTextureLoad("Resources\\Minions\\Miner.png");
	AE_ASSERT_MESG(button5, "Failed to create Miner!!");

	// -----------------------UI Create

	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	pMesh6 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh6, "Failed to create mesh 6!!");

	// Texture 1: From file
	button6 = AEGfxTextureLoad("Resources\\UI\\BottomScreen_UI.png");
	AE_ASSERT_MESG(button6, "Failed to create UI!");

	button_j = AEGfxTextureLoad("Resources\\UI\\button_j.png");
	AE_ASSERT_MESG(button_j, "Failed to create UI!");
	button_k = AEGfxTextureLoad("Resources\\UI\\button_k.png");
	AE_ASSERT_MESG(button_k, "Failed to create UI!");
	button_l = AEGfxTextureLoad("Resources\\UI\\button_l.png");
	AE_ASSERT_MESG(button_l, "Failed to create UI!");
	leftA = AEGfxTextureLoad("Resources\\Buttons\\leftArrowNotPressed.png");
	AE_ASSERT_MESG(leftA, "Failed to create leftArrowNotPressed!");
	rightD = AEGfxTextureLoad("Resources\\Buttons\\rightArrowNotPressed.png");
	AE_ASSERT_MESG(rightD, "Failed to create rightArrowNotPressed!");

	/********************PAUSE************************/
	//menu
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	Global::GetInstance()->PauseMenuMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(Global::GetInstance()->PauseMenuMesh, "Failed to create PauseMenuMesh!!");

	Global::GetInstance()->PauseMenuTexture = AEGfxTextureLoad("Resources\\Pause\\bird wars-last flight_pause.png");
	AE_ASSERT_MESG(Global::GetInstance()->PauseMenuTexture, "Failed to create PauseMenuTexture!");
	// pause button
	// resume
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	Global::GetInstance()->ResumeMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(Global::GetInstance()->ResumeMesh, "Failed to create ResumeMesh!!");

	Global::GetInstance()->ResumeTexture = AEGfxTextureLoad("Resources\\Pause\\bird wars-last flight_resumebutton_greyed.png");
	AE_ASSERT_MESG(Global::GetInstance()->ResumeTexture, "Failed to create ResumeTexture!");
	// mainmenu
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	Global::GetInstance()->MainMenuMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(Global::GetInstance()->MainMenuMesh, "Failed to create MainMenuMesh!!");

	Global::GetInstance()->MainMenuTexture = AEGfxTextureLoad("Resources\\Pause\\bird wars-last flight_menubutton_greyed.png");
	AE_ASSERT_MESG(Global::GetInstance()->MainMenuTexture, "Failed to create MainMenuTexture!");

	// restart
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	Global::GetInstance()->RestartMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(Global::GetInstance()->RestartMesh, "Failed to create MainMenuMesh!!");

	Global::GetInstance()->RestartTexture = AEGfxTextureLoad("Resources\\Pause\\confirmation.png");
	AE_ASSERT_MESG(Global::GetInstance()->RestartTexture, "Failed to create MainMenuTexture!");
	Global::GetInstance()->restartGame = AEGfxTextureLoad("Resources\\Pause\\restartConfirm.png");
	AE_ASSERT_MESG(Global::GetInstance()->restartGame, "Failed to create restartGame texture!");
	Global::GetInstance()->option_pause = AEGfxTextureLoad("Resources\\Pause\\option.png");
	AE_ASSERT_MESG(Global::GetInstance()->option_pause, "Failed to create option_pause texture!");
	yesTexture = AEGfxTextureLoad("Resources\\Pause\\yes.png");
	AE_ASSERT_MESG(yesTexture, "Failed to create yes texture!");
	noTexture = AEGfxTextureLoad("Resources\\Pause\\no.png");
	AE_ASSERT_MESG(noTexture, "Failed to create no texture!");
}


void counter_init()
{
	counterMiner = 1;
	btn_scaleX = 14;
	btn_scaley = 14;
}


void button_init()
{
	Global::GetInstance()->camera.axis.x = 0.0f;
}

// Function to draw and colour the buttons appropriately when it is being pressed
void draw_button()
{
	AEMtx33 scale, trans, transform;

	//Create UI
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(button6, 0.0f, 0.0f);


	//Offset 112.0
	//The following code will adjust the size of the buttons
	AEMtx33Scale(&scale, 14.5f, 14.2f);
	AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x, Global::GetInstance()->camera.axis.y - 112.0f);

	AEMtx33Concat(&transform, &trans, &scale);
	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(pMesh6, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransparency(1.0f);

	if (AEInputCheckCurr(AEVK_Q))
	{
		//you must know the syntax chose one lmao
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxTextureSet(button1, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 248.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);

	}

	if (AEInputCheckCurr(AEVK_W))
	{

		// Drawing button3 - (first) - No tint
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(button2, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 251.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);
		// Set Transparency
		AEGfxSetTransparency(1.0f);

	}

	if (AEInputCheckCurr(AEVK_E))
	{
		// Drawing button3 - (first) - No tint
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(button3, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 254.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMesh3, AE_GFX_MDM_TRIANGLES);
		// Set Transparency
		AEGfxSetTransparency(1.0f);
	}

	if (AEInputCheckCurr(AEVK_R))
	{
		// Drawing button3 - (first) - No tint
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(button4, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 257.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMesh4, AE_GFX_MDM_TRIANGLES);
		// Set Transparency
		AEGfxSetTransparency(1.0f);


	}

	

	//Prompt to deploy resource penguin
	if (Global::GetInstance()->minerInst < 1)
	{
		MinerTimer += Global::GetInstance()->g_dt;
		if (MinerTimer >= 2.0f)
		{
			// Drawing button3 - (first) - No tint
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			//blend to bg
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			// No tint
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			// Set texture
			AEGfxTextureSet(button4, 0.0f, 0.0f);

			//The following code will adjust the size of the buttons
			AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
			AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 257.0f, Global::GetInstance()->camera.axis.y - 112.0f);
			AEMtx33Concat(&transform, &trans, &scale);
			AEGfxSetTransform(transform.m);

			// Drawing the mesh (list of triangles)
			AEGfxMeshDraw(pMesh4, AE_GFX_MDM_TRIANGLES);
			// Set Transparency
			AEGfxSetTransparency(1.0f);

			

		}
	}

	if (MinerTimer >= 4)
	{
		MinerTimer = 0.0f;
	}


	//	//if (counterMiner > 0)
	//	//{
	//	//	// Drawing button3 - (first) - No tint
	//	//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//	//	//blend to bg
	//	//	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//	//	// No tint
	//	//	AEGfxSetTintColor(0.0f, 1.0f, 1.0f, 1.0f);
	//	//	// Set texture
	//	//	AEGfxTextureSet(button4, 0.0f, 0.0f);

	//	//	//The following code will adjust the size of the buttons
	//	//	AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
	//	//	AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 140, Global::GetInstance()->camera.axis.y - 125.0f);
	//	//	AEMtx33Concat(&transform, &trans, &scale);
	//	//	AEGfxSetTransform(transform.m);

	//	//	// Drawing the mesh (list of triangles)
	//	//	AEGfxMeshDraw(pMesh4, AE_GFX_MDM_TRIANGLES);
	//	//	// Set Transparency
	//	//	AEGfxSetTransparency(1.0f);
	//	//}
	//}


	//Draw Debug

	//printf("MinerInst: %d\n", Global::GetInstance()->minerInst);

	//Miner Inst Debug
	char strBuffer1[100];
	memset(strBuffer1, 0, 100 * sizeof(char));
	sprintf_s(strBuffer1, "Miner Inst: %d / 3", minerInst);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBuffer1, 0.0f, 0.85f, 1.0f, 1.0f, 1.f, 1.f);

	//MinerTimer Inst Debug
	char strBuffer2[100];
	memset(strBuffer2, 0, 100 * sizeof(char));
	sprintf_s(strBuffer2, "MinerTimer Inst: %f ", MinerTimer);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBuffer2, 0.0f, 0.80f, 1.0f, 1.0f, 1.f, 1.f);

	//Ready checker for J
	char strBufferJ[100];
	memset(strBufferJ, 0, 100 * sizeof(char));

	if (Global::GetInstance()->player.mana > 2)
	{
		sprintf_s(strBufferJ, "Ready!");
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Global::GetInstance()->fontId, strBufferJ, 0.22f, -0.66f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	else
	{
		sprintf_s(strBufferJ, "Not Ready!");
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Global::GetInstance()->fontId, strBufferJ, 0.22f, -0.66f, 1.0f, 1.0f, 0.0f, 0.0f);
	}
	

	//Ready checker for K
	char strBufferK[100];
	memset(strBufferK, 0, 100 * sizeof(char));

	if (Global::GetInstance()->player.mana > 9)
	{
		sprintf_s(strBufferK, "Ready!");
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Global::GetInstance()->fontId, strBufferK, 0.44f, -0.66f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	else
	{
		sprintf_s(strBufferK, "Not Ready!");
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Global::GetInstance()->fontId, strBufferK, 0.44f, -0.66f, 1.0f, 1.0f, 0.0f, 0.0f);
	}


	//Ready checker for L
	char strBufferL[100];
	memset(strBufferL, 0, 100 * sizeof(char));

	if (Global::GetInstance()->player.mana > 30)
	{
		sprintf_s(strBufferL, "Ready!");
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Global::GetInstance()->fontId, strBufferL, 0.65f, -0.66f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	else
	{
		sprintf_s(strBufferL, "Not Ready!");
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Global::GetInstance()->fontId, strBufferL, 0.65f, -0.66f, 1.0f, 1.0f, 0.0f, 0.0f);
	}


	/*******************************************************************************************************************/
	//Q for duck
	//char strBuffer3[100];
	//memset(strBuffer3, 0, 100 * sizeof(char));
	//sprintf_s(strBuffer3, "Q");

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBuffer3, -0.86f, -0.37f, 1.0f, 1.0f, 1.f, 1.f);

	////Price for duck
	//char strBufferA[100];
	//memset(strBufferA, 0, 100 * sizeof(char));
	//sprintf_s(strBufferA, "100");
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBufferA, -0.82f, -0.59f, 1.0f, 1.0f, 1.f, 1.f);

	/*******************************************************************************************************************/

	//W for Ostrich
	//char strBuffer4[100];
	//memset(strBuffer4, 0, 100 * sizeof(char));
	//sprintf_s(strBuffer4, "W");

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBuffer4, -0.62f, -0.37f, 1.0f, 1.0f, 1.f, 1.f);

	////Price for Ostrich
	//char strBufferB[100];
	//memset(strBufferB, 0, 100 * sizeof(char));
	//sprintf_s(strBufferB, "200");

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBufferB, -0.63f, -0.59f, 1.0f, 1.0f, 1.f, 1.f);

	/*******************************************************************************************************************/

	 //E for Attack Penguin
	//char strBuffer5[100];
	//memset(strBuffer5, 0, 100 * sizeof(char));
	//sprintf_s(strBuffer5, "E");

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBuffer5, -0.41f, -0.37f, 1.0f, 1.0f, 1.f, 1.f);

	////Penguin PRICE
	//char strBufferc[100];
	//memset(strBufferc, 0, 100 * sizeof(char));
	//sprintf_s(strBufferc, "150");

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBufferc, -0.45f, -0.59f, 1.0f, 1.0f, 1.f, 1.f);

	/*******************************************************************************************************************/

	//R for Resource Penguin
	//char strBuffer6[100];
	//memset(strBuffer6, 0, 100 * sizeof(char));
	//sprintf_s(strBuffer6, "R");

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBuffer6, -0.21f, -0.37f, 1.0f, 1.0f, 1.f, 1.f);

	////R for Resource Penguin
	//char strBufferd[100];
	//memset(strBufferd, 0, 100 * sizeof(char));
	//sprintf_s(strBufferd, "50");

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(Global::GetInstance()->fontId, strBufferd, -0.26f, -0.59f, 1.0f, 1.0f, 1.f, 1.f);


	//Mission Obj
	/*char strBuffer7[100];
	memset(strBuffer7, 0, 100 * sizeof(char));
	sprintf_s(strBuffer7, "Objective: Destroy the ENEMY TOWER! ");

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Global::GetInstance()->fontId, strBuffer7, -0.72f, -0.472f, 1.0f, 1.0f, 1.f, 1.f);*/

#if 0
	char strBuffer9[100];
	memset(strBuffer9, 0, 100 * sizeof(char));
	sprintf_s(strBuffer9, "%d", Global::GetInstance()->boss_lvll1.HealthCurr);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Global::GetInstance()->fontId, strBuffer9, 0.70f, 0.0f, 1.5f, 1.0f, 1.0f, 1.0f);
#endif

}

void update_pause()
{
	//CameraPos(&Global::GetInstance()->camera, Global::GetInstance()->camera.axis.x, 0.0f);

	//if (AEInputCheckReleased(AEVK_RIGHT))
	//	++Global::GetInstance()->pauseCase;
	//if (AEInputCheckReleased(AEVK_LEFT))
	//	--Global::GetInstance()->pauseCase;

	//if (Global::GetInstance()->pauseCase > 4)
	//	Global::GetInstance()->pauseCase = 1;
	//if (Global::GetInstance()->pauseCase < 1)
	//	Global::GetInstance()->pauseCase = 4;

	//if (AEInputCheckReleased(AEVK_RETURN))
	//{
	//	if (Global::GetInstance()->pauseCase == 1)
	//	{			
	//		Global::GetInstance()->IsPaused = false;
	//	}
	//	else if (Global::GetInstance()->pauseCase == 2)
	//	{
	//		Global::GetInstance()->IsPaused = false;
	//	}
	//	else if (Global::GetInstance()->pauseCase == 3)
	//	{
	//		Global::GetInstance()->IsPaused = false;
	//	}
	//	else if (Global::GetInstance()->pauseCase == 4)
	//	{
	//		//print confirm
	//		printf("Confirm?\n");
	//	}
	//	else
	//	{
	//		gGameStateNext = GS_MAINMENU;
	//	}
	//}

	if (!Global::GetInstance()->isOption && !Global::GetInstance()->isConfirm)
	{
		
		SetBGVolume(0.0f);
		if ((mouse_x3 >= 201 && mouse_x3 <= 325) && (mouse_y3 >= 356 && mouse_y3 <= 423))
		{
			Global::GetInstance()->pauseCase = 1;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				Global::GetInstance()->IsPaused = false;
			}
		}
		else if ((mouse_x3 >= 305 && mouse_x3 <= 503) && (mouse_y3 >= 451 && mouse_y3 <= 520))
		{
			Global::GetInstance()->pauseCase = 2;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				Global::GetInstance()->isConfirm = true;
				/*gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_MAINMENU;*/
			}
		}
		else if ((mouse_x3 >= 343 && mouse_x3 <= 466) && (mouse_y3 >= 356 && mouse_y3 <= 423))
		{
			Global::GetInstance()->pauseCase = 3;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_RESTART;
			}
		}
		else if ((mouse_x3 >= 482 && mouse_x3 <= 611) && (mouse_y3 >= 356 && mouse_y3 <= 423))
		{
			Global::GetInstance()->pauseCase = 4;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				// logic goes here for OPTIONS
				Global::GetInstance()->pauseCase = 0;
				Global::GetInstance()->isOption = true;
			}
		}
		/*else
		{
			Global::GetInstance()->pauseCase = 0;
		}*/
		if (AEInputCheckReleased(AEVK_RIGHT))
		{
			Global::GetInstance()->pauseCase++;
		}
		else if (AEInputCheckReleased(AEVK_LEFT))
		{
			Global::GetInstance()->pauseCase--;
		}
		Global::GetInstance()->pauseCase = Global::GetInstance()->pauseCase > 4 ? 1 : Global::GetInstance()->pauseCase;
		Global::GetInstance()->pauseCase = Global::GetInstance()->pauseCase < 1 ? 4 : Global::GetInstance()->pauseCase;

		if (AEInputCheckReleased(AEVK_RETURN))
		{
			if (Global::GetInstance()->pauseCase == 1)
			{
				Global::GetInstance()->IsPaused = false;
			}
			else if (Global::GetInstance()->pauseCase == 2)
			{
				Global::GetInstance()->isConfirm = true;
			}
			else if (Global::GetInstance()->pauseCase == 3)
			{
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_RESTART;
			}
			else if (Global::GetInstance()->pauseCase == 4)
			{
				// logic goes here for OPTIONS
				Global::GetInstance()->pauseCase = 0;
				Global::GetInstance()->isOption = true;
			}
		}
	}
	else if (Global::GetInstance()->isOption && !Global::GetInstance()->isConfirm)
	{
		update_options();
	}
	else if (!Global::GetInstance()->isOption && Global::GetInstance()->isConfirm)
	{
		if (Global::GetInstance()->IsPaused)
			SetBGVolume(0.0f);
		if ((mouse_x3 >= 168 && mouse_x3 <= 372) && (mouse_y3 >= 337 && mouse_y3 <= 446))
		{
			isConfirmSelect = 1;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_MAINMENU;
				isConfirmSelect = 0;
				Global::GetInstance()->isConfirm = false;
			}
		}
		if ((mouse_x3 >= 416 && mouse_x3 <= 620) && (mouse_y3 >= 337 && mouse_y3 <= 446))
		{
			isConfirmSelect = 2;
			if (AEInputCheckReleased(AEVK_LBUTTON))
			{
				isConfirmSelect = 0;
				Global::GetInstance()->isConfirm = false;
			}
		}

		if (AEInputCheckReleased(AEVK_RIGHT))
		{
			isConfirmSelect++;
		}
		else if (AEInputCheckReleased(AEVK_LEFT))
		{
			isConfirmSelect--;
		}
		isConfirmSelect = isConfirmSelect > 2 ? 1 : isConfirmSelect;
		isConfirmSelect = isConfirmSelect < 1 ? 2 : isConfirmSelect;

		if (AEInputCheckReleased(AEVK_RETURN))
		{
			if (isConfirmSelect == 1)
			{
				gGameStatePrev = gGameStateCurr;
				gGameStateNext = GS_MAINMENU;
			}
			else if (isConfirmSelect == 2)
			{
				isConfirmSelect = 0;
				Global::GetInstance()->isConfirm = false;
			}
		}
	}
	AEInputGetCursorPosition(&mouse_x3, &mouse_y3);
}
void draw_pause()
{
	AEMtx33 scale, trans, transform;
	// pause menu
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//blend to bg
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set texture
	AEGfxTextureSet(Global::GetInstance()->PauseMenuTexture, 0.0f, 0.0f);

	//The following code will adjust the size of the buttons
	AEMtx33Scale(&scale, 400.0f / 30.0f, 10.0f);
	AEMtx33Trans(&trans, 0.0f + Global::GetInstance()->camera.axis.x, 0.0f + Global::GetInstance()->camera.axis.y);
	AEMtx33Concat(&transform, &trans, &scale);
	AEGfxSetTransform(transform.m);

	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(Global::GetInstance()->PauseMenuMesh, AE_GFX_MDM_TRIANGLES);
	// Set Transparency
	AEGfxSetTransparency(1.0f);

	switch (Global::GetInstance()->pauseCase)
	{
	case 1:
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(Global::GetInstance()->ResumeTexture, 0.0f, 0.0f);
		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, (400.0f * (486.0f / 1920.0f)) / 45.0f, (300.0f * (198.0f / 1080.0f)) / 45.0f);
		AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x - 135.0f, -93.0f + Global::GetInstance()->camera.axis.y);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(Global::GetInstance()->ResumeMesh, AE_GFX_MDM_TRIANGLES);
		break;
	case 2:
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(Global::GetInstance()->MainMenuTexture, 0.0f, 0.0f);
		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, (400.0f * (486.0f / 1920.0f)) / 30.0f, (300.0f * (198.0f / 1080.0f)) / 45.0f);
		AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x + 5.0f, -185.0f + Global::GetInstance()->camera.axis.y);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(Global::GetInstance()->MainMenuMesh, AE_GFX_MDM_TRIANGLES);
		break;
	case 3:
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(Global::GetInstance()->restartGame, 0.0f, 0.0f);
		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, (400.0f * (486.0f / 1920.0f)) / 45.0f, (300.0f * (198.0f / 1080.0f)) / 45.0f);
		AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x + 5.0f, -93.0f + Global::GetInstance()->camera.axis.y);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(Global::GetInstance()->MainMenuMesh, AE_GFX_MDM_TRIANGLES);
		break;
	case 4:
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(Global::GetInstance()->option_pause, 0.0f, 0.0f);
		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, (400.0f * (486.0f / 1920.0f)) / 45.0f, (300.0f * (198.0f / 1080.0f)) / 45.0f);
		AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x + 145.0f, -93.0f + Global::GetInstance()->camera.axis.y);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(Global::GetInstance()->MainMenuMesh, AE_GFX_MDM_TRIANGLES);
		break;
	default:
		break;
	}
	if (Global::GetInstance()->isOption)
	{
		draw_options_pause();
	}
	if (Global::GetInstance()->isConfirm)
	{
		//AEMtx33 scale, trans, transform;
		// pause menu
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(Global::GetInstance()->RestartTexture, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, 400.0f / 30.0f, 10.0f);
		AEMtx33Trans(&trans, 0.0f + Global::GetInstance()->camera.axis.x, 0.0f + Global::GetInstance()->camera.axis.y);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(Global::GetInstance()->RestartMesh, AE_GFX_MDM_TRIANGLES);
		// Set Transparency
		AEGfxSetTransparency(1.0f);

		if (isConfirmSelect == 1)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			//blend to bg
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			// No tint
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			// Set texture
			AEGfxTextureSet(yesTexture, 0.0f, 0.0f);
			//The following code will adjust the size of the buttons
			AEMtx33Scale(&scale, (400.0f * (486.0f / 1920.0f)) / 30.0f, (300.0f * (198.0f / 1080.0f)) / 30.0f);
			AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x + -130.0f, -93.0f + Global::GetInstance()->camera.axis.y);
			AEMtx33Concat(&transform, &trans, &scale);
			AEGfxSetTransform(transform.m);
			// Drawing the mesh (list of triangles)
			AEGfxMeshDraw(Global::GetInstance()->MainMenuMesh, AE_GFX_MDM_TRIANGLES);
		}
		else if (isConfirmSelect == 2)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			//blend to bg
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			// No tint
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			// Set texture
			AEGfxTextureSet(noTexture, 0.0f, 0.0f);
			//The following code will adjust the size of the buttons
			AEMtx33Scale(&scale, (400.0f * (486.0f / 1920.0f)) / 30.0f, (300.0f * (198.0f / 1080.0f)) / 30.0f);
			AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x + 118.0f, -93.0f + Global::GetInstance()->camera.axis.y);
			AEMtx33Concat(&transform, &trans, &scale);
			AEGfxSetTransform(transform.m);
			// Drawing the mesh (list of triangles)
			AEGfxMeshDraw(Global::GetInstance()->MainMenuMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

// Function to free button mesh
void free_button()
{
	AEGfxMeshFree(pMesh1);
	AEGfxMeshFree(pMesh2);
	AEGfxMeshFree(pMesh3);
	AEGfxMeshFree(pMesh4);
	AEGfxMeshFree(pMesh5);
	AEGfxMeshFree(pMesh6);

	AEGfxMeshFree(Global::GetInstance()->PauseMenuMesh);
	AEGfxMeshFree(Global::GetInstance()->ResumeMesh);
	AEGfxMeshFree(Global::GetInstance()->MainMenuMesh);
	AEGfxMeshFree(Global::GetInstance()->RestartMesh);
}

//Function to Unload Button Texture
void unload_button()
{
	unload_options();
	AEGfxTextureUnload(button1);
	AEGfxTextureUnload(button2);
	AEGfxTextureUnload(button3);
	AEGfxTextureUnload(button4);
	AEGfxTextureUnload(button5);
	AEGfxTextureUnload(button6);

	AEGfxTextureUnload(button_j);
	AEGfxTextureUnload(button_k);
	AEGfxTextureUnload(button_l);
	AEGfxTextureUnload(leftA);
	AEGfxTextureUnload(rightD);

	AEGfxTextureUnload(yesTexture);
	AEGfxTextureUnload(noTexture);

	AEGfxTextureUnload(Global::GetInstance()->PauseMenuTexture);
	AEGfxTextureUnload(Global::GetInstance()->ResumeTexture);
	AEGfxTextureUnload(Global::GetInstance()->MainMenuTexture);
	AEGfxTextureUnload(Global::GetInstance()->RestartTexture);
	AEGfxTextureUnload(Global::GetInstance()->restartGame);
	AEGfxTextureUnload(Global::GetInstance()->option_pause);
}


//if cursor collides object returns 1 else 0
//uses aabb method heh
int ColAt(int mouse_x, int mouse_y, int x1, int y1, int x2, int y2)
{
	if (mouse_x >= x1 && mouse_x <= x2 && mouse_y <= y2 && mouse_y >= y1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


// SHAIKH DOING THIS DON'T TOUCH
void HoverButton(s32 x, s32 y)
{
	/**************SUMMONING ABILITIES PART*****************/
	/* FOR BUTTON Q - DUCK SUMMON */
	if ((x >= 27 && x <= 100) && (y >= 397 && y <= 474))
	{
		selection = DUCK;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			Global::GetInstance()->mouse_spawn = 1;
		}
	}
	/* FOR BUTTON W - OSTRICH SUMMON */
	else if ((x >= 112 && x <= 187) && (y >= 397 && y <= 474))
	{
		selection = OSTRICH;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			Global::GetInstance()->mouse_spawn = 2;
		}
	}
	/* FOR BUTTON E - PENGUIN SUMMON */
	else if ((x >= 196 && x <= 272) && (y >= 397 && y <= 474))
	{
		selection = PENGUIN;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			Global::GetInstance()->mouse_spawn = 3;
		}
	}
	/* FOR BUTTON R - MINER SUMMON */
	else if ((x >= 283 && x <= 358) && (y >= 397 && y <= 474))
	{
		selection = MINER;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			Global::GetInstance()->mouse_spawn = 4;
		}
	}

	/**************PLAYER ABILITIES PART*****************/
	else if ((x >= 470 && x <= 552) && (y >= 497 && y <= 572))
	{
		selection = BASIC1;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			Global::GetInstance()->player.activate = 1;
		}
	}
	else if ((x >= 560 && x <= 642) && (y >= 497 && y <= 572))
	{
		selection = FREEZE2;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			Global::GetInstance()->player.activate = 2;
		}
	}
	else if ((x >= 652 && x <= 731) && (y >= 497 && y <= 572))
	{
		selection = ULTIMATE3;
		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			Global::GetInstance()->player.activate = 3;
		}
	}
	else if ((x >= 75 && x <= 222) && (y >= 502 && y <= 565) || AEInputCheckCurr(AEVK_A))
	{
		selection = LEFT_A;
		if (AEInputCheckCurr(AEVK_LBUTTON))
		{
			Global::GetInstance()->player.step = 1;
		}
	}
	else if ((x >= 243 && x <= 387) && (y >= 502 && y <= 565) || AEInputCheckCurr(AEVK_D))
	{
		selection = RIGHT_D;
		if (AEInputCheckCurr(AEVK_LBUTTON))
		{
			Global::GetInstance()->player.step = 2;
		}
	}
	else
	{
		selection = DISABLED;
	}

}

void debug_controls()
{
	if (AEInputCheckCurr(AEVK_8))
	{
		Global::GetInstance()->player.HealthCurr = 100000;
		Global::GetInstance()->player.mana = 100000;
		Global::GetInstance()->BirdSeed = 100000;
		//boss_lvl1.HealthCurr -= 50;
	}
}

void DrawButtonTint()
{
	AEMtx33 scale, trans, transform;
	/* RENDER THE TINTS HERE */
	if (selection == DUCK)
	{
		//you must know the syntax chose one lmao
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxTextureSet(button1, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 248.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);
	}
	else if (selection == OSTRICH)
	{
		//you must know the syntax chose one lmao
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxTextureSet(button2, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 251.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);
	}
	else if (selection == PENGUIN)
	{
		// Drawing button3 - (first) - No tint
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(button3, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 254.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMesh3, AE_GFX_MDM_TRIANGLES);
		// Set Transparency
		AEGfxSetTransparency(1.0f);
	}
	else if (selection == MINER)
	{
		// Drawing button3 - (first) - No tint
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//blend to bg
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(button4, 0.0f, 0.0f);

		//The following code will adjust the size of the buttons
		AEMtx33Scale(&scale, btn_scaleX, btn_scaley);
		AEMtx33Trans(&trans, -260.0f + Global::GetInstance()->camera.axis.x + 257.0f, Global::GetInstance()->camera.axis.y - 112.0f);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);

		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMesh4, AE_GFX_MDM_TRIANGLES);
		// Set Transparency
		AEGfxSetTransparency(1.0f);
	}
	else if (selection == BASIC1)
	{
		attackButton_Render(pMesh6, button_j);
	}
	else if (selection == FREEZE2)
	{
		attackButton_Render(pMesh6, button_k);
	}
	else if (selection == ULTIMATE3)
	{
		attackButton_Render(pMesh6, button_l);
	}
	else if (selection == LEFT_A)
	{
		attackButton_Render(pMesh6, leftA);
	}
	else if (selection == RIGHT_D)
	{
		attackButton_Render(pMesh6, rightD);
	}
	else
	{
		// CHANGE BACK THE TINT TO ORIGINAL IMAGE
	}
}

// SHAIKH DOING THIS DON'T TOUCH
void DrawMouseDebug(s32 x, s32 y)
{
	char strBuffer9[100];
	memset(strBuffer9, 0, 100 * sizeof(char));
	sprintf_s(strBuffer9, "mouse.x: %i\nmouse.y: %i", x, y);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Global::GetInstance()->fontId, strBuffer9, -0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 1.0f);
}

 