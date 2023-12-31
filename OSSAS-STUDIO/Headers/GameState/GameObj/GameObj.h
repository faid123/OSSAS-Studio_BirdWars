/******************************************************************************
* \file			GameObj.h
* \brief		Contain all the Gameobj related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
* 
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once

#include "AEEngine.h"
#include "Collision/Collision.h"
#include <vector>
#include <string>
/*****************************************************************************/
/*!
 \fn enum class TYPE_OBJECT
 \brief list of objects used
*/
/*****************************************************************************/
enum class TYPE_OBJECT
{
	TYPE_OBJECT_NONE = 0,
	TYPE_OBJECT_TOWER,
	TYPE_OBJECT_HERO,
	TYPE_OBJECT_DUCK,
	TYPE_OBJECT_GROUND,
	TYPE_OBJECT_BULLET,
	TYPE_OBJECT_OSTRICH,
	TYPE_OBJECT_PENGIUN, /// TYPO HERE, i am triggered
	TYPE_OBJECT_MINER
};
/*****************************************************************************/
/*!
 \fn enum class TYPE_COLOUR
 \brief list of COLOUR used
*/
/*****************************************************************************/
enum class COLOUR
{
	RED,
	BLUE,
	GREEN,
	WHITE,
	BLACK
};
/*****************************************************************************/
/*!
 \fn enum class INNER_STATE
 \brief list of state used for attack sequence
*/
/*****************************************************************************/
enum class INNER_STATE
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ATTACK,
	INNER_STATE_IDOL
};
/*****************************************************************************/
/*!
 \fn struct GameObj
 \brief basic object items for reference to when rendering
*/
/*****************************************************************************/
struct GameObj
{
	TYPE_OBJECT			type;		// object type
	AEGfxVertexList*	pMesh;		// pbject
	AEGfxTexture*		texture;
	AEGfxTexture*		texture2;
	AEGfxTexture*		texture3;
	AEGfxTexture*		texture_RIP;
	int					Health;
	int					Attack_Power;
	float				Attack_Range;
	float				Movement_Speed;
	int					Upgrade;
	AEVec2				Scale;
	bool				Enemy;
	int					Cost;
	/*****************************************************************************/
	/*!
	 \fn GameObj();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	GameObj();
	/*****************************************************************************/
	/*!
	 \fn GameObj();
	 \brief implementing Constructor
	 \param TYPE_OBJECT _type
	 the type of object it is
	 \param COLOUR _colour
	 what colour you when to render for the object
	 \param std::string texture_files
	 the location and name of the texture files
	 \param int _Health
	 the standard health of the object
	 \param int _Power
	 the attack power of the object
	 \param float _Attack_Range
	 how far can the object attack can hit
	 \param float _Movement_Speed
	 how fast the object move
	 \param AEVec2 _scale
	 how big the object is
	 \param bool _enemy
	 to make sure they are facing each other
	*/
	/*****************************************************************************/
	GameObj(TYPE_OBJECT _type, COLOUR _colour, 
		std::string texture_files, std::string texture_files2, 
		std::string texture_files3, std::string texture_RIP,
		int _Health, int _Power, 
		float _Attack_Range, float _Movement_Speed, AEVec2 _scale, bool enemy, int _cost);
	/*****************************************************************************/
	/*!
	 \fn GameObj(const GameObj& rhs);
	 \brief copy Constructor
	 \param const GameObj& rhs
	 the value to copy from
	*/
	/*****************************************************************************/
	GameObj(const GameObj& rhs);
	/*****************************************************************************/
	/*!
	 \fn GameObj& operator=(GameObj& rhs);
	 \brief copy assignment
	 \param GameObj& rhs
	 the value to copy from
	*/
	/*****************************************************************************/
	GameObj& operator=(GameObj& rhs);

	// destuctor
	~GameObj();
};
/*****************************************************************************/
/*!
 \fn struct GameObjInst
 \brief basic things need for each item on the screen
*/
/*****************************************************************************/
struct GameObjInst
{

	GameObj*		pObject;	// pointer to the 'original'
	// unsigned int	flag;		// bit flag or-ed together

	AEVec2			posCurr;	// object current position
	AEVec2			velCurr;	// object current velocity

	AEMtx33			transform;	// object drawing matrix

	AABB			boundingBox;// object bouding box that encapsulates the object

	AABB			AttackRange;

	unsigned long		flag;///
	//State of the object instance
	enum			INNER_STATE innerState;

	//General purpose counter (This variable will be used for the enemy state machine)
	float			timer;
	float			lastBulletShotTime;

	bool			enemy;
	bool			Start_Attacking;
	int				HealthCurr;
	GameObjInst*	m_pTarget;
	bool			moving;
	bool			hit;
	/*****************************************************************************/
	/*!
	 \fn GameObjInst();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	GameObjInst();
	/*****************************************************************************/
	/*!
	 \fn GameObjInst(AEVec2 pPos, bool enemy, GameObjInst* target);
	 \brief implementing Constructor
	 \param AEVec2 pPos
	 the pos the object is on
	 \param bool enemy
	 indicating if its an enemy 
	 \param GameObjInst* target
	 giving the object a target 
	*/
	/*****************************************************************************/
	GameObjInst(AEVec2 pPos, bool enemy, GameObjInst* target);
	/*****************************************************************************/
	/*!
	 \fn ~GameObjInst();
	 \brief basic Destructor
	*/
	/*****************************************************************************/
	~GameObjInst();

	/*****************************************************************************/
	/*!
	 \fn bool NeedNewEnemy()
	 \brief checking if the object need an enemy
	*/
	/*****************************************************************************/
	bool NeedNewEnemy();
	/*****************************************************************************/
	/*!
	 \fn void AssignEnemy();
	 \brief Assigning an enemy for the object
	*/
	/*****************************************************************************/
	void AssignEnemy();
	/*****************************************************************************/
	/*!
	 \fn void Calculate_Position(float Mutiply_By);
	 \brief Calculating the position in the game
	 \param float Mutiply_By
	 the deta time to find its position
	*/
	/*****************************************************************************/
	void Calculate_Position(float Mutiply_By);
	/*****************************************************************************/
	/*!
	 \fn void Calculate_BoundingBox();
	 \brief Calculating the BoundingBox for collision
	*/
	/*****************************************************************************/
	void Calculate_BoundingBox();
	/*****************************************************************************/
	/*!
	 \fn bool Within_AttackRange(GameObjInst& rhs);
	 \brief check if the enemy is inside the range of object
	 \param GameObjInst& rhs
	 the object to be checked against
	*/
	/*****************************************************************************/
	bool Within_AttackRange(GameObjInst& rhs);
	/*****************************************************************************/
	/*!
	 \fn void Attack_Sequence();
	 \brief the entire attacking sequence for the obj
	*/
	/*****************************************************************************/
	void Attack_Sequence();
	/*****************************************************************************/
	/*!
	 \fn void show_detail(s8 fontId);
	 \brief for debuging
	 \param s8 fontId
	 the type of fornt used to show the debuging details
	*/
	/*****************************************************************************/
	void show_detail(s8 fontId);
	/*****************************************************************************/
	/*!
	 \fn std::vector< GameObj >& getEnemyObj();
	 \brief get the list of enemy object list
	*/
	/*****************************************************************************/
	std::vector< GameObj >& getEnemyObj();
	/*****************************************************************************/
	/*!
	 \fn std::vector< GameObjInst* >& getEnemyList();
	 \brief get the list of enemy object inst list
	*/
	/*****************************************************************************/
	std::vector< GameObjInst* >& getEnemyList();
	/*****************************************************************************/
	/*!
	 \fn void CreateBullet();
	 \brief when start attack is true create bullet
	*/
	/*****************************************************************************/
	void CreateBullet();


	GameObjInst* GetClosestEnemy();
};
/*****************************************************************************/
/*!
 \fn struct Bullet
 \brief inherited from the gameobjinst with characteristic of the bullet
*/
/*****************************************************************************/
struct Bullet :public GameObjInst
{
	AEVec2	Start_Position;
	float	Range;
	int* Power;
	/*****************************************************************************/
	/*!
	 \fn Bullet();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	Bullet();
	/*****************************************************************************/
	/*!
	 \fn Bullet(AEVec2 pPos, bool enemy, GameObjInst* target, AEVec2 _Start_Position, float _range, int* _power);
	 \brief implementing Constructor
	 \param AEVec2 pPos
	 the pos the object is on
	 \param bool enemy
	 indicating if its an enemy
	 \param GameObjInst* target
	 giving the object a target
	 \param AEVec2 _Start_Position
	 geting the starting position to know where to end
	 \param float _range
	 the place it will end
	 \param int* _power
	 taking the attack power of the object
	*/
	/*****************************************************************************/
	Bullet(AEVec2 pPos, bool enemy, GameObjInst* target, AEVec2 _Start_Position, float _range, int* _power);
	/*****************************************************************************/
	/*!
	 \fn ~GameObjInst();
	 \brief basic Destructor
	*/
	/*****************************************************************************/
	~Bullet();
};
// shortcut for the list of objects
using ObjectList = std::vector< GameObjInst* >;
using Objects = std::vector< GameObj >;
using Bullets = std::vector< Bullet >;
/*****************************************************************************/
/*!
 \fn struct Miner
 \brief special obj miner
*/
/*****************************************************************************/
struct Miner : public GameObjInst
{
	float stop_position;
	float inner_timer;
	/*****************************************************************************/
	/*!
	 \fn Miner();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	Miner();
	/*****************************************************************************/
	/*!
	 \fn Miner(AEVec2 pos);
	 \brief implementing Constructor
	 \param AEVec2 Pos
	*/
	/*****************************************************************************/
	Miner(AEVec2 pos);
	/*****************************************************************************/
	/*!
	 \fn void increase_birdseed();
	 \brief increase birdseed when the timer is up
	*/
	/*****************************************************************************/
	void increase_birdseed();
};
/*****************************************************************************/
/*!
 \fn struct Health_bar
 \brief Things need for health
*/
/*****************************************************************************/
struct Health_bar
{
	GameObjInst*	 Host;
	AEGfxVertexList* Health;
	AEGfxVertexList* BackGround;
	AEVec2			 Health_Pos;
	AEVec2			 Health_Scale;
	AEVec2			 Background_Pos;
	AEVec2			 Background_Scale;
	AEMtx33			 Health_transform;
	AEMtx33			 Background_transform;
	int				 Pre_Health;
	/*****************************************************************************/
	/*!
	 \fn Health_bar();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	Health_bar();
	/*****************************************************************************/
	/*!
	 \fn Health_bar(GameObjInst* __Host);
	 \brief implementing Constructor
	 \param GameObjInst* __Host
	 the instance that is going on top of
	*/
	/*****************************************************************************/
	Health_bar(GameObjInst* _Host);
	/*****************************************************************************/
	/*!
	 \fn ~Health_bar();
	 \brief basic Destructor
	*/
	/*****************************************************************************/
	~Health_bar();
	/*****************************************************************************/
	/*!
	 \fn update();
	 \brief update health bar position and scaling
	*/
	/*****************************************************************************/
	void update();
};
using Health_Bars = std::vector<Health_bar>;
/*****************************************************************************/
/*!
 \fn struct Explosion
 \brief Things need for Explosion
*/
/*****************************************************************************/
struct Explosion
{
	GameObjInst*	 Host;
	AEGfxVertexList* Vertex;
	AEGfxTexture*	 Texture;
	AEVec2*			 Pos;
	AEVec2			 Scale;
	AEMtx33			 Transform_Matrix;
	float			 Timer;
	/*****************************************************************************/
	/*!
	 \fn Explosion();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	Explosion();
	/*****************************************************************************/
	/*!
	 \fn Explosion(GameObjInst* __Host);
	 \brief implementing Constructor
	 \param GameObjInst* __Host
	 the instance that is going on top of
	*/
	/*****************************************************************************/
	Explosion(GameObjInst* Host);
	/*****************************************************************************/
	/*!
	 \fn void update();
	 \brief update explosion matrix
	*/
	/*****************************************************************************/
	void update();
	/*****************************************************************************/
	/*!
	 \fn void draw();
	 \brief draw explosion
	*/
	/*****************************************************************************/
	void draw();
};
using Explosions = std::vector<Explosion>;
/*****************************************************************************/
/*!
 \fn struct Health_mesh
 \brief Mesh use for health bar
*/
/*****************************************************************************/
struct Health_mesh
{
	AEGfxVertexList* Health;
	AEGfxVertexList* BackGround;
	/*****************************************************************************/
	/*!
	 \fn Health_mesh();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	Health_mesh();
};
/*****************************************************************************/
/*!
 \fn struct Explosion_mesh
 \brief Mesh use for Explosion bar
*/
/*****************************************************************************/
struct Explosion_mesh
{
	AEGfxVertexList* Vertex;
	AEGfxTexture* Texture;
	/*****************************************************************************/
	/*!
	 \fn Explosion_mesh();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	Explosion_mesh();
	/*****************************************************************************/
	/*!
	 \fn Explosion_mesh& operator=(Explosion_mesh& rhs);
	 \brief copy assignment
	 \param Explosion_mesh& rhs
	 the value to copy from
	*/
	/*****************************************************************************/
	Explosion_mesh& operator=(Explosion_mesh& rhs);
};
using Explosion_meshs = std::vector<Explosion_mesh>;