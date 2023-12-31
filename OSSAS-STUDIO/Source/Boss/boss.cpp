/******************************************************************************/
/*!
\file		boss.cpp
\author 	Nur Syazwan Bin Syamsuddin
            100% Code Contribution
\par    	nursyazwan.b@digipen.edu
\date   	20 Jun, 2020
\brief      This document contains the function instructions
            for the boss minion

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Boss/boss.h"
#include "GameState/Global.h"
#include "AEEngine.h"
#include "Player/Player.h"
#include "Audio/AudioManager.h"

struct Position
{
    float oldPos_y = 0, oldPos_x = 0, newPos_y = 0, newPos_x = 0;
    float velocity = 120.0f, gravity = -250.0f;
    float floor_y = 0.0f, floor_x = 0.0;

}Sprite;


//basic constructor
Boss::Boss() :

    Alive{ 1 },
    Lives{ 1 },
    Direction{ left },
    Height{ 1.0f },
    Scale{ 150 },
    IsJumping{ false },
    IsMoving{ false }

{
    posCurr.x = 2100;
    posCurr.y = 105.0f;
    velCurr.x = -200.0f;
    velCurr.y = 0.0f;
    HealthCurr = 1000;
    timer = 0.0f;
}


//Global Var

int boss_dir = 1;
int dist = 1000;
Boss& boss_lvl1 = Global::GetInstance()->boss_lvll1;
double timer = 10;
bool status = false; // counter start
float counter;
float counter_rush = 3;
float controller;
int state;
bool jump;
bool needNum = true; // need number then give random number
int randomNumber;
bool spawnBoss = false;
bool rush = false;
int step = 0;
/************Bullet Var************/

static BossBullet* bullet;
static BulletObj bulletObj;
const int BULLET_SIZE = 10;
const unsigned int GAME_OBJ_INST_NUM_MAX = 2048U;
static unsigned int sGameObjNum;
AttackTimer* Timer;
const unsigned int ATTACK_OBJ_NUM_MAX = 5;
static BossBullet			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];

//Flags
const unsigned int	FLAG_ACTIVE = 0x00000001;
const unsigned int	FLAG_VISIBLE = 0x00000002;

//fire
bool fire = false;

//boss
int BOSS_HEALTH = 1000;
bool bossActive = false; // if true, then boss will start shootin; 1900 end of map
bool movingLeft = false;
float cooldown;  // cooldown for the big man called the bossman
bool coolDownState = false;
float deathCounter;
int returnPoint;
float strobe = 0;

//Anime
float timerAnime; // for animation
bool isMoving = false; // for animation

// Destructor
Boss::~Boss()
{

}

AEVec2 Boss::getPos() {
    return posCurr;
}


void boss_init()
{
    //init other stuff
    timerAnime = 0.5;

    boss_lvl1.Alive = true;
    boss_lvl1.posCurr.x = 2100;  //set to 2100 // 500 to test
    boss_lvl1.posCurr.y = Global::GetInstance()->player.posCurr.y;
    boss_lvl1.HealthCurr = BOSS_HEALTH;
    boss_lvl1.Scale = 150.0f;

    bossActive = false;
    counter_rush = 1;
    counter = 1;
    controller = 5;
    state = -1;
    randomNumber = 0;
    
    rush = false;
    movingLeft = false;
    step = 0;
    
    cooldown = 3.0f;
    deathCounter = 3.0f;
    coolDownState = false;

    returnPoint = 1600;
    strobe = 1;
}

void boss_load()
{
 
    boss_lvl1.Direction = Left;
    AEGfxStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

    Global::GetInstance()->vertex_Boss = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_Boss, "Failed to create mesh for the boss!!");

    // Texture 1: From file
    Global::GetInstance()->texture_boss = AEGfxTextureLoad("Resources\\Player\\PlayerFrame1.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_boss, "Failed to create the BOSS!!");

    /* Frame 2 of boss*/

    AEGfxStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

    Global::GetInstance()->vertex_Boss1 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_Boss1, "Failed to create mesh for the boss1!!");

    // Texture 1: From file
    Global::GetInstance()->texture_boss1 = AEGfxTextureLoad("Resources\\Player\\PlayerFrame2.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_boss1, "Failed to create the BOSS1!!");


    /********************************************************************************************/


    //Bullet texture
    AEGfxStart();
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_Bullet = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_Bullet, "Failed to create mesh for the boss bullet!!");


    //// Texture 1: From file
    //Global::GetInstance()->texture_Bullet = AEGfxTextureLoad("Resources\\fx\\feather.png");
    //AE_ASSERT_MESG(Global::GetInstance()->texture_Bullet, "Failed to create the BOSS bullet!!");

    /*******************************************************************************************/

   
}

void LoadBullet()
{
    bullet = (BossBullet*)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(BossBullet));
    sGameObjNum = 0;

    AEGfxMeshStart();

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

    AEGfxTriAdd(
        -0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);

    bulletObj.pMesh = AEGfxMeshEnd();
    AE_ASSERT_MESG(bulletObj.pMesh, "Failed to create bullet mesh!!");

    // Texture 1: From file
    bulletObj.pText = AEGfxTextureLoad("Resources\\fx\\feather.png");
    AE_ASSERT_MESG(bulletObj.pText, "Failed to create the BOSS bullet!!");

}

BossBullet* CreateBullet(float scale, AEVec2* pPos, AEVec2* pVel, float dir)
{
    // loop through the object instance list to find a non-used object instance
    for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
    {
        BossBullet* pInst = bullet + i;
        AEVec2 zero;
        AEVec2Zero(&zero);

        // check if current instance is not used
        if (pInst->flag == 0)
        {
            // it is not used => use it to create the new instance
            pInst->pObject = &bulletObj;
            pInst->flag = FLAG_ACTIVE | FLAG_VISIBLE;
            pInst->scale = scale;
            pInst->posCurr = pPos ? *pPos : zero;
            pInst->velCurr = pVel ? *pVel : zero;
            pInst->dirCurr = dir;
            pInst->lifetime = 0;

            // return the newly created instance
            return pInst;
        }
    }
    return 0;
}




void UpdateBossBullet()
{
    BossBullet* pInst;
    for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
    {
        pInst = bullet + i;
        // skip non-active object
        if ((pInst->flag & FLAG_ACTIVE) == 0)
            continue;
        // update all active particles positions accordingly
        pInst->posCurr.x += bullet->velCurr.x * Global::GetInstance()->g_dt;
        //    pInst->posCurr.y += pInst->velCurr.y * Global::GetInstance()->g_dt;

            // bounding rectangle of every active instances
        pInst->boundingBox.min.x = -0.5f * pInst->scale + pInst->posCurr.x;
        pInst->boundingBox.max.x = 0.5f * pInst->scale + pInst->posCurr.x;
        pInst->boundingBox.min.y = -0.5f * pInst->scale + pInst->posCurr.y;
        pInst->boundingBox.max.y = 0.5f * pInst->scale + pInst->posCurr.y;

        //Computing the transformation matrices of the game object instances
        for (unsigned int j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
        {
            AEMtx33 scale, trans;
            pInst = bullet + j;

            // skip non-active object instances
            if ((pInst->flag & FLAG_ACTIVE) == 0)
                continue;

           // AEMtx33Scale(&scale, pInst->scale, pInst->scale);
            //AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
            //AEMtx33Concat(&pInst->transform, &trans, &scale);

            // Compute the scaling matrix
            AEMtx33Scale(&scale, pInst->scale, pInst->scale);
            // Compute the translation matrix
            AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
            // Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
            AEMtx33Concat(&trans, &trans, &scale);
            pInst->transform = trans;
        }
    }
}

void update_boss()
{

    //If boss have yet to come out, heath must remain as full
    if (Global::GetInstance()->boss_skills == false)
    {
        Global::GetInstance()->boss_lvll1.HealthCurr = BOSS_HEALTH;
    }

    //Look at the player
    if (Global::GetInstance()->player.posCurr.x > boss_lvl1.posCurr.x)
    {
        boss_dir = 1;
    }
    else
    {
        boss_dir = -1;
    }

    if (boss_lvl1.Alive)
    {
        //run the sequence
    }

    dist =(int) (boss_lvl1.posCurr.x - Global::GetInstance()->player.posCurr.x);
    if (Global::GetInstance()->player.posCurr.x >= 500.0f)
        bossActive = true;

    //Collision
    boss_lvl1.boundingBox.min.x = -0.5f * boss_lvl1.Scale + boss_lvl1.posCurr.x;
    boss_lvl1.boundingBox.max.x = 0.5f * boss_lvl1.Scale + boss_lvl1.posCurr.x ;
    boss_lvl1.boundingBox.min.y = -0.5f * (boss_lvl1.Scale) + boss_lvl1.posCurr.y ;
    boss_lvl1.boundingBox.max.y = 0.5f *(boss_lvl1.Scale) + boss_lvl1.posCurr.y;
    boss_lvl1.boundingBox.max.y -= 100;

    for (auto& obj : Global::GetInstance()->Ally_List)
    {
        if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_DUCK)
        {
            if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr,
                boss_lvl1.boundingBox, boss_lvl1.velCurr))
            {
               // obj->HealthCurr -= 10;
               // boss_lvl1.HealthCurr -= 10;
              //  printf("HIT!\n");
                obj->posCurr.x -= 10;
            }
        }

        if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_PENGIUN)
        {
            if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr,
                boss_lvl1.boundingBox, boss_lvl1.velCurr))
            {
              // obj->HealthCurr -= 10;
              // boss_lvl1.HealthCurr -= 10;
              //  printf("HIT!\n");
                obj->posCurr.x -= 10;
            }
        }

        if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_OSTRICH)
        {
            if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr,
                boss_lvl1.boundingBox, boss_lvl1.velCurr))
            {
                 //obj->HealthCurr -= 10;
                 //boss_lvl1.HealthCurr -= 10;
                 //printf("HIT!\n");
                 obj->posCurr.x -= 10;
            }
        }

        if (boss_lvl1.HealthCurr <= 0)
        {
            //Global::GetInstance()->boss_skills = false;
            //Start death shake
            boss_lvl1.Alive = false;
            //boss_lvl1.posCurr.y = Global::GetInstance()->player.posCurr.y + 500; //teleport boss when he dead
        }

    }

   

   
    //AEGfxPrint(Global::GetInstance()->fontId, strBuffer2, 0.0f, 0.80f, 1.0f, 1.0f, 1.f, 1.f);

    /* 
    
    When boss dies,
    It will have strob effect, move left abit, right abit, fly up and dissapear!
    
    */

    if (!boss_lvl1.Alive) // if boss dead start death sequence
    {
        //printf("Death Seq INIT!\n");
        deathCounter -= Global::GetInstance()->g_dt;
        strobe -= Global::GetInstance()->g_dt;
    }

    if (strobe < 0.5 && !boss_lvl1.Alive)
    {
        strobe = 1;
    }

    if (strobe > 0.7 && !boss_lvl1.Alive)
    {
        coolDownState = true;
    }

    if (strobe < 0.7 && !boss_lvl1.Alive)
    {
        coolDownState = false;
    }

    if (deathCounter > 0 && !boss_lvl1.Alive)
    {
        
        if ((int)deathCounter % 2)
        {
            boss_lvl1.posCurr.x -= 1 ;
        }
        
        else
        {
            boss_lvl1.posCurr.x += 1;
        }
    }

    if (deathCounter < 0 && deathCounter > -5)
    {
        boss_lvl1.posCurr.y += 11 * Global::GetInstance()->g_dt;
    }

    if (deathCounter<-5)
    {
        boss_lvl1.posCurr.y = Global::GetInstance()->pHero_Tower->posCurr.y + 100; // dissapear boss
        boss_lvl1.posCurr.y = Global::GetInstance()->pHero_Tower->posCurr.x; // dissapear boss
        Global::GetInstance()->boss_skills = false;
    }

    //cool debug feature to bring boss to you!
    if (AEInputCheckCurr(AEVK_T))
    {
        boss_lvl1.posCurr.x = 500;
        returnPoint = 500;
        //boss_lvl1.HealthCurr -= 50;
    }

    if (AEInputCheckCurr(AEVK_Y))
    {
        Global::GetInstance()->player.HealthCurr = 100000;
        Global::GetInstance()->player.mana = 100000;
        //boss_lvl1.HealthCurr -= 50;
    }

    if (AEInputCheckCurr(AEVK_U))
    {
        boss_lvl1.HealthCurr -= 50;
    }

    //Collision of bullet to anything else

    for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
    {
        BossBullet* pInst;
        pInst = bullet + i;

        // skip non-active object
        if ((pInst->flag & FLAG_ACTIVE) == 0)
            continue;

        //bullet to player
        if (CollisionIntersection_RectRect(Global::GetInstance()->player.boundingBox,
            Global::GetInstance()->player.velCurr, pInst->boundingBox, pInst->velCurr))
        {
            //obj->HealthCurr -= 1;
            Global::GetInstance()->player.posCurr.x -= 50 * Global::GetInstance()->g_dt;
            Global::GetInstance()->player.HealthCurr -= 50;

            if (Global::GetInstance()->volumeGlobal > 0)
            {
                PlayAudio(SOUNDTYPE::ST_PLAYERDAMAGED);
                SetSFXVolume(Global::GetInstance()->volumeGlobal);
            }
           
            //printf("player eating bullet!\n");
            DestroyBossBullet(pInst);
        }

        //Bullet to allies
        for (auto& obj : Global::GetInstance()->Ally_List)
        {
            if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_DUCK)
            {
                if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr,
                    pInst->boundingBox, pInst->velCurr)) //pInst is the buller
                {
                    //obj->posCurr.x -= 100;
                    obj->HealthCurr -= 50;
                    
                    if (Global::GetInstance()->volumeGlobal > 0)
                    {
                        PlayAudio(SOUNDTYPE::ST_MONSTERDAMAGED);
                        SetSFXVolume(Global::GetInstance()->volumeGlobal);
                    }

                    DestroyBossBullet(pInst);
                   // printf("HIT!\n");
                }
            }

            if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_PENGIUN)
            {
                if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr,
                    pInst->boundingBox, pInst->velCurr)) //pInst is the buller
                {
                    //obj->posCurr.x -= 100;
                    obj->HealthCurr -= 50;
                    
                    if (Global::GetInstance()->volumeGlobal > 0)
                    {
                        PlayAudio(SOUNDTYPE::ST_MONSTERDAMAGED);
                        SetSFXVolume(Global::GetInstance()->volumeGlobal);
                    }
                    DestroyBossBullet(pInst);
                   // printf("HIT!\n");
                }
            }

            if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_OSTRICH)
            {
                if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr,
                    pInst->boundingBox, pInst->velCurr)) //pInst is the buller
                {
                    obj->HealthCurr -= 50;
                    //obj->posCurr.x -= 100;
                    if (Global::GetInstance()->volumeGlobal > 0)
                    {
                        PlayAudio(SOUNDTYPE::ST_MONSTERDAMAGED);
                        SetSFXVolume(Global::GetInstance()->volumeGlobal);
                    }
                    DestroyBossBullet(pInst);
                    //printf("HIT!\n");
                }
            }

            if (obj->pObject->type == TYPE_OBJECT::TYPE_OBJECT_MINER)
            {
                if (CollisionIntersection_RectRect(obj->boundingBox, obj->velCurr,
                    pInst->boundingBox, pInst->velCurr)) //pInst is the buller
                {
                    obj->HealthCurr -= 50;
                    if (Global::GetInstance()->volumeGlobal > 0)
                    {
                        PlayAudio(SOUNDTYPE::ST_MONSTERDAMAGED);
                        SetSFXVolume(Global::GetInstance()->volumeGlobal);
                    }
                    DestroyBossBullet(pInst);
                    //printf("HIT!\n");
                }
            }
        }
    }
    
   
    //player collision
    if (CollisionIntersection_RectRect(Global::GetInstance()->player.boundingBox,
        Global::GetInstance()->player.velCurr, boss_lvl1.boundingBox, boss_lvl1.velCurr))
    {//player push back
        //obj->HealthCurr -= 1;
        Global::GetInstance()->player.posCurr.x -= 300 * Global::GetInstance()->g_dt;
        CameraPos(&Global::GetInstance()->camera, Global::GetInstance()->player.posCurr.x, 0.0f);
        //printf("HIT!\n");
    }

    //player attacks boss

#if 0 
    
    /* Random behaviour function put on hold due to not niceness */

    // choose one number 
    if (needNum)
    {
        randomNumber = rand() % 4;
       // printf("New number is: %d", randomNumber);
        needNum = false; // dont need new number for now;
    }
#endif

   // printf("Rand Val for today is: %d\n", randomNumber);

    // do action for 3 seconds
    counter -= Global::GetInstance()->g_dt;
    float dt = Global::GetInstance()->g_dt;

    if (bossActive)    // if player nearby set boss to be active
    {
        //printf("Boss is active!\n");
    }

    else
    {
        bossActive = false;
    }



    /*

        Intro:
        1. Push player and minion back for 3 seconds  (RUSH MODE)

        During game:
        2. Social distance until x = 1800 or more


        3. Shoot (Target Minions) for 5 seconds
                if player get too close, less than 350,
                        Push back the player

        Dead:
        Boss become big and explode

        ------------------

        BossFacts: Boss is immune to all minion attacks. Based on real life

    */

    //printf("BossX: %f\n",boss_lvl1.posCurr.x);

    //Rush
    if (rush == false && Global::GetInstance()->boss_skills == true)
    {
        counter_rush -= Global::GetInstance()->g_dt;
    }

    // rush as long as counter > 0
    if (counter_rush > 0)
    {
        boss_lvl1.posCurr.x -= 300 * dt;
        isMoving = true;
        //printf("Rushing\n");
    }

    else
    {
        rush = true; //rush for legit 3 seconds
    }

    // if (counter > 0 && rush )
    if (rush && boss_lvl1.Alive)
    {
       
        //Social distancing
        if (boss_lvl1.posCurr.x < returnPoint) 
        {
           boss_lvl1.posCurr.x += 300 * dt; //move right till reach pos
           //printf("Backing Up!\n");

           isMoving = true;
        }

        else if (boss_lvl1.posCurr.x < returnPoint)
        {
            boss_lvl1.posCurr.x -= 300 * dt; //move right till reach pos
            //printf("Backing Up!\n");

            isMoving = true;
        }

        else if (boss_lvl1.posCurr.x >= 2000)
        {
            boss_lvl1.posCurr.x -= 100 * dt; //move right till reach pos
            //printf("Backing Up!\n");

            isMoving = true;
        }

        else
        {
            AEVec2 pPos;
            pPos = { 0,0 };
            pPos.x = boss_lvl1.posCurr.x - 50;
            pPos.y = boss_lvl1.posCurr.y - 50;

            AEVec2 pVel = { 100,0 };
            pVel.x = boss_lvl1.velCurr.x;
            pVel.y = boss_lvl1.velCurr.y;

            if (!fire && !movingLeft && !coolDownState && boss_lvl1.Alive)
            {
                isMoving = false;
                fire = true;
                
                CreateBullet(1.5, &pPos, &pVel, 1);
                //printf("SHOOTING!\n");
            }

            // if player gets too close, push him away for 3 seconds
            if (dist < 300)
            {
                rush = false;
                counter_rush = 2;
                //printf("Too close bro!!\n");
            }

            
        }

       

        //printf("Dist: %d\n", dist);
#if 0
        //printf("boss has rushed!\n");
        if (randomNumber == 0)
        {
            if (boss_lvl1.posCurr.x < Global::GetInstance()->pHero_Tower->posCurr.x + 350)
            {
                boss_lvl1.posCurr.x += 100 * dt; // move right     
           //     printf("Right!\n");
                movingLeft = false;
            }
        }

        /***************************************if too right, move left********************************************************************/
        if (randomNumber == 1 || boss_lvl1.posCurr.x < Global::GetInstance()->pHero_Tower->posCurr.x + 350 )
        {
            boss_lvl1.posCurr.x += 100 * dt; // move right     
       //     printf("Right!\n");
            movingLeft = false;
        }

        else
        {
            boss_lvl1.posCurr.x -= 100 * dt;  // move left 
            movingLeft = true;
         //   printf("Right limited!\n");
        }

        /***************************************if too left, move right********************************************************************/
        if (randomNumber == 2 || boss_lvl1.posCurr.x >= 1900 )
        {  
            boss_lvl1.posCurr.x -= 100 * dt;  // move left 
            movingLeft = true;
         //   printf("Left!\n");
        }

        else
        {
            boss_lvl1.posCurr.x += 100 * dt; // move right  
            movingLeft = false;
          //  printf("Left limited!\n");
        }

        /***************************************if player being a coward, shoot him in the face********************************************/

        if (dist > 300 ) 
        {
            //shoot
            //float scale = 0.5f;

            AEVec2 pPos;
            pPos = { 0,0 };
            pPos.x = boss_lvl1.posCurr.x - 50;
            pPos.y = boss_lvl1.posCurr.y - 50;

            AEVec2 pVel = { 100,0 };
            pVel.x = boss_lvl1.velCurr.x;
            pVel.y = boss_lvl1.velCurr.y;

            if (!fire && !movingLeft)
            {
                fire = true;
                CreateBullet(1.5, &pPos, &pVel, 1);
            //    printf("SHOOTING!\n");
            }


        }

        UpdateBossBullet();
#endif

    }

    // Keep Bullet Moving when needed
    
    if (Global::GetInstance()->boss_skills)
    {
        UpdateBossBullet();
    }

    cooldown -= Global::GetInstance()->g_dt;
    //printf("Cool Down: %f\n",cooldown);

    // Fire rate controller or comptroller heh
    if (counter <= 0 && !coolDownState)
    {
        counter = 1;
       // needNum = true; // look for next lucky number 
        fire = false;
       // printf("Fire\n");
    }
   
    //Start cool down after 3 seconds
    if (boss_lvl1.Alive)
    {
        if (cooldown < 0)
        {
            coolDownState = true;
        }

        //Reset after 3 seconds
        if (cooldown < -7)
        {
            cooldown = 3;
        }

        //3 sec of no cooldown
        if (cooldown > 0)
        {
            coolDownState = false;
        }
    }
   


}

void draw_boss()
{

#if 0 
    //you must know the syntax chose one lmao
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
    AEGfxSetPosition(boss_lvl1.posCurr.x, boss_lvl1.posCurr.y);
    AEGfxTextureSet(Global::GetInstance()->texture_boss, 1.0f, 1.0f);
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);

    AEMtx33 Scale, Trans, Transform;
    //The following code will adjust the size of the buttons
    AEMtx33Scale(&Scale, boss_lvl1.Scale, boss_lvl1.Scale);// something wrong

    AEMtx33Trans(&Trans, boss_lvl1.posCurr.x, boss_lvl1.posCurr.y);
    AEMtx33Concat(&Transform, &Trans, &Scale);
    boss_lvl1.transform = Transform;
    AEGfxSetTransform(boss_lvl1.transform.m);


    AEGfxMeshDraw(Global::GetInstance()->vertex_Boss, AE_GFX_MDM_TRIANGLES);

    AEGfxSetTransparency(1.0f);

#endif


    AEMtx33 scale1, trans1, transform1;

    timerAnime -= Global::GetInstance()->g_dt;

    if (timerAnime < -0.5)
    {
        timerAnime = 0.5;
    }

    if (isMoving)
    {
        if (timerAnime > 0)
        {

            // Drawing button3 - (first) - No tint
            AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
            //blend to bg
            AEGfxSetBlendMode(AE_GFX_BM_BLEND);
            AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
            // tint
            if (coolDownState)
            {
                AEGfxSetTintColor(0.0f, 0.0f, 1.0f, 1.0f);//blue
            }
            else
            {
                 AEGfxSetTintColor(1.0f, 0.0f, 0.0f, 1.0f);//red
            }
           

            // Set texture
            AEGfxTextureSet(Global::GetInstance()->texture_boss, 0.0f, 0.0f);

            //The following code will adjust the size of the buttons
            AEMtx33Scale(&scale1, -boss_lvl1.Scale, boss_lvl1.Scale);
            AEMtx33Trans(&trans1, boss_lvl1.posCurr.x, boss_lvl1.posCurr.y);
            AEMtx33Concat(&transform1, &trans1, &scale1);
            AEGfxSetTransform(transform1.m);

            // Drawing the mesh (list of triangles)
            AEGfxMeshDraw(Global::GetInstance()->vertex_Boss, AE_GFX_MDM_TRIANGLES);

        }
        else
        {

            // Drawing button3 - (first) - No tint
            AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
            //blend to bg
            AEGfxSetBlendMode(AE_GFX_BM_BLEND);
            AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
            if (coolDownState)
            {
                AEGfxSetTintColor(0.0f, 0.0f, 1.0f, 1.0f); // blue
            }
            else
            {
                AEGfxSetTintColor(1.0f, 0.0f, 0.0f, 1.0f);  // red
            }

            // Set texture
            AEGfxTextureSet(Global::GetInstance()->texture_boss1, 0.0f, 0.0f);

            //The following code will adjust the size of the buttons
            AEMtx33Scale(&scale1, -boss_lvl1.Scale, boss_lvl1.Scale);
            AEMtx33Trans(&trans1, boss_lvl1.posCurr.x, boss_lvl1.posCurr.y);
            AEMtx33Concat(&transform1, &trans1, &scale1);
            AEGfxSetTransform(transform1.m);

            // Drawing the mesh (list of triangles)
            AEGfxMeshDraw(Global::GetInstance()->vertex_Boss1, AE_GFX_MDM_TRIANGLES);

        }
    }

    else
    {
        // Drawing button3 - (first) - No tint
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        //blend to bg
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);

        if (coolDownState)
        {
            AEGfxSetTintColor(0.0f, 0.0f, 1.0f, 1.0f); //blue
        }
        else
        {
            AEGfxSetTintColor(1.0f, 0.0f, 0.0f, 1.0f);  //red
        }

        // Set texture
        AEGfxTextureSet(Global::GetInstance()->texture_boss1, 0.0f, 0.0f);

        //The following code will adjust the size of the buttons
        AEMtx33Scale(&scale1, -boss_lvl1.Scale, boss_lvl1.Scale);
        AEMtx33Trans(&trans1, boss_lvl1.posCurr.x, boss_lvl1.posCurr.y);
        AEMtx33Concat(&transform1, &trans1, &scale1);
        AEGfxSetTransform(transform1.m);

        // Drawing the mesh (list of triangles)
        AEGfxMeshDraw(Global::GetInstance()->vertex_Boss1, AE_GFX_MDM_TRIANGLES);

        
    }
    
    if (Global::GetInstance()->boss_skills)
    {
        DrawBossBullet();
    }
    

    // Set Transparency
   // AEGfxSetTransparency(1.0f);

   // //Debug Distance
   // char strBuffer[1000];
   // memset(strBuffer, 0, 100 * sizeof(char));
   // sprintf_s(strBuffer, "scale: %f", boss_lvl1.Scale);

   // AEGfxSetBlendMode(AE_GFX_BM_BLEND);
   // AEGfxPrint(Global::GetInstance()->fontId, strBuffer, 0.0f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f);

   // //Boss x
   // char strBuffer3[100];
   // memset(strBuffer3, 0, 100 * sizeof(char));
   // sprintf_s(strBuffer3, "Boss X: %f", boss_lvl1.posCurr.x);

   // AEGfxSetBlendMode(AE_GFX_BM_BLEND);
   // AEGfxPrint(Global::GetInstance()->fontId, strBuffer3, 0.0f, 0.70f, 1.0f, 1.0f, 1.0f, 1.0f);

   // //pplayer x
   // char strBuffer2[100];
   // memset(strBuffer2, 0, 100 * sizeof(char));
   // sprintf_s(strBuffer2, "player X: %f", Global::GetInstance()->player.posCurr.x);

   // AEGfxSetBlendMode(AE_GFX_BM_BLEND);
   // AEGfxPrint(Global::GetInstance()->fontId, strBuffer2, 0.0f, 0.60f, 1.0f, 1.0f, 1.0f, 1.0f);

   // //Debug Chicken Position
   // char strBuffer1[100];
   // memset(strBuffer1, 0, 100 * sizeof(char));
   // sprintf_s(strBuffer1, "Boss scale: %f", boss_lvl1.Scale);

   // AEGfxSetBlendMode(AE_GFX_BM_BLEND);
   //// AEGfxPrint(Global::GetInstance()->fontId, strBuffer1, 0.0f, 0.60f, 1.0f, 1.0f, 1.f, 1.f);

    
}

void DrawBossBullet()
{
    // draw all object instances in the list
    for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
    {
        BossBullet* pInst = bullet + i;

        // skip non-active object
        if ((pInst->flag & FLAG_ACTIVE) == 0)
            continue;
        AEGfxTextureSet(pInst->pObject->pText, 0, 0);
        // Set the current object instance's transform matrix using "AEGfxSetTransform"
        AEGfxSetTransform(pInst->transform.m);
        // Draw the shape used by the current object instance using "AEGfxMeshDraw"

        AEGfxSetTintColor(1.0f, 0.0f, 0.0f, 1.0f);    
        AEGfxMeshDraw(Global::GetInstance()->vertex_Bullet, AE_GFX_MDM_TRIANGLES);
    }
}

void UnloadBullet()
{
    AEGfxMeshFree(bulletObj.pMesh);
    AEGfxTextureUnload(bulletObj.pText);
   // AEGfxTextureUnload(Global::GetInstance()->texture_Bullet);
    free(bullet);
    //free(Timer);
}

void free_boss()
{

    AEGfxMeshFree(Global::GetInstance()->vertex_Boss);
    AEGfxMeshFree(Global::GetInstance()->vertex_Boss1);
    AEGfxMeshFree(Global::GetInstance()->vertex_Bullet);
    //FreeBossBullet();

}

void unload_boss()
{
   AEGfxTextureUnload(Global::GetInstance()->texture_boss);
   AEGfxTextureUnload(Global::GetInstance()->texture_boss1);
   //UnloadBullet();
}

void DestroyBossBullet(BossBullet* pInst)
{
    // if particle is destroyed before just return
    if (pInst->flag == 0)
        return;

    // zero out the flag
    pInst->flag = 0;
}

void FreeBossBullet()
{
    // kill all object in the list
    for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
    {
        DestroyBossBullet(bullet + i);
    }
}
