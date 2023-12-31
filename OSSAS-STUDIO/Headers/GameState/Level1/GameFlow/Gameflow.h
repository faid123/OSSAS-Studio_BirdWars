/******************************************************************************
* \file			Gameflow.h
* \brief		Contain all the Gameflow related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2020 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once
#include <vector>
/*****************************************************************************/
/*!
 \fn struct Event
 \brief basic event
*/
/*****************************************************************************/
struct Event
{
	float timer;
	bool done;
	float waiting_time;
	bool timer_set;
	/*****************************************************************************/
	/*!
	 \fn Event();
	 \brief basic Constructor
	*/
	/*****************************************************************************/
	Event();
	/*****************************************************************************/
	/*!
	 \fn virtual void flow()
	 \brief virtual game flow for each child class
	*/
	/*****************************************************************************/
	virtual void flow() = 0;
	/*****************************************************************************/
	/*!
	 \fn virtual ~Event();
	 \brief virtual destructor
	*/
	/*****************************************************************************/
	virtual ~Event();
};
/*****************************************************************************/
/*!
 \fn struct Duck
 \brief when duck is spawn on the enemy side
*/
/*****************************************************************************/
struct Duck: public Event
{
	/*****************************************************************************/
	/*!
	 \fn Duck(float _waiting_time);
	 \brief implementing Constructor
	 \param float _waiting_time
	 the timer for the event to start
	*/
	/*****************************************************************************/
	Duck(float _waiting_time);
	/*****************************************************************************/
	/*!
	 \fn void flow()
	 \brief what flow is needed for this event
	*/
	/*****************************************************************************/
	void flow();
};
/*****************************************************************************/
/*!
 \fn struct Duck_Trouble
 \brief when 3 duck is spawn on the enemy side
*/
/*****************************************************************************/
struct Duck_Trouble : public Event
{
	/*****************************************************************************/
	/*!
	 \fn Duck_Trouble(float _waiting_time);
	 \brief implementing Constructor
	 \param float _waiting_time
	 the timer for the event to start
	*/
	/*****************************************************************************/
	Duck_Trouble(float _waiting_time);
	/*****************************************************************************/
	/*!
	 \fn void flow()
	 \brief what flow is needed for this event
	*/
	/*****************************************************************************/
	void flow();
};
/*****************************************************************************/
/*!
 \fn struct Penguin
 \brief when Penguin is spawn on the enemy side
*/
/*****************************************************************************/
struct Penguin : public Event
{
	/*****************************************************************************/
	/*!
	 \fn Penguin(float _waiting_time);
	 \brief implementing Constructor
	 \param float _waiting_time
	 the timer for the event to start
	*/
	/*****************************************************************************/
	Penguin(float _waiting_time);
	/*****************************************************************************/
	/*!
	 \fn void flow()
	 \brief what flow is needed for this event
	*/
	/*****************************************************************************/
	void flow();
};
/*****************************************************************************/
/*!
 \fn struct Ostrich
 \brief when Ostrich is spawn on the enemy side
*/
/*****************************************************************************/
struct Ostrich : public Event
{
	/*****************************************************************************/
	/*!
	 \fn Ostrich(float _waiting_time);
	 \brief implementing Constructor
	 \param float _waiting_time
	 the timer for the event to start
	*/
	/*****************************************************************************/
	Ostrich(float _waiting_time);
	/*****************************************************************************/
	/*!
	 \fn void flow()
	 \brief what flow is needed for this event
	*/
	/*****************************************************************************/
	void flow();
};
using Events = std::vector<Event*>;
/*****************************************************************************/
/*!
 \fn struct GameFlow
 \brief things needed for gameflow
*/
/*****************************************************************************/
struct GameFlow
{
	static bool touch;
	static bool half_life;
	/*****************************************************************************/
	/*!
	 \fn static void Level1_GameFlow();
	 \brief init the gameflow
	*/
	/*****************************************************************************/
	static void Level1_GameFlow();
	/*****************************************************************************/
	/*!
	 \fn static void GameFlow_update();
	 \brief updating which event is next
	*/
	/*****************************************************************************/
	static void GameFlow_update();
	/*****************************************************************************/
	/*!
	 \fn static void Unload_Events();
	 \brief Delete all event 
	*/
	/*****************************************************************************/
	static void Free_Events();
};