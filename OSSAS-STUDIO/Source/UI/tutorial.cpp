/******************************************************************************/
/*!
\file		tutorial.cpp
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
\date   	20 Jun, 2020
\brief      To be used in GS_LEVEL1 only

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "AEEngine.h"
#include "GameState/Global.h"
#include "UI/tutorial.h"
#include "Player/Player.h"
#include "Audio/AudioManager.h"
#include "Gamestate/GameStateMgr.h"
#include "UI/ui.h" //remove later

unsigned int selection = 0;


// for options
// variable declarations to be used only for mainmenu state
AEGfxTexture* options_page = nullptr;
AEGfxVertexList* options = nullptr;
AEGfxTexture* leftbtn1 = nullptr;
AEGfxVertexList* leftbtnMesh1 = nullptr;
AEGfxTexture* checkbox = nullptr;
AEGfxVertexList* checkboxMesh = nullptr;
AEGfxTexture* checkbox1 = nullptr;
AEGfxTexture* option_back = nullptr;
AEGfxVertexList* volumeSlider = nullptr;
AEGfxTexture* volumeSliderTex = nullptr;

s32 mouse_y4 = 0, mouse_x4 = 0;
bool leftbtn_hover1;
int selection_checkbox;
bool* fullscreen_check = &Global::GetInstance()->fullscreen_check;
bool* mute_check = &Global::GetInstance()->mute_check;
float sliderx, slidery;

void tutorial_load ()
{
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slide1 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slide1, "Failed to create mesh for slide1!!");

    // Texture 1: From file
    Global::GetInstance()->texture_slide1 = AEGfxTextureLoad("Resources\\UI\\tutorial\\slide1.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slide1, "Failed to create the slide1 texture!!");

    /// <summary>
    /// slide 2
    /// </summary>
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);
    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slide2 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slide2, "Failed to create mesh for slide2!!");

    // Texture 1: From file
    Global::GetInstance()->texture_slide2 = AEGfxTextureLoad("Resources\\UI\\tutorial\\slide2.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slide2, "Failed to create the slide2 texture!!");

    //slide 3
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slide3 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slide3, "Failed to create mesh for slide3!!");

    // Texture 1: From file
    Global::GetInstance()->texture_slide3 = AEGfxTextureLoad("Resources\\UI\\tutorial\\slide3.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slide3, "Failed to create the slide3 texture!!");

    //slide 4
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slide4 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slide4, "Failed to create mesh for slide4!!");

    // Texture 1: From file
    Global::GetInstance()->texture_slide4 = AEGfxTextureLoad("Resources\\UI\\tutorial\\slide4.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slide4, "Failed to create the slide4 texture!!");

    //slide 5
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slide5 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slide5, "Failed to create mesh for slide5!!");

    // Texture 1: From file
    Global::GetInstance()->texture_slide5 = AEGfxTextureLoad("Resources\\UI\\tutorial\\slide5.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slide5, "Failed to create the slide5 texture!!");

    //slide 6
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slide6 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slide6, "Failed to create mesh for slide6!!");

    // Texture 1: From file
    Global::GetInstance()->texture_slide6 = AEGfxTextureLoad("Resources\\UI\\tutorial\\slide6.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slide6, "Failed to create the slide6 texture!!");

    //slide 7
    AEGfxTriAdd(
      -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
      30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
      -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
      30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
      30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
      -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slide7 = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slide7, "Failed to create mesh for slide7!!");

    // Texture 1: From file
    Global::GetInstance()->texture_slide7 = AEGfxTextureLoad("Resources\\UI\\tutorial\\slide7.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slide7, "Failed to create the slide6 texture!!");

    //slide Regen
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slideRegen = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slideRegen, "Failed to create mesh for slideRegen!");

    // Texture 1: From file
    Global::GetInstance()->texture_slideRegen = AEGfxTextureLoad("Resources\\UI\\tutorial\\basic.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slideRegen, "Failed to create the slideRegen texture!!");

    //slide Basic
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slideBasic = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slideBasic, "Failed to create mesh for slideBasic!");

    // Texture 1: From file
    Global::GetInstance()->texture_slideBasic = AEGfxTextureLoad("Resources\\UI\\tutorial\\regen.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slideBasic, "Failed to create the slideBasic texture!!");

    //slide adv
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_slideAdv = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_slideAdv, "Failed to create mesh for slideAdv!");

    // Texture 1: From file
    Global::GetInstance()->texture_slideAdv = AEGfxTextureLoad("Resources\\UI\\tutorial\\adv.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_slideAdv, "Failed to create the slideBasic texture!!");


    //slide BOSS
    AEGfxTriAdd(
        -30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
        30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
        -30.0f, 30.0f, 0x0000FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
        30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
        -30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

    Global::GetInstance()->vertex_bossTut = AEGfxMeshEnd();
    AE_ASSERT_MESG(Global::GetInstance()->vertex_bossTut, "Failed to create mesh for bossTut!");

    // Texture 1: From file
    Global::GetInstance()->texture_bossTut = AEGfxTextureLoad("Resources\\UI\\tutorial\\bossTut.png");
    AE_ASSERT_MESG(Global::GetInstance()->texture_bossTut, "Failed to create the bossTut texture!!");

    selection = slide1;
}

void tutorial_draw(AEGfxTexture* mainMenu, AEGfxVertexList* menu)
{
  AEMtx33 Scale, Trans, Transform;

  AEGfxSetCamPosition(0.0f, 0.0f);
  // Drawing mainMenu
  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
  AEGfxSetBlendMode(AE_GFX_BM_NONE);
  AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
  // Set position for object 1
  AEGfxSetPosition(0.0f, 0.0f);
  // set mainmenu texture
  AEGfxTextureSet(mainMenu, 1.0f, 1.0f);
  // No tint
  AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

  ////Drawing the mesh (list of triangles)
 //The following code will adjust the size of the buttons
  //AEMtx33Scale(&Scale, 13.0f, 13.0f);
  AEMtx33Scale(&Scale, 40/3.0f, 10.0f);
  AEMtx33Trans(&Trans, 0.0f, 0.0f);
  AEMtx33Concat(&Transform, &Trans, &Scale);
  AEGfxSetTransform(Transform.m);

  // render menu image
  AEGfxMeshDraw(menu, AE_GFX_MDM_TRIANGLES);

  //DrawMouseDebug(mouse_x4, mouse_y4); // remove later
}

void tutorial_update()
{
   AEInputGetCursorPosition(&mouse_x4, &mouse_y4);

  /*if (AEInputCheckReleased(AEVK_RETURN) || AEInputCheckReleased(AEVK_LBUTTON))
  {
    if (Global::GetInstance()->IsPaused)
        ++selection;
  }*/

  if (AEInputCheckReleased(AEVK_BACK))
  {
      selection = slideNone;
  }

  switch (selection)
  {
  case slide1:
    tutorial_draw(Global::GetInstance()->texture_slide1, Global::GetInstance()->vertex_slide1);
    char strBuffer1[100];
    memset(strBuffer1, 0, 100 * sizeof(char));
    sprintf_s(strBuffer1, "Press ENTER!");

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxPrint(Global::GetInstance()->fontId, strBuffer1, 0.52f, -0.250f, 1.0f, 1.0f, 1.f, 1.f);
    skipORreturn();
    break;

  case slide2:
    tutorial_draw(Global::GetInstance()->texture_slide2, Global::GetInstance()->vertex_slide2);
    char strBuffer2[100];
    memset(strBuffer2, 0, 100 * sizeof(char));
    sprintf_s(strBuffer2, "Press ENTER!");

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxPrint(Global::GetInstance()->fontId, strBuffer2, 0.62f, -0.300f, 1.0f, 1.0f, 1.f, 1.f);
    skipORreturn();
    break;

  case slide3:
    tutorial_draw(Global::GetInstance()->texture_slide3, Global::GetInstance()->vertex_slide3);
    char strBuffer3[100];
    memset(strBuffer3, 0, 100 * sizeof(char));
    sprintf_s(strBuffer3, "Press ENTER!");

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxPrint(Global::GetInstance()->fontId, strBuffer3, 0.62f, -0.300f, 1.0f, 1.0f, 1.f, 1.f);
    skipORreturn();
    break;

  case slide4:
    tutorial_draw(Global::GetInstance()->texture_slide4, Global::GetInstance()->vertex_slide4);
    char strBuffer4[100];
    memset(strBuffer4, 0, 100 * sizeof(char));
    sprintf_s(strBuffer4, "Press ENTER!");

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxPrint(Global::GetInstance()->fontId, strBuffer4, 0.62f, -0.300f, 1.0f, 1.0f, 1.f, 1.f);
    skipORreturn();
    break;

  case slide5:
    tutorial_draw(Global::GetInstance()->texture_slide5, Global::GetInstance()->vertex_slide5);
    char strBuffer5[100];
    memset(strBuffer5, 0, 100 * sizeof(char));
    sprintf_s(strBuffer5, "Press ENTER!");

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxPrint(Global::GetInstance()->fontId, strBuffer5, 0.62f, -0.300f, 1.0f, 1.0f, 1.f, 1.f);
    skipORreturn();
    break;

  case slide6:
    tutorial_draw(Global::GetInstance()->texture_slide6, Global::GetInstance()->vertex_slide6);
    char strBuffer6[100];
    memset(strBuffer6, 0, 100 * sizeof(char));
    sprintf_s(strBuffer6, "Press ENTER!");

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxPrint(Global::GetInstance()->fontId, strBuffer6, 0.62f, -0.200f, 1.0f, 1.0f, 1.f, 1.f);
    skipORreturn();
    break;

  case slide7:
    tutorial_draw(Global::GetInstance()->texture_slide7, Global::GetInstance()->vertex_slide7);
    char strBuffer7[100];
    memset(strBuffer7, 0, 100 * sizeof(char));
    sprintf_s(strBuffer7, "Press ENTER!");

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxPrint(Global::GetInstance()->fontId, strBuffer7, 0.62f, -0.300f, 1.0f, 1.0f, 1.f, 1.f);
    //selection = slideRegenWait;
    //Global::GetInstance()->IsPaused = false;
    skipORreturn();
    break;
  
  case slideBoss:
      tutorial_draw(Global::GetInstance()->texture_bossTut, Global::GetInstance()->vertex_bossTut);
      char strBuffer8[100];
      memset(strBuffer8, 0, 100 * sizeof(char));
      sprintf_s(strBuffer8, "Press ENTER!");

      AEGfxSetBlendMode(AE_GFX_BM_BLEND);
      AEGfxPrint(Global::GetInstance()->fontId, strBuffer8, 0.62f, -0.300f, 1.0f, 1.0f, 1.f, 1.f);
      //selection = slideRegenWait;
      //Global::GetInstance()->IsPaused = false;
      skipORreturn();
      break;

  case transitionToGame:
    selection = slideRegenWait;
    Global::GetInstance()->IsPaused = false;
    break;

  case slideRegenWait:
    if (Global::GetInstance()->player.mana > 3)
    {
      selection = slideRegen;
    }
    break;

  case slideRegen:
    Global::GetInstance()->IsPaused = true;
    tutorial_draw(Global::GetInstance()->texture_slideRegen, Global::GetInstance()->vertex_slideRegen);
    CameraPos(&Global::GetInstance()->camera, Global::GetInstance()->player.posCurr.x, 0.0f);
    skipORreturn();
    break;

  case slideBasicWait:
    Global::GetInstance()->IsPaused = false;
    if (Global::GetInstance()->player.mana > 10)
    {
      Global::GetInstance()->IsPaused = true;
      selection = slideBasic;
    }
    break;

  case slideBasic:
    tutorial_draw(Global::GetInstance()->texture_slideBasic, Global::GetInstance()->vertex_slideBasic);
    CameraPos(&Global::GetInstance()->camera, Global::GetInstance()->player.posCurr.x, 0.0f);
    skipORreturn();
    break;

  case slideAdvWait:
    Global::GetInstance()->IsPaused = false;
    if (Global::GetInstance()->player.mana > 30)
    {
      Global::GetInstance()->IsPaused = true;
      selection = slideAdv;
    }
    break;

  case slideAdv:
    tutorial_draw(Global::GetInstance()->texture_slideAdv, Global::GetInstance()->vertex_slideAdv);
    CameraPos(&Global::GetInstance()->camera, Global::GetInstance()->player.posCurr.x, 0.0f);
    skipORreturn();
    break;

  case slideNone:
    Global::GetInstance()->IsPaused = false;
    Global::GetInstance()->tutorial_done = true;
    break;

  default:
    Global::GetInstance()->IsPaused = false;
    Global::GetInstance()->tutorial_done = true;
    break;
  }

  if (AEInputCheckReleased(AEVK_RETURN) || AEInputCheckReleased(AEVK_LBUTTON))
  {
      if (Global::GetInstance()->IsPaused)
          ++selection;
  }
}

void free_tutorial()
{
    AEGfxMeshFree(Global::GetInstance()->vertex_slide1);
    AEGfxMeshFree(Global::GetInstance()->vertex_slide2);
    AEGfxMeshFree(Global::GetInstance()->vertex_slide3);
    AEGfxMeshFree(Global::GetInstance()->vertex_slide4);
    AEGfxMeshFree(Global::GetInstance()->vertex_slide5);
    AEGfxMeshFree(Global::GetInstance()->vertex_slide6);
    AEGfxMeshFree(Global::GetInstance()->vertex_slide7);
    AEGfxMeshFree(Global::GetInstance()->vertex_slideRegen);
    AEGfxMeshFree(Global::GetInstance()->vertex_slideBasic);
    AEGfxMeshFree(Global::GetInstance()->vertex_slideAdv);
    AEGfxMeshFree(Global::GetInstance()->vertex_bossTut);
}

void unload_tutorial()
{
    AEGfxTextureUnload(Global::GetInstance()->texture_slide1);
    AEGfxTextureUnload(Global::GetInstance()->texture_slide2);
    AEGfxTextureUnload(Global::GetInstance()->texture_slide3);
    AEGfxTextureUnload(Global::GetInstance()->texture_slide4);
    AEGfxTextureUnload(Global::GetInstance()->texture_slide5);
    AEGfxTextureUnload(Global::GetInstance()->texture_slide6);
    AEGfxTextureUnload(Global::GetInstance()->texture_slide7);
    AEGfxTextureUnload(Global::GetInstance()->texture_slideRegen);
    AEGfxTextureUnload(Global::GetInstance()->texture_slideBasic);
    AEGfxTextureUnload(Global::GetInstance()->texture_slideAdv);
    AEGfxTextureUnload(Global::GetInstance()->texture_bossTut);
}


void load_options()
{
    // start adding mesh for the main menu button use
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
    options = AEGfxMeshEnd();
    AE_ASSERT_MESG(options, "Failed to create mesh button!!");

    // load all the required textures to be used later with the meshes
    options_page = AEGfxTextureLoad("Resources\\Options\\option.png");
    AE_ASSERT_MESG(options_page, "Failed to create instructions_page texture!");

    // start adding mesh for the main menu button use
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
    leftbtnMesh1 = AEGfxMeshEnd();
    AE_ASSERT_MESG(leftbtnMesh1, "Failed to create leftbuttonmesh!!");

    // start adding mesh for the main menu button use
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
    checkboxMesh = AEGfxMeshEnd();
    AE_ASSERT_MESG(checkboxMesh, "Failed to create checkboxMesh!!");

    // start adding mesh for the main menu button use
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
    volumeSlider = AEGfxMeshEnd();
    AE_ASSERT_MESG(volumeSlider, "Failed to create volumeSlider mesh!!");
    // Texture 1: From file
    volumeSliderTex = AEGfxTextureLoad("Resources\\Options\\volumeslider.png");
    AE_ASSERT_MESG(volumeSliderTex, "Failed to create the volumeslider texture!!");
    // load all the required textures to be used later with the meshes
    leftbtn1 = AEGfxTextureLoad("Resources\\Instructions\\leftbutton.png");
    AE_ASSERT_MESG(leftbtn1, "Failed to create leftbutton texture!");
    // load all the required textures to be used later with the meshes
    checkbox = AEGfxTextureLoad("Resources\\Options\\checkbox.png");
    AE_ASSERT_MESG(checkbox, "Failed to create leftbutton texture!");
    // load all the required textures to be used later with the meshes
    checkbox1 = AEGfxTextureLoad("Resources\\Options\\checkbox.png");
    AE_ASSERT_MESG(checkbox1, "Failed to create leftbutton texture!");
    // Texture 1: From file
    option_back = AEGfxTextureLoad("Resources\\Instructions\\leftbutton.png");
    AE_ASSERT_MESG(option_back, "Failed to create the leftbutton texture!!");
}

void init_options()
{
    leftbtn_hover1 = false;
    selection_checkbox = 0;
    //*fullscreen_check = false;
    //*mute_check = false;
    InitAudio();
    PlayAudio(SOUNDTYPE::ST_MMBACKGROUND);
}

void update_options()
{
    SetBGVolume(Global::GetInstance()->volumeGlobal);

    fullscreenCheck();

    if (AEInputCheckReleased(AEVK_ESCAPE) && !Global::GetInstance()->IsPaused)
    {
        // Back to Main Menu
        gGameStateNext = gGameStatePrev;
        gGameStatePrev = gGameStateCurr;

    }
    
    if (Global::GetInstance()->IsPaused)
        SetBGVolume(0.0f);

    if ((mouse_x4 >= 397 && mouse_x4 <= 428) && (mouse_y4 >= 250 && mouse_y4 <= 286))
    {
        if (AEInputCheckReleased(AEVK_LBUTTON) && !*fullscreen_check)
        {
            //selection_checkbox = 1;
            //Global::GetInstance()->full_screen = !Global::GetInstance()->full_screen;
            AEToogleFullScreen(Global::GetInstance()->full_screen);
            *fullscreen_check = true;
        }
        else if (AEInputCheckReleased(AEVK_LBUTTON) && *fullscreen_check)
        {
            //selection_checkbox = 0;
            //!Global::GetInstance()->full_screen = Global::GetInstance()->full_screen;
            AEToogleFullScreen(!Global::GetInstance()->full_screen);
            *fullscreen_check = false;
        }
    }
    else if ((mouse_x4 >= 397 && mouse_x4 <= 428) && (mouse_y4 >= 328 && mouse_y4 <= 363))
    {
        if (AEInputCheckReleased(AEVK_LBUTTON) && !*mute_check)
        {
            //selection_checkbox = 2;
            *mute_check = true;
            //SetBGVolume(0.0f);
            Global::GetInstance()->volumeGlobal = 0.0f;
        }
        else if (AEInputCheckReleased(AEVK_LBUTTON) && *mute_check)
        {
            //selection_checkbox = 0;
            *mute_check = false;
            //SetBGVolume(0.5f);
            Global::GetInstance()->volumeGlobal = 0.5f;
        }

    }
    else if ((mouse_x4 >= 400 && mouse_x4 <= 514) && (mouse_y4 >= 417 && mouse_y4 <= 435))
    {
        if (AEInputCheckReleased(AEVK_LBUTTON))
        {
            selection_checkbox = 3;
        }
    }
    if ((mouse_x4 >= 73 && mouse_x4 <= 224) && (mouse_y4 >= 475 && mouse_y4 <= 561))
    {
        leftbtn_hover1 = true;
        if (AEInputCheckReleased(AEVK_LBUTTON))
        {
            // change the state - MAIN MENU
            if (Global::GetInstance()->IsPaused)
                Global::GetInstance()->isOption = false;
            else
            {
                // change the state - MAIN MENU
                gGameStateNext = gGameStatePrev;
                gGameStatePrev = gGameStateCurr;
            }
        }
    }
    else
    {
        leftbtn_hover1 = false;
    }

    //Volume adjustment

    //divide bar into 100 portions
    //every portion will set volume to a diff level
    //if mouse click to that portion change volume

    if ((mouse_x4 >= 400 && mouse_x4 <= 500) &&
        (mouse_y4 >= 417 && mouse_y4 <= 436))
    {
        if (AEInputCheckCurr(AEVK_LBUTTON))
        {
            float length = 500 - 400;
            float curr = (mouse_x4 / length) - 4;

            Global::GetInstance()->volumeGlobal = curr;
            sliderx = (curr * 100) + 400 * Global::GetInstance()->g_dt;
        }
    }

    if (Global::GetInstance()->volumeGlobal <= 0.0f)
    {
        *mute_check = true;
    }
    else
    {
        *mute_check = false;
    }



    AEInputGetCursorPosition(&mouse_x4, &mouse_y4);

}

void draw_options()
{
    MainMenu_Render(options_page, options);
    if (leftbtn_hover1)
    {
        AEMtx33 scale, trans, transform;
        //you must know the syntax chose one lmao
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
        AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxTextureSet(option_back, 0.0f, 0.0f);

        //The following code will adjust the size of the buttons
        AEMtx33Scale(&scale, 400.0f, 500.0f);
        AEMtx33Trans(&trans, -250.0f, -240.0f);
        AEMtx33Concat(&transform, &trans, &scale);
        AEGfxSetTransform(transform.m);

        AEGfxMeshDraw(leftbtnMesh1, AE_GFX_MDM_TRIANGLES);
        AEGfxSetTransparency(1.0f);
    }
    if (*fullscreen_check)
    {

        AEMtx33 scale, trans, transform;
        //you must know the syntax chose one lmao
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
        AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxTextureSet(checkbox, 0.0f, 0.0f);

        //The following code will adjust the size of the buttons
        AEMtx33Scale(&scale, 50.0f, 50.0f);
        AEMtx33Trans(&trans, 20.0f, 35.0f);
        AEMtx33Concat(&transform, &trans, &scale);
        AEGfxSetTransform(transform.m);

        AEGfxMeshDraw(leftbtnMesh1, AE_GFX_MDM_TRIANGLES);
        AEGfxSetTransparency(1.0f);
    }
    if (*mute_check)
    {
        AEMtx33 scale1, trans1, transform1;
        //you must know the syntax chose one lmao
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
        AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxTextureSet(checkbox1, 0.0f, 0.0f);

        //The following code will adjust the size of the buttons
        AEMtx33Scale(&scale1, 50.0f, 50.0f);
        AEMtx33Trans(&trans1, 20.0f, -42.0f);
        AEMtx33Concat(&transform1, &trans1, &scale1);
        AEGfxSetTransform(transform1.m);

        AEGfxMeshDraw(checkboxMesh, AE_GFX_MDM_TRIANGLES);
        AEGfxSetTransparency(1.0f);
    }
    AEMtx33 scale111, trans111, transform111;
    //you must know the syntax chose one lmao
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxTextureSet(volumeSliderTex, 0.0f, 0.0f);

    //The following code will adjust the size of the buttons
    AEMtx33Scale(&scale111, 120.0f, 120.0f);
    AEMtx33Trans(&trans111, sliderx, -138.0f);
    AEMtx33Concat(&transform111, &trans111, &scale111);
    AEGfxSetTransform(transform111.m);

    AEGfxMeshDraw(volumeSlider, AE_GFX_MDM_TRIANGLES);
    AEGfxSetTransparency(1.0f);
}

void draw_options_pause()
{
    // Drawing mainMenu
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_NONE);
    AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
    // Set position for object 1
    AEGfxSetPosition(0.0f, 0.0f);
    // set mainmenu texture
    AEGfxTextureSet(options_page, 1.0f, 1.0f);
    // No tint
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Drawing the mesh (list of triangles)
    AEMtx33 scale, trans, transform;
    AEMtx33Scale(&scale, (f32)AEGetWindowWidth() * 1.0f, (f32)AEGetWindowHeight() * 1.0f);
    AEMtx33Trans(&trans, Global::GetInstance()->camera.axis.x, 0.0f);
    AEMtx33Concat(&transform, &trans, &scale);
    AEGfxSetTransform(transform.m);
    // render menu image
    AEGfxMeshDraw(options, AE_GFX_MDM_TRIANGLES);

    if (leftbtn_hover1)
    {
        /*AEMtx33 scale, trans, transform;*/
        //you must know the syntax chose one lmao
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
        AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxTextureSet(option_back, 0.0f, 0.0f);

        //The following code will adjust the size of the buttons
        AEMtx33Scale(&scale, 400.0f, 500.0f);
        AEMtx33Trans(&trans, -250.0f + Global::GetInstance()->camera.axis.x, -240.0f);
        AEMtx33Concat(&transform, &trans, &scale);
        AEGfxSetTransform(transform.m);

        AEGfxMeshDraw(leftbtnMesh1, AE_GFX_MDM_TRIANGLES);
        AEGfxSetTransparency(1.0f);
    }
    if (*fullscreen_check)
    {

        //AEMtx33 scale, trans, transform;
        //you must know the syntax chose one lmao
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
        AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxTextureSet(checkbox, 0.0f, 0.0f);

        //The following code will adjust the size of the buttons
        AEMtx33Scale(&scale, 50.0f, 50.0f);
        AEMtx33Trans(&trans, 20.0f + Global::GetInstance()->camera.axis.x, 35.0f);
        AEMtx33Concat(&transform, &trans, &scale);
        AEGfxSetTransform(transform.m);

        AEGfxMeshDraw(leftbtnMesh1, AE_GFX_MDM_TRIANGLES);
        AEGfxSetTransparency(1.0f);
    }
    if (*mute_check)
    {
        AEMtx33 scale1, trans1, transform1;
        //you must know the syntax chose one lmao
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
        AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxTextureSet(checkbox1, 0.0f, 0.0f);

        //The following code will adjust the size of the buttons
        AEMtx33Scale(&scale1, 50.0f, 50.0f);
        AEMtx33Trans(&trans1, 20.0f + Global::GetInstance()->camera.axis.x, -42.0f);
        AEMtx33Concat(&transform1, &trans1, &scale1);
        AEGfxSetTransform(transform1.m);

        AEGfxMeshDraw(checkboxMesh, AE_GFX_MDM_TRIANGLES);
        AEGfxSetTransparency(1.0f);
    }
    AEMtx33 scale112, trans112, transform112;
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxTextureSet(volumeSliderTex, 0.0f, 0.0f);

    //The following code will adjust the size of the buttons
    AEMtx33Scale(&scale112, 120.0f, 120.0f);
    AEMtx33Trans(&trans112, sliderx + Global::GetInstance()->camera.axis.x, -138.0f);
    AEMtx33Concat(&transform112, &trans112, &scale112);
    AEGfxSetTransform(transform112.m);

    AEGfxMeshDraw(volumeSlider, AE_GFX_MDM_TRIANGLES);
    AEGfxSetTransparency(1.0f);
}

void unload_options()
{
    AEGfxMeshFree(options);
    AEGfxMeshFree(leftbtnMesh1);
    AEGfxMeshFree(checkboxMesh);
    AEGfxMeshFree(volumeSlider);
    FreeAudio();
    AEGfxTextureUnload(options_page);
    AEGfxTextureUnload(leftbtn1);
    AEGfxTextureUnload(checkbox);
    AEGfxTextureUnload(checkbox1);
    AEGfxTextureUnload(option_back);
    AEGfxTextureUnload(volumeSliderTex);
}

void fullscreenCheck()
{
    if (AEInputCheckReleased(AEVK_F11))
    {
        //Global::GetInstance()->full_screen = !Global::GetInstance()->full_screen;
        //AEToogleFullScreen(Global::GetInstance()->full_screen);

        if (Global::GetInstance()->fullscreen_check)
        {
            Global::GetInstance()->fullscreen_check = false;
            AEToogleFullScreen(!Global::GetInstance()->full_screen);
        }
            
        else
        {
            Global::GetInstance()->fullscreen_check = true;
            AEToogleFullScreen(Global::GetInstance()->full_screen);
        }
            
    }
}

void skipORreturn()
{
    if (selection == slide6)
    {
        if ((mouse_x4 >= 8 && mouse_x4 <= 206) && (mouse_y4 >= 544 && mouse_y4 <= 591))
        {
            if (AEInputCheckReleased(AEVK_LBUTTON))
            {
                selection = slide1;
                gGameStatePrev = gGameStateCurr;
                gGameStateNext = GS_MAINMENU;
            }
        }
        if ((mouse_x4 >= 396 && mouse_x4 <= 614) && (mouse_y4 >= 321 && mouse_y4 <= 341))
        {
            if (AEInputCheckReleased(AEVK_LBUTTON))
            {
                selection = slideNone;
            }
        }
    }
    else if (selection == slideRegen || selection == slideAdv)
    {
        if ((mouse_x4 >= 8 && mouse_x4 <= 206) && (mouse_y4 >= 544 && mouse_y4 <= 591))
        {
            if (AEInputCheckReleased(AEVK_LBUTTON))
            {
                selection = slide1;
                gGameStatePrev = gGameStateCurr;
                gGameStateNext = GS_MAINMENU;
            }
        }
        if ((mouse_x4 >= 422 && mouse_x4 <= 645) && (mouse_y4 >= 361 && mouse_y4 <= 386))
        {
            if (AEInputCheckReleased(AEVK_LBUTTON))
            {
                selection = slideNone;
            }
        }
    }
    else
    {
        if ((mouse_x4 >= 8 && mouse_x4 <= 206) && (mouse_y4 >= 544 && mouse_y4 <= 591))
        {
            if (AEInputCheckReleased(AEVK_LBUTTON))
            {
                selection = slide1;
                gGameStatePrev = gGameStateCurr;
                gGameStateNext = GS_MAINMENU;
            }
        }
        if ((mouse_x4 >= 573 && mouse_x4 <= 781) && (mouse_y4 >= 544 && mouse_y4 <= 591))
        {
            if (AEInputCheckReleased(AEVK_LBUTTON))
            {
                selection = slideNone;
            }
        }
    }
}