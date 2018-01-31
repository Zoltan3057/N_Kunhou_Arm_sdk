/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHUtils.hpp
* Description:  commom utils
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-05-25
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_UTILS_H_
#define _KH_UTILS_H_

#include <math.h>
#include <float.h>
#include <sstream>
#include <iostream>
#include "KHTypes.h"
#include "KHCommon.h"


/***************************************************************************************************
* Interface Functions
***************************************************************************************************/
class KHUtils
{
public:
	/***************************************************************************************************
	* Circle
	***************************************************************************************************/
	// calculate the radius of the circle by three-point method
	static F32 CalCircleRadius(const SPos pnt[3])
	{
		F32 radius = 0.0f;

		F32 a11 = 2.0f * (pnt[0].x_ - pnt[1].x_);
		F32 a12 = 2.0f * (pnt[0].y_ - pnt[1].y_);
		F32 a21 = 2.0f * (pnt[1].x_ - pnt[2].x_);
		F32 a22 = 2.0f * (pnt[1].y_ - pnt[2].y_);

		F32 b1 = (pnt[0].x_ * pnt[0].x_ - pnt[1].x_ * pnt[1].x_)
			+ (pnt[0].y_ * pnt[0].y_ - pnt[1].y_ * pnt[1].y_) ;

		F32 b2 = (pnt[1].x_ * pnt[1].x_ - pnt[2].x_ * pnt[2].x_)
			+ (pnt[1].y_ * pnt[1].y_ - pnt[2].y_ * pnt[2].y_) ;

		//
		F32 k = a11 * a22 - a12 * a21;
		if( fabs(k) < KH_EPS )
		{
			radius = FLT_MAX;
		}
		else
		{
			F32 center_x = (b1 * a22 - a12 * b2) / k;
			F32 center_y = (a11 * b2 - a12 * b1) / k;

			F32 dx = center_x - pnt[0].x_;
			F32 dy = center_y - pnt[0].y_;

			radius = sqrt( dx * dx + dy * dy );
		}

		return radius;
	}


	/***************************************************************************************************
	* Position
	***************************************************************************************************/
	static SPos ComputeNewPosition(const SPos   &pos,
		                           const SSpeed &speed,
		                           const F32    &dt)
	{
		SPos new_pos;

		new_pos.x_ = pos.x_ + (speed.vx_ * cos(pos.th_) + speed.vy_ * cos(0.5f * KH_PI + pos.th_)) * dt;
		new_pos.y_ = pos.y_ + (speed.vx_ * sin(pos.th_) + speed.vy_ * sin(0.5f * KH_PI + pos.th_)) * dt;
		new_pos.th_ = pos.th_ + speed.vw_ * dt;

		return new_pos;
	}


	/***************************************************************************************************
	* Distance
	***************************************************************************************************/
	static F32 DistanceBetween2Pos(const SPos &pos1,
		                           const SPos &pos2)
	{
		F32 dx = pos1.x_ - pos2.x_ ;
		F32 dy = pos1.y_ - pos2.y_ ;

		return sqrt(dx * dx + dy *dy);
	}


	/***************************************************************************************************
	* Angle
	***************************************************************************************************/
//	static F32 IntersectionAngle(const Vector2DF32 &vec1,
//		                         const Vector2DF32 &vec2)
//	{
//		std::cout<<"vec1.x_:"<<vec1.x_<<" vec1.y_"<<vec1.y_<<" vec2.x_:"<<vec2.x_<<" vec2.y_"<<vec2.y_<<std::endl;
//
//		F32 dot_product = (vec1.x_ * vec2.x_ + vec1.y_ * vec2.y_);
//
//		//
//		F32 cross_product = (vec1.x_ * vec2.y_ - vec2.x_ * vec1.y_);
//		if (fabs(cross_product) < KH_EPS)
//		{
//			if (dot_product >= 0.0f)
//			{
//				std::cout<<"return 0"<<std::endl;
//				return 0.0f;
//			}
//			else
//			{
//				std::cout<<"return KH_PI"<<std::endl;
//				return KH_PI;
//			}
//		}
//		else
//		{
//			F32 vec1_norm = sqrt(vec1.x_ * vec1.x_ + vec1.y_ * vec1.y_);
//			F32 vec2_norm = sqrt(vec2.x_ * vec2.x_ + vec2.y_ * vec2.y_);
//
//			std::cout<<"vec1_norm:"<<vec1_norm<<" vec2_norm:"<<vec2_norm<<std::endl;
//
//			//
//			F32 angle = acos( dot_product / (vec1_norm * vec2_norm) );
//			std::cout<<"acos angle"<<angle<<std::endl;
//			if(cross_product < 0)
//			{
//				angle *= -1.0f;
//			}
//			std::cout<<"return angle"<<angle<<std::endl;
//			return angle;
//		}
//
//	}


//	static F32 IntersectionAngle(const Vector2DF32  &vec1,
//		                         const F32          &vec2_angle)
//	{
//		Vector2DF32 vec2;
//		vec2.x_ = cos(vec2_angle);
//		vec2.y_ = sin(vec2_angle);
//
//		return IntersectionAngle(vec2, vec1);
//	}


	/***************************************************************************************************
	* data conversion
	***************************************************************************************************/
	template<typename Type>
	static void String2Type(const STRING &data,
		                    Type         &type_val)
	{
		std::stringstream sstr;
		sstr<<data;
		sstr>>type_val;
	}


	template<typename Type>
	static void Type2String(const Type &type_val,
		                    STRING     &data)
	{
		std::stringstream sstr;
		sstr<<type_val;

		data = sstr.str();
	}
};


#endif // _KH_UTILS_H_
