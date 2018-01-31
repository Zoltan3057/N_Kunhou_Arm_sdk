#ifndef _CHASSIS_FORKLIFT_LD_ANALOG_WANGHONGTAO_2017_03_25_
#define _CHASSIS_FORKLIFT_LD_ANALOG_WANGHONGTAO_2017_03_25_

#include "chassis_base.h"
#include "TimerDiff.h"
//#include "Pid.h"

class Chassis_forklift_ld_analog : Chassis_base
{
public:

	Chassis_forklift_ld_analog();
	~Chassis_forklift_ld_analog();

private://virtual
	//
	void setPara(std::string name , std::string value);
	bool initDriver(std::string para);

	//get odom in Base link (origin point is robot rotation center)
	void getOdomInBaseLink(F32 &dx,F32 &dy, F32 &th);
	void getOdomSpeed(F32 &vx,F32 &vy, F32 &th);

	void setOdomSpeed(F32 vx, F32 vy, F32 vw);

	F32 last_angle_action_;
	F32 control_dt_;

private:

	F32 wheel_a_dia_;	//action wheel dia 
	F32 D_;				//vertical from action wheel to the line where is follower wheels on(axel)
	F32 L_;			    //foot is from action wheel to the line where is follower wheels on(axel) diff from odom (0,0)
	
	F32 wheel_f_dia_;   //follower wheel dia
	F32 axle_;		

	F32 ASpeed2RPM( F32 speed );
	F32 angleConver(F32 ad_angle);
	void resolution(F32 &speed,F32 &angle,const F32& Vx,const F32& W);

protected:
private:

	cTimerDiff dt_;

	F32 vx_ ;
	F32 vy_ ;
	F32 th_ ;

	filter<F32,10> filter_vx_;
	filter<F32,10> filter_vy_;
	filter<F32,10> filter_vw_;

	//Pid angle_pid_;

};



#endif//_CHASSIS_FORKLIFT_LD_ANALOG_WANGHONGTAO_2017_03_25_
