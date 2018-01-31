/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHCommon.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-05-10
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_COMMON_H_
#define _KH_COMMON_H_

#include "KHTypes.h"
#include "RobotStruct.h"

/***************************************************************************************************
* MARCO DEFINITION
***************************************************************************************************/
#define KH_PI        ( 3.1415926535897932384626433832795 )
#define KH_EPS       ( 1.0e-4 )
#define KH_MAX(a,b)  ( (a) > (b) ? (a) : (b) )
#define KH_MIN(a,b)  ( (a) > (b) ? (b) : (a) )
#define KH_ABS(val)  ( ((val) > 0) ? (val) : (-val) )

/***************************************************************************************************
* STRUCT 
***************************************************************************************************/
struct Point2DS32
{
	bool operator==(const Point2DS32 &pnt) const
	{
		return ((x_ == pnt.x_) && (y_ == pnt.y_));
	}  

	S32 x_;
	S32 y_;
};

struct Point2DU32
{
	bool operator==(const Point2DU32 &pnt) const
	{
		return ((x_ == pnt.x_) && (y_ == pnt.y_));
	}

	U32 x_;
	U32 y_;
};


struct Point2DF32
{
	F32 x_;
	F32 y_;
};


struct Point3DF32
{
	F32 x_;
	F32 y_;
	F32 z_;
};


struct Vector2DS32
{
	S32 x_;
	S32 y_;
};


struct Vector2DF32
{
	F32 x_;
	F32 y_;
};


struct Speed3DF32
{
	F32 vx_;
	F32 vy_;
	F32 vz_;
};


struct RobotPosInfo
{
	Point3DF32 pos_;
	Speed3DF32 speed_;
};

struct PathPointF32
{
	F32       x_;
	F32       y_;
	F32       th_;
	Direction dir_;
};


#endif//_KH_COMMON_H_
