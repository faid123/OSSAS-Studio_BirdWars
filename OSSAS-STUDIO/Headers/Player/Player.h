/******************************************************************************/
/*!
\file		Player.h
\author 	Shaikh Faid Bin Omar, shaikhfaid.binomar, 390003019
			100% Code Contribution
\par    	email: shaikhfaid.binomar@digipen.edu
\date   	May 04, 2020
\brief		Contains function and variable declarations for Player.cpp use
			that is all related to the Player game object, particles, camera
			system and also a menu render

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef PLAYER_H_
#define PLAYER_H_

#include "AEEngine.h"
#include "Collision/Collision.h"
#include "GameState/GameObj/GameObj.h"

/* for Player movement direction */
enum playerDirection
{
	right,
	left,
	top,
	down
};

/* for Player attack type when it is called */
enum playerAttack
{
	NONE = 0,
	BASIC,
	REGEN,
	ULTIMATE,
	BOMB,
	LAUNCH
};

/* to be used with particle types */
enum typeObject
{
	NONOBJECT = 0,
	PLAYER,
	ATTACK,
};

/* various particle colour types available */
enum ParticleObject
{
	PARTICLE1 = 0,
	PARTICLE2,
	PARTICLE3
};

/* to be used with the Player Game object, all attributes that concerns the player */
struct Player : public GameObjInst
{
	bool			isJumping;
	bool			isMoving;
	f32				direction;
	int				alive;
	int				lives;
	f32				scale;
	float			height;
	unsigned int	mana;
	int				activate;
	int				step;
	// basic Constructor
	Player();
	// Destructor
	~Player();
};

/* the Player's attack attributes */
typedef struct Attack
{
	/* the damage that each skill will do */
	union damage
	{
		int basic;
		int heavy;
		int ultimate;
	}damage;

	/* bloodlust cost of each skills, J,K,L */
	union ManaCost
	{
		unsigned int cbasic;
		unsigned int cregen;
		unsigned int cultimate;
	}cost;

	/* the attack object attributes */
	int		type;
	f32		scale;
	f32		direction;
	AEVec2	range;
	AEVec2  oldPos;
	AEVec2	posCurr;
	AEVec2  velCurr;
	AEMtx33 transform;
	AABB	boundingBox;

}Attack;

/* the attack timer and cooldown upon spawning, J,K,L */
struct AttackTimer
{
	float timer;
	float cooldown;
	int type;
};

/* the Particle object */
struct ParticleObj
{
	unsigned int	 type;		// object type
	AEGfxVertexList* pMesh;		// pbject
};

/* the Particle object attributes and variables */
struct Particle
{
	ParticleObj* pObject;
	AEVec2 posCurr;
	AEVec2 velCurr;
	unsigned int flag;
	float scale;
	float dirCurr;
	AEMtx33 transform;
	AABB boundingBox;
	double lifetime;
};

/* the Camera data structure to be operated with camera system */
typedef struct Camera
{
	f32 width, height;
	AEVec2 axis;
}Camera;

/*****************************************************************************/
/*!
 \fn void LoadParticle();
 \brief Loading of particle objects
*/
/*****************************************************************************/
void LoadParticle();
/*****************************************************************************/
/*!
 \fn Particle* CreateParticle(float scale, AEVec2* pPos, AEVec2* pVel, 
							float dir, unsigned int type);
 \brief Creating the particles with input
		float scale:		size of particle
		AEVec2* pPos:		initial position of particle
		AEVec2* pVel:		velocity for the particle
		float dir:			direction where particle is going
		unsigned int type:	the type of particle
*/
/*****************************************************************************/
Particle* CreateParticle(float scale, AEVec2* pPos, AEVec2* pVel, 
						float dir, unsigned int type);
/*****************************************************************************/
/*!
 \fn void DestroyParticle(Particle* pInst);
 \brief Destroying all the created particles with input
		Particle* pInst:	the particle object position in the list
*/
/*****************************************************************************/
void DestroyParticle(Particle* pInst);
/*****************************************************************************/
/*!
 \fn void FreeParticle();
 \brief Destroying of particle objects
*/
/*****************************************************************************/
void FreeParticle();
/*****************************************************************************/
/*!
 \fn void UnloadParticle();
 \brief Unloading of particle objects
*/
/*****************************************************************************/
void UnloadParticle();
/*****************************************************************************/
/*!
 \fn void DrawParticle();
 \brief Drawing of particle objects onto the screen
*/
/*****************************************************************************/
void DrawParticle();
/*****************************************************************************/
/*!
 \fn template<typename T>
	void SpawnParticle(T* type, int typeObj, int particle, int life);
 \brief Spawning of particle objects to set it's attributes
*/
/*****************************************************************************/
template<typename T>
void SpawnParticle(T* type, int typeObj, int particle, int life);
/*****************************************************************************/
/*!
 \fn void UpdateParticle();
 \brief Updating of the particle objects every loop
*/
/*****************************************************************************/
void UpdateParticle();
/*****************************************************************************/
/*!
 \fn template <typename T1>
	void UpdateParticleState(int& state, T1* type, int life);
 \brief Updating of particle objects states to determine drawing and destroy
*/
/*****************************************************************************/
template <typename T1>
void UpdateParticleState(int& state, T1* type, int life);
/*****************************************************************************/
/*!
 \fn void AttackCooldown();
 \brief counter for the attack cooldown
*/
/*****************************************************************************/
void AttackCooldown();
/*****************************************************************************/
/*!
 \fn void FreeCoolDown();
 \brief resets the attack counter and cooldown
*/
/*****************************************************************************/
void FreeCoolDown();
/*****************************************************************************/
/*!
 \fn void InitPlayer(Attack* attack);
 \brief Initializes the player, attacks, health bars, particles
*/
/*****************************************************************************/
void InitPlayer(Attack* attack);
/*****************************************************************************/
/*!
 \fn void LoadPlayer();
 \brief Load the Player game object
*/
/*****************************************************************************/
void LoadPlayer();
/*****************************************************************************/
/*!
 \fn void MovePlayerNew();
 \brief Updates the player movement and jumping physics
*/
/*****************************************************************************/
void MovePlayerNew();
/*****************************************************************************/
/*!
 \fn void PlayerAttack(Attack& attack, AEVec2 pos);
 \brief Updates the player attacks and physics accepts input:
		Attack& attack:		the attack object
		AEVec2 pos:			position where the attack should be created
*/
/*****************************************************************************/
void PlayerAttack(Attack& attack, AEVec2 pos);
/*****************************************************************************/
/*!
 \fn void JumpNew(f64 dt);
 \brief Updates the player jump physics accepts input:
		f64 dt:		the delta time to be operated on 
*/
/*****************************************************************************/
void JumpNew(f64 dt);
/*****************************************************************************/
/*!
 \fn void LoadAttack();
 \brief Load the attack objects and textures
*/
/*****************************************************************************/
void LoadAttack();
/*****************************************************************************/
/*!
 \fn void RegenMana();
 \brief Logic to regenerate the player's bloodlust every half second
*/
/*****************************************************************************/
void RegenMana();
/*****************************************************************************/
/*!
 \fn void DrawRegen(Attack* attack);
 \brief Renders the attack type REGEN onto the screen, accepts input:
		Attack* attack:		attack object
*/
/*****************************************************************************/
void DrawRegen(Attack* attack);
/*****************************************************************************/
/*!
 \fn void UnloadRegen();
 \brief Unloads the REGEN attack objects that was created
*/
/*****************************************************************************/
void UnloadRegen();
/*****************************************************************************/
/*!
 \fn void UpdateAttack(Attack* attack);
 \brief Updates the attack physics and logic, accepts input:
		Attack* attack:		attack object
*/
/*****************************************************************************/
void UpdateAttack(Attack* attack);
/*****************************************************************************/
/*!
 \fn void DrawAttack(Attack* attack);
 \brief Renders the attacks onto the screen, accepts input:
		Attack* attack:		attack object
*/
/*****************************************************************************/
void DrawAttack(Attack* attack);
/*****************************************************************************/
/*!
 \fn void UpdateUlt(Attack* attack);
 \brief Updates the ultimate attack physics and logic, accepts input:
		Attack* attack:		attack object
*/
/*****************************************************************************/
void UpdateUlt(Attack* attack);
/*****************************************************************************/
/*!
 \fn void RenderUlt(Attack* attack);
 \brief Renders the ultimate attack onto the screen, accepts input:
		Attack* attack:		attack object
*/
/*****************************************************************************/
void RenderUlt(Attack* attack);
/*****************************************************************************/
/*!
 \fn void DrawPlayer();
 \brief Renders the Player onto the screen
*/
/*****************************************************************************/
void DrawPlayer();
/*****************************************************************************/
/*!
 \fn void UpdatePlayer();
 \brief Updates the Player's physics and logic
*/
/*****************************************************************************/
void UpdatePlayer();
/*****************************************************************************/
/*!
 \fn void CameraPos(Camera* cameral, f32 x, f32 y);
 \brief The camera system to manipulate the camera movement, accepts input:
		Camera* cameral:	Camera data structure
		f32 x:				position of x axis of camera to be operated on
		f32 y:				position of y axis of camera to be operated on
*/
/*****************************************************************************/
void CameraPos(Camera* cameral, f32 x, f32 y);
/*****************************************************************************/
/*!
 \fn void CheckLives();
 \brief Updates the Tower health, player health and player bloodlust
*/
/*****************************************************************************/
void CheckLives();
/*****************************************************************************/
/*!
 \fn void LoadHealth();
 \brief Load the Health meshes and textures
*/
/*****************************************************************************/
void LoadHealth();
/*****************************************************************************/
/*!
 \fn void DrawHealth();
 \brief Renders the health bar onto the screen
*/
/*****************************************************************************/
void DrawHealth();
/*****************************************************************************/
/*!
 \fn void FreeHealth();
 \brief Unload the health meshes and textures
*/
/*****************************************************************************/
void FreeHealth();
/*****************************************************************************/
/*!
 \fn void UpdateMana(Attack& attack);
 \brief Updates the player's bloodlust accordingly, accepts input:
		Attack& attack:		attack object
*/
/*****************************************************************************/
void UpdateMana(Attack& attack);
/*****************************************************************************/
/*!
 \fn void MainMenu_ProcessInput(char& selection, float& btn_y);
 \brief Updates the MainMenu button selection
		char& selection:		the current selection
		float& btn_y:			position y of button
*/
/*****************************************************************************/
void MainMenu_ProcessInput(char& selection, float& btn_y);
/*****************************************************************************/
/*!
 \fn void MainMenu_Render(AEGfxTexture* mainMenu, AEGfxVertexList* menu);
 \brief Renders the MainMenu background onto the screen
		AEGfxTexture* mainMenu:		background image of main menu
		AEGfxVertexList* menu:		mesh of main menu
*/
/*****************************************************************************/
void MainMenu_Render(AEGfxTexture* mainMenu, AEGfxVertexList* menu);
/*****************************************************************************/
/*!
 \fn void menuButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, 
							float& btn_x, float& btn_y, 
							float scalex, float scaley);
 \brief Renders the MainMenu buttons onto the screen
		AEGfxTexture* menuButton:		button image of Main menu
		AEGfxVertexList* button:		mesh of button
		float& btn_x:					position x of the button
		float& btn_y:					position y of the button
		float scalex:					size x of the button
		float scaley:					size y of the button
*/
/*****************************************************************************/
void menuButton_Render(AEGfxTexture* menuButton, AEGfxVertexList* button, 
						float& btn_x, float& btn_y, 
						float scalex, float scaley);
/*****************************************************************************/
/*!
 \fn void otherButton_Render(AEGfxVertexList* button, 
							float x, float y, 
							float scalex, float scaley);
 \brief Renders the Quit Game tint onto the screen
		AEGfxVertexList* button:	mesh of button
		float x:					position x of the button
		float y:					position y of the button
		float scalex:				size x of the button
		float scaley:				size y of the button
*/
/*****************************************************************************/
void otherButton_Render(AEGfxVertexList* button, 
						float x, float y, 
						float scalex, float scaley);
/*****************************************************************************/
/*!
 \fn void attackButton_Render(AEGfxVertexList* pMesh, AEGfxTexture* button_attack);
 \brief Renders the attack button greyed onto the screen
		AEGfxVertexList* pMesh:			mesh of atack button
		AEGfxTexture* button_attack:	texture of attack button
*/
/*****************************************************************************/
void attackButton_Render(AEGfxVertexList* pMesh, AEGfxTexture* button_attack);
/*****************************************************************************/
/*!
 \fn void FreePlayer();
 \brief Unload the Player mesh and textures
*/
/*****************************************************************************/
void FreePlayer();
/*****************************************************************************/
/*!
 \fn void FreeAttack();
 \brief Unload the attack textures
*/
/*****************************************************************************/
void FreeAttack();
/*****************************************************************************/
/*!
 \fn void FreeAttack();
 \brief Unload the ultimate attack mesh and textures
*/
/*****************************************************************************/
void FreeRenderUlt();
/*****************************************************************************/
/*!
 \fn void FreeMenu(AEGfxVertexList* menu, AEGfxVertexList* button);
 \brief Frees the meshes of the menu and menu button
		AEGfxVertexList* menu:			mesh of menu
		AEGfxVertexList* button:		mesh of menu button
*/
/*****************************************************************************/
void FreeMenu(AEGfxVertexList* menu, AEGfxVertexList* button);
/*****************************************************************************/
/*!
 \fn void UnloadMenu(AEGfxTexture* mainMenu, AEGfxTexture* playButton, 
				AEGfxTexture* optionsButton, AEGfxTexture* creditsButton, 
				AEGfxTexture* instructionButton);
 \brief Unloads the textures of menu and its buttons
		AEGfxTexture* mainMenu:				texture of mainmenu
		AEGfxTexture* playButton:			texture of playbutton
		AEGfxTexture* optionsButton:		texture of option button
		AEGfxTexture* creditsButton:		texture of credits button
		AEGfxTexture* instructionButton:	texture of instructions button
*/
/*****************************************************************************/
void UnloadMenu(AEGfxTexture* mainMenu, AEGfxTexture* playButton, 
				AEGfxTexture* optionsButton, AEGfxTexture* creditsButton, 
				AEGfxTexture* instructionButton);
/*****************************************************************************/
/*!
 \fn void MainMenu_Mouse(char& selection, f32& btn_y, s32 x, s32 y);
 \brief Updates the selection menu based on mouse axis
		char& selection:	selection level
		f32& btn_y:			position y of the button axis
		s32 x:				position x of the mouse axis
		s32 y:				position y of the mouse axis
*/
/*****************************************************************************/
void MainMenu_Mouse(char& selection, f32& btn_y, s32 x, s32 y);

#endif // PLAYER_H_