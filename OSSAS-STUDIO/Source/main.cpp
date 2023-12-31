/******************************************************************************
* \file			main.cpp
* \brief		Contain all the main related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include <memory>
#include "AEEngine.h"
#include "GameState/GameStateList.h"
#include "GameState/GameStateMgr.h"
#include "GameState/Global.h"
#include"Audio/AudioManager.h"

// ---------------------------------------------------------------------------
// Globals
float	 accum = 0.0f;
float&	 g_dt = Global::GetInstance()->g_dt;
double&	 g_appTime = Global::GetInstance()->g_appTime;
Global* Global::m_Instance = nullptr;

/******************************************************************************/
/*!
	Starting point of the application
*/
/******************************************************************************/
int WINAPI WinMain(
	_In_ HINSTANCE instanceH,
	_In_opt_ HINSTANCE prevInstanceH,
	_In_ LPSTR command_line,
	_In_ int show
)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);
	//// Enable run-time memory check for debug builds.

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Initialize the system
	AESysInit(instanceH, show, 800, 600, 1, 60, false, NULL);
	Global::GetInstance()->fontId = AEGfxCreateFont("Resources\\FunSmiles.ttf", 12);
	Global::GetInstance()->fontId_large = AEGfxCreateFont("Resources\\FunSmiles.ttf", 35);
	GameStateMgrInit(GS_PRECREDITS);
	// fullscreen
	AEToogleFullScreen(Global::GetInstance()->full_screen);
	Global::GetInstance()->fullscreen_check = true;
	char sAppTitle[256];	


	while (gGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if (gGameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
		}
		else
			gGameStateNext = gGameStateCurr = gGameStatePrev;

		// Initialize the gamestate
		GameStateInit();


		while (gGameStateCurr == gGameStateNext)
		{
			AESysFrameStart();
			while (accum > 0.016f)
			{
				g_dt = 0.016f;
				accum -= 0.016f;
				AEInputUpdate();

				GameStateUpdate();

				GameStateDraw();
				if (!GetFocus())
				{
					SetWindowPos(AESysGetWindowHandle(), HWND_NOTOPMOST, 0, 0, 800, 600, SWP_NOSIZE | SWP_NOMOVE);
				}
			}

			/*AEInputUpdate();
			GameStateUpdate();
			GameStateDraw();*/

			AESysFrameEnd();

			g_dt = (f32)AEFrameRateControllerGetFrameTime();

			// check if forcing the application to quit
			if (AESysDoesWindowExist() == false)
			{
				gGameStateNext = GS_QUIT;
			}

			//sprintf_s(sAppTitle, "BirdWars (%f)", g_dt);
			sprintf_s(sAppTitle, "BirdWars - Last Flight");
			AESysSetWindowTitle(sAppTitle);
			
			accum += g_dt;
			if(Global::GetInstance()->IsPaused == false)
				g_appTime += g_dt;
		}

		GameStateFree();

		if (gGameStateNext != GS_RESTART)
			GameStateUnload();

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;
	}
	AEGfxDestroyFont(Global::GetInstance()->fontId);
	AEGfxDestroyFont(Global::GetInstance()->fontId_large);
	Global::deleteInstance();
	// free the system
	AESysExit();
}