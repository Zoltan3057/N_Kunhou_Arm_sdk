#ifndef _CHASSIS_FORKLIFT_DIFF_KT_WANGHONGTAO_2016_08_22_
#define _CHASSIS_FORKLIFT_DIFF_KT_WANGHONGTAO_2016_08_22_

#include "chassis_base.h"
#include "Pid.h"
#include "TimerDiff.h"

class Chassis_forklift_diff_kt : Chassis_base
{
public:

	Chassis_forklift_diff_kt();
	~Chassis_forklift_diff_kt();

private://virtual
	//
	void setPara(std::string name , std::string value);
	bool initDriver(std::string para);

	//get odom in Base link (origin point is robot rotation center)
	void getOdomInBaseLink(F32 &dx,F32 &dy, F32 &th);
	void getOdomSpeed(F32 &vx,F32 &vy, F32 &th);

	void setOdomSpeed(F32 vx, F32 vy, F32 vw);

private:

	F32 wheel_dia_;	    //action wheel dia 
	F32 D_;				//vertical from action wheel to the line where is follower wheels on(axel)
	//F32 L_;			    //foot is from action wheel to the line where is follower wheels on(axel) diff from odom (0,0)
	
	F32 axle_;		

	F32 Speed2RPM( F32 speed );

	void resolution(F32 &vleft,F32 &vright,F32 &angle,const F32& Vx,const F32& W);
	F32 getDecreaseW(const F32 &angle);

protected:
private:

	cTimerDiff dt_;

	F32 vx_ ;
	F32 vy_ ;
	F32 th_ ;

private:
	void check_safe_angle(F32 &v_left,F32 &v_right,const F32 &current_angle);
	void mixAngleSpeed( F32 &v_left,F32 &v_right,const F32 &set_angle,const F32 &current_angle );
	F32 last_set_speed_;

	Pid angle_pid_;


};



#endif//_CHASSIS_FORKLIFT_DIFF_KT_WANGHONGTAO_2016_08_22_
