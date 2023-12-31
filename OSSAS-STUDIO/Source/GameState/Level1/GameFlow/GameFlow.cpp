/******************************************************************************
* \file			Gameflow.h
* \brief		Contain all the Gameflow related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include "GameState/Level1/GameFlow/Gameflow.h"
#include "GameState/GameObj/GameObj.h"
#include "GameState/Global.h"
#include "Helper/Helper.h"

#include <algorithm>

#define GROUND_LEVEL -50.0f

auto& pEnemy_Tower = Global::GetInstance()->pEnemy_Tower;
auto& Total_time = Global::GetInstance()->g_appTime;
auto& Event_List = Global::GetInstance()->Event_list;
//int GameFlow::Num_Event_Done = 0;
bool GameFlow::touch = false;
bool GameFlow::half_life = false;
/*****************************************************************************/
/*!
 \fn static void Level1_GameFlow();
 \brief init the gameflow
*/
/*****************************************************************************/
void GameFlow::Level1_GameFlow()
{
	GameFlow::half_life = false;
	GameFlow::touch = false;

	Event_List.push_back(new Duck(10.0f));
	Event_List.push_back(new Ostrich(5.0f));
	Event_List.push_back(new Duck(5.0f));
	Event_List.push_back(new Ostrich(5.0f));
	Event_List.push_back(new Ostrich(5.0f));
	Event_List.push_back(new Penguin(5.0f));
	Event_List.push_back(new Ostrich(3.0f));
	Event_List.push_back(new Ostrich(3.0f));
	Event_List.push_back(new Ostrich(3.0f));
	Event_List.push_back(new Penguin(1.5));
	Event_List.push_back(new Duck(1.5f));
	Event_List.push_back(new Ostrich(1.5f));
	Event_List.push_back(new Ostrich(1.5f));
}

void SpawnWave(int numDuckTrouble, int numOstrich, int numPenguins)
{
	for (int i = 0; i < numDuckTrouble; ++i)
		Event_List.insert(Event_List.begin(), new Duck_Trouble(0.1f));

	for (int i = 0; i < numOstrich; ++i)
		Event_List.insert(Event_List.begin(), new Ostrich(0.1f));

	for (int f = 0; f < numPenguins; ++f)
		Event_List.insert(Event_List.begin(), new Penguin(0.1f));
}

/*****************************************************************************/
/*!
 \fn static void GameFlow_update();
 \brief updating which event is next
*/
/*****************************************************************************/
void GameFlow::GameFlow_update()
{
	bool previous = true;
	if (Global::GetInstance()->pEnemy_Tower != nullptr)
	{
		if (Global::GetInstance()->pEnemy_Tower->HealthCurr !=
			Global::GetInstance()->pEnemy_Tower->pObject->Health
			&&
			!touch)
		{
			Event_List.insert(Event_List.begin(), new Duck_Trouble(0.1f));
			for (int i = 0; i < 5; ++i)
				Event_List.insert(Event_List.begin(), new Ostrich(0.1f));
			for (int f = 0; f < 2; ++f)
				Event_List.insert(Event_List.begin(), new Penguin(0.1f));
			touch = true;
		}
		else if (Global::GetInstance()->pEnemy_Tower->HealthCurr <
			Global::GetInstance()->pEnemy_Tower->pObject->Health / 2.0f &&
			!half_life)
		{
			if (Global::GetInstance()->level_select == 1)
			{
				Event_List.insert(Event_List.begin(), new Duck_Trouble(0.1f));
				for (int i = 0; i < 7; ++i)
					Event_List.insert(Event_List.begin(), new Ostrich(0.1f));
				for (int f = 0; f < 3; ++f)
					Event_List.insert(Event_List.begin(), new Penguin(0.1f));
			}
			else if (Global::GetInstance()->level_select == 2)
			{

				Event_List.insert(Event_List.begin(), new Duck_Trouble(0.1f));
				for (int i = 0; i < 10; ++i)
					Event_List.insert(Event_List.begin(), new Ostrich(0.1f));
				for (int f = 0; f < 5; ++f)
					Event_List.insert(Event_List.begin(), new Penguin(0.1f));
				Global::GetInstance()->boss_skills = true;
			}
			half_life = true;
		}
		for (auto& Event : Event_List)
		{
			if (previous)
			{
				if (!Event->timer_set)
				{
					Event->timer = (float)Total_time;
					Event->timer_set = true;
				}
				Event->flow();// problem
			}
			previous = Event->done;
		}
		// deleting
		// C++ 98 with iterators
		for (auto it = Event_List.begin(); it != Event_List.end();)
		{
			if ((*it)->done)
			{
				Event* e = (*it);
				it = Event_List.erase(it);

				delete e;
			}
			else
				++it;
		}

		// C++ 11 with Lambdas
		//Event_List.erase(
		//	std::remove_if(Event_List.begin(), Event_List.end(), [](auto& e) {return e->done; }), Event_List.end()
		//);

		if (Global::GetInstance()->level_select == 1 ||
			Global::GetInstance()->boss_skills)
		{
			if (Event_List.empty())
			{
				for (int i = 0; i < 2; ++i)
					Event_List.insert(Event_List.begin(), new Ostrich(1.0f));

				Event_List.insert(Event_List.begin(), new Penguin(1.0f));
				Event_List.insert(Event_List.begin(), new Duck(5.0f));
			}
		}
		else if (Global::GetInstance()->level_select == 2)
		{
			if (Event_List.empty())
			{
				for (int i = 0; i < 3; ++i)
					Event_List.insert(Event_List.begin(), new Ostrich(1.0f));
				for (int i = 0; i < 2; ++i)
					Event_List.insert(Event_List.begin(), new Penguin(1.0f));
				Event_List.insert(Event_List.begin(), new Duck(5.0f));
			}
		}
	}


	// YGE : Debug Function
#ifdef _DEBUG
	// Spawn a wave
	if (AEInputCheckReleased(AEVK_5))
	{
		SpawnWave(1,5,5);
	}
#endif // _DEBUG



}
/*****************************************************************************/
/*!
 \fn static void Unload_Events();
 \brief Delete all event
*/
/*****************************************************************************/
void GameFlow::Free_Events()
{
	for (size_t i = 0; i < Event_List.size(); ++i)
	{
		delete Event_List[i];
	}
	Event_List.clear();
}
/*****************************************************************************/
/*!
 \fn Event();
 \brief basic Constructor
*/
/*****************************************************************************/
Event::Event() :
	timer{ 0.0f },
	done{ false },
	waiting_time{ 0.0f },
	timer_set{ false }
{
}
/*****************************************************************************/
/*!
 \fn virtual ~Event();
 \brief virtual destructor
*/
/*****************************************************************************/
Event::~Event()
{
}
/*****************************************************************************/
/*!
 \fn Duck(float _waiting_time);
 \brief implementing Constructor
 \param float _waiting_time
 the timer for the event to start
*/
/*****************************************************************************/
Duck::Duck(float _waiting_time)
{
	waiting_time = _waiting_time;
}
/*****************************************************************************/
/*!
 \fn void flow()
 \brief what flow is needed for this event
*/
/*****************************************************************************/
void Duck::flow()
{
	if ((float)Total_time - timer >= waiting_time && !done)
	{
		Helper::ObjCreater(Helper::Create_2d_Vector(pEnemy_Tower->posCurr.x,
			pEnemy_Tower->posCurr.y - 100.0f + Helper::get_Enemy_Y_pos()),
			true, nullptr, TYPE_OBJECT::TYPE_OBJECT_DUCK);
		done = true;
		Helper::Sort_List();
	}
}
/*****************************************************************************/
/*!
 \fn Duck_Trouble(float _waiting_time);
 \brief implementing Constructor
 \param float _waiting_time
 the timer for the event to start
*/
/*****************************************************************************/
Duck_Trouble::Duck_Trouble(float _waiting_time)
{
	waiting_time = _waiting_time;
}
/*****************************************************************************/
/*!
 \fn void flow()
 \brief what flow is needed for this event
*/
/*****************************************************************************/
void Duck_Trouble::flow()
{
	if ((float)Total_time - timer >= waiting_time && !done)
	{
		for (int i = 0; i < 3; ++i)
		{
			Helper::ObjCreater(Helper::Create_2d_Vector(pEnemy_Tower->posCurr.x,
				pEnemy_Tower->posCurr.y - 100.0f + Helper::get_Enemy_Y_pos()),
				true, nullptr, TYPE_OBJECT::TYPE_OBJECT_DUCK);
		}
		Helper::Sort_List();
		done = true;
	}
}
/*****************************************************************************/
/*!
 \fn Penguin(float _waiting_time);
 \brief implementing Constructor
 \param float _waiting_time
 the timer for the event to start
*/
/*****************************************************************************/
Penguin::Penguin(float _waiting_time)
{
	waiting_time = _waiting_time;
}
/*****************************************************************************/
/*!
 \fn void flow()
 \brief what flow is needed for this event
*/
/*****************************************************************************/
void Penguin::flow()
{
	if ((float)Total_time - timer >= waiting_time && !done)
	{
		Helper::ObjCreater(Helper::Create_2d_Vector(pEnemy_Tower->posCurr.x,
			pEnemy_Tower->posCurr.y - 100.0f + Helper::get_Enemy_Y_pos()),
			true, nullptr, TYPE_OBJECT::TYPE_OBJECT_PENGIUN);
		done = true;
		Helper::Sort_List();
	}
}
/*****************************************************************************/
/*!
 \fn Ostrich(float _waiting_time);
 \brief implementing Constructor
 \param float _waiting_time
 the timer for the event to start
*/
/*****************************************************************************/
Ostrich::Ostrich(float _waiting_time)
{
	waiting_time = _waiting_time;
}
/*****************************************************************************/
/*!
 \fn void flow()
 \brief what flow is needed for this event
*/
/*****************************************************************************/
void Ostrich::flow()
{
	if ((float)Total_time - timer >= waiting_time && !done)
	{
		Helper::ObjCreater(Helper::Create_2d_Vector(pEnemy_Tower->posCurr.x,
			(GROUND_LEVEL + 125.0f / 2.0f) + Helper::get_Enemy_Y_pos()),
			true, nullptr, TYPE_OBJECT::TYPE_OBJECT_OSTRICH);
		done = true;
		Helper::Sort_List();
	}
}
