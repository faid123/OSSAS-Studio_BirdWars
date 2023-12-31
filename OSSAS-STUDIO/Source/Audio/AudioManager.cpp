/******************************************************************************/
/*!
\file		AudioManager.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      To be used in all the gamestates.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <fmod.h>
#include <fmod_common.h>
#include"Audio/AudioManager.h"

FMOD_SYSTEM* fmod_system;

FMOD_CHANNEL* channelBGM = 0;
FMOD_CHANNEL* channelSFX = 0;// Chicken hit nothing
FMOD_CHANNEL* channelSFX2 = 0;// Chicken hit successfully
FMOD_CHANNEL* channelSFX3 = 0;// Boss death
FMOD_CHANNEL* channelSFX4 = 0;// Monster damaged

FMOD_SOUND* MMBackgroundMusic;
FMOD_SOUND* LVL1BackgroundMusic;
FMOD_SOUND* MonsterDamagedSFX;
FMOD_SOUND* MonsterDeathSFX;
FMOD_SOUND* PlayerAttackMonSFX;
FMOD_SOUND* PlayerAttackNthSFX;
FMOD_SOUND* PlayerDamagedSFX;
FMOD_SOUND* PlayerJUMPSFX;
FMOD_SOUND* ASkillSFX;
FMOD_SOUND* DSkillSFX;
FMOD_SOUND* PlayerDeath;
FMOD_SOUND* VictoryMusic;

float BGvol;
float SFXvol;
void InitAudio()
{
	FMOD_System_Create(&fmod_system);
	FMOD_System_Init(fmod_system, 32, FMOD_INIT_NORMAL, nullptr);
	BGvol = 0.5f;
	SFXvol = 1.0f;
	LoadAudio();
}

void LoadAudio()
{
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\MMBackground.wav", FMOD_LOOP_NORMAL, FMOD_DEFAULT, &MMBackgroundMusic);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\LVL1Background.wav", FMOD_LOOP_NORMAL, FMOD_DEFAULT, &LVL1BackgroundMusic);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\EnemyDies.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &MonsterDeathSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\ShootingSound.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &PlayerAttackMonSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\ShootingSound.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &PlayerAttackNthSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\ChickenNoise.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &PlayerDamagedSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\EnemyDies.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &MonsterDamagedSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\WalkingSound.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &ASkillSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\WalkingSound.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &DSkillSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\ChickenNoise.wav", FMOD_LOOP_OFF, FMOD_DEFAULT, &PlayerJUMPSFX);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\ChickenNoise.wav", FMOD_LOOP_NORMAL, FMOD_DEFAULT, &PlayerDeath);
	FMOD_System_CreateSound(fmod_system, "Resources\\Audio\\VictoryBackground.wav", FMOD_LOOP_NORMAL, FMOD_DEFAULT, &VictoryMusic);
}

void PlayAudio(SOUNDTYPE st)
{
	if (st == SOUNDTYPE::ST_MMBACKGROUND)
	{
		FMOD_Channel_Stop(channelBGM);
		FMOD_System_PlaySound(fmod_system, MMBackgroundMusic, 0, false, &channelBGM);
		SetBGVolume(BGvol);
	}

	if (st == SOUNDTYPE::ST_S1BACKGROUND)
	{
		FMOD_Channel_Stop(channelBGM);
		FMOD_System_PlaySound(fmod_system, LVL1BackgroundMusic, 0, false, &channelBGM);
		SetBGVolume(BGvol);
	}

	if (st == SOUNDTYPE::ST_MONSTERDEATH)
	{
		FMOD_Channel_Stop(channelSFX3);
		FMOD_System_PlaySound(fmod_system, MonsterDeathSFX, 0, false, &channelSFX3);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_PLAYERATTACKMONSTER)
	{
		FMOD_Channel_Stop(channelSFX2);
		FMOD_System_PlaySound(fmod_system, PlayerAttackMonSFX, 0, false, &channelSFX2);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_PLAYERATTACKNOTHING)
	{
		FMOD_Channel_Stop(channelSFX);
		FMOD_System_PlaySound(fmod_system, PlayerAttackNthSFX, 0, false, &channelSFX);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_PLAYERDAMAGED)
	{
		FMOD_Channel_Stop(channelSFX3);
		FMOD_System_PlaySound(fmod_system, PlayerDamagedSFX, 0, false, &channelSFX3);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_MONSTERDAMAGED)
	{
		FMOD_Channel_Stop(channelSFX4);
		FMOD_System_PlaySound(fmod_system, MonsterDamagedSFX, 0, false, &channelSFX4);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_A)
	{
		FMOD_Channel_Stop(channelSFX);
		FMOD_System_PlaySound(fmod_system, ASkillSFX, 0, false, &channelSFX);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_D)
	{
		FMOD_Channel_Stop(channelSFX);
		FMOD_System_PlaySound(fmod_system, DSkillSFX, 0, false, &channelSFX);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_SPACE)
	{
		FMOD_Channel_Stop(channelSFX);
		FMOD_System_PlaySound(fmod_system, PlayerJUMPSFX, 0, false, &channelSFX);
		SetSFXVolume(SFXvol);
	}

	if (st == SOUNDTYPE::ST_PlayerDeath)
	{
		FMOD_Channel_Stop(channelBGM);
		FMOD_System_PlaySound(fmod_system, PlayerDeath, 0, false, &channelBGM);
		SetSFXVolume(BGvol);
	}

	if (st == SOUNDTYPE::ST_Victory)
	{
		FMOD_Channel_Stop(channelBGM);
		FMOD_System_PlaySound(fmod_system, VictoryMusic, 0, false, &channelBGM);
		SetBGVolume(BGvol);
	}
}

// Function to get current volume float level of the current state
float GetVolume(FMOD_CHANNEL* channel)
{
	float volume;
	FMOD_Channel_GetVolume(channel, &volume);

	return volume;
}

// Function to set volume to current state
void SetVolume(FMOD_CHANNEL* channel, float volumelvl)
{
	FMOD_Channel_SetVolume(channel, volumelvl);
}

// Gets the current volume level of current state
float GetBGVolume()
{
	float volume;
	FMOD_Channel_GetVolume(channelBGM, &volume);

	return volume;
}

// Get the SFX volume of the current state
float GetSFXVolume()
{
	float volume;
	FMOD_Channel_GetVolume(channelSFX, &volume);

	return volume;
}

// Set BG volume of current state
void SetBGVolume(float volumelvl)
{
	FMOD_Channel_SetVolume(channelBGM, volumelvl);
}

// Setting the sfx volume of current channel
void SetSFXVolume(float volumelvl)
{
	FMOD_Channel_SetVolume(channelSFX, volumelvl);
	FMOD_Channel_SetVolume(channelSFX2, volumelvl);
	FMOD_Channel_SetVolume(channelSFX3, volumelvl);
	FMOD_Channel_SetVolume(channelSFX4, volumelvl);
}

// Updates the different system of channels that we initialised
void UpdateAudio()
{
	FMOD_System_Update(fmod_system);
}

// Frees the audio in the game
void FreeAudio()
{
	FMOD_System_Release(fmod_system);
	FMOD_System_Close(fmod_system);
}