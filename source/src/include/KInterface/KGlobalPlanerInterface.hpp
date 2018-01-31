/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KGlobalPlannerV2.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-05-19
*     Operate:  Modify
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _K_GLOBAL_PLANER_INTERFACE_HPP_
#define _K_GLOBAL_PLANER_INTERFACE_HPP_

#include "KHCommon.h"
#include "Pluma/Pluma.hpp"
#include "mapserver/GridMap.h"

class Map_Server;

class KGlobalPlanerInterface
{
public:
	KGlobalPlanerInterface(){}

public:
	virtual bool Init(Map_Server* map_server) = 0;

	virtual std::string GetName() = 0;

public:
	virtual bool MakePlan( SPos                      &start, 
		                   SPos                      &goal, 
						   SOdomSpeed                &current_odo,
						   std::vector<PathPointF32> &plan,
						   std::vector<SGridXY> &gplan) = 0;

	virtual void StopMakePlan() = 0;

};

PLUMA_PROVIDER_HEADER(KGlobalPlanerInterface);


#endif  // _K_GLOBAL_PLANER_INTERFACE_HPP_
