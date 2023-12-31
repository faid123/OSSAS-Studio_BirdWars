/******************************************************************************/
/*!
\file		tutorial.h
\author 	Ryan Ong Kok Chin
\par    	kokchinryan.ong@digipen.edu
          100% code contribution
\date   	20 Jun, 2020
\brief      To be used in GS_LEVEL1 only and holds header for tutorial.cpp.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
enum SLIDE
{
  slide1 = 0,
  slide2,
  slide3,
  slide4,
  slide5,
  slide6,
  slide7,
  slideBoss,
  transitionToGame,
  slideRegenWait,
  slideRegen,
  slideBasicWait,
  slideBasic,
  slideAdvWait,
  slideAdv,
  slideNone
};

void tutorial_load();
void tutorial_draw(AEGfxTexture* mainMenu, AEGfxVertexList* menu);
void tutorial_update();
void free_tutorial();
void unload_tutorial();
void debug_controls();

void load_options();
void init_options();
void update_options();
void draw_options();
void unload_options();
void draw_options_pause();
void fullscreenCheck();
void skipORreturn();