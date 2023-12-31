/******************************************************************************/
/*!
\file		AudioManager.h
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      Header files for AudioManager.cpp.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <fmod.h>
#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

extern float BGvol;
extern float SFXvol;

enum class SOUNDTYPE
{
	ST_MMBACKGROUND,
	//ST_PreCred,
	ST_HQBACKGROUND,
	ST_S1BACKGROUND,
	ST_MONSTERDEATH,
	ST_PLAYERATTACKMONSTER,
	ST_PLAYERATTACKNOTHING,
	ST_PLAYERDAMAGED,
	ST_MONSTERDAMAGED,
	ST_A,
	ST_D,
	ST_SPACE,
	ST_PlayerDeath,
	ST_Victory
};

void InitAudio();
void LoadAudio();
void PlayAudio(SOUNDTYPE st);
float GetVolume(FMOD_CHANNEL* channel);
void SetVolume(FMOD_CHANNEL* channel, float volumelvl);
void UpdateAudio();
void FreeAudio();
float GetBGVolume();
float GetSFXVolume();
void SetBGVolume(float volumelvl);
void SetSFXVolume(float volumelvl);

#endif 
