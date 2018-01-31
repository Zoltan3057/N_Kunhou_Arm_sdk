/*
 * bias_steer_wheel.h
 *
 *  Created on: 08/05/2017
 *      Author: edwardzhang
 */

#ifndef BIAS_STEER_WHEEL_H_
#define BIAS_STEER_WHEEL_H_

#include "chassis/chassis_base.h"
#include "TimerDiff.h"


class bias_steer_wheel : Chassis_base
{
public:

	bias_steer_wheel();
	~bias_steer_wheel();

private://virtual
	//
	void setPara(std::string name , std::string value);
	bool initDriver(std::string para);

	//get odom in Base link (origin point is robot rotation center)
	void getOdomInBaseLink(F32 &dx,F32 &dy, F32 &th);
	void getOdomSpeed(F32 &vx,F32 &vy, F32 &th);

	void setOdomSpeed(F32 vx, F32 vy, F32 vw);



private:

	F32 wheel_a_dia_;	//action wheel dia
	F32 D_;				//vertical from action wheel to the line where is follower wheels on(axel)
	F32 L_;			    //foot is from action wheel to the line where is follower wheels on(axel) diff from odom (0,0)

	F32 wheel_f_dia_;   //follower wheel dia
	F32 axle_;

	F32 ASpeed2RPM( F32 speed );

	void resolution(F32 &speed,F32 &angle,const F32& Vx,const F32& W);

protected:
private:

	cTimerDiff dt_;

	F32 vx_ ;
	F32 vy_ ;
	F32 th_ ;

};



#endif /* BIAS_STEER_WHEEL_H_ */
