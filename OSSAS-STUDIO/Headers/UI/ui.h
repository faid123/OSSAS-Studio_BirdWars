/******************************************************************************/
/*!
\file		ui.h
\author 	Nur Syazwan Bin Syamsuddin
\par    	nursyazwan.b@digipen.edu
			100% Code Contribution
\date   	20 Jun, 2020
\brief      This document contains the function headers
			for the UI function.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

enum UI_BUTTONS
{
	DISABLED = 0,
	DUCK,
	OSTRICH,
	PENGUIN,
	MINER,
	BASIC1,
	FREEZE2,
	ULTIMATE3,
	LEFT_A,
	RIGHT_D
};

/*Other function headers*/
void free_button();
void draw_button();
void draw_pause();
void update_pause();
void button_load();
void button_init();
int ColAt(int mouse_x, int mouse_y, int x1, int y1, int x2, int y2);
void unload_button();
void counter_init();
void HoverButton(s32 x, s32 y);
void DrawButtonTint();
void DrawMouseDebug(s32 x, s32 y);