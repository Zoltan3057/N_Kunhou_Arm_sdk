#ifndef _CHASSIS_2WD_DIFF_WANGHONGTAO_2015_09_17_
#define _CHASSIS_2WD_DIFF_WANGHONGTAO_2015_09_17_

#include "chassis_base.h"

class Chassis_2wd_diff : Chassis_base
{
public:

	Chassis_2wd_diff();
	~Chassis_2wd_diff();

private://virtual
	//
	void setPara(std::string name , std::string value);
	bool initDriver(std::string para);

	//get odom in Base link (origin point is robot rotation center)
	void getOdomInBaseLink(F32 &dx,F32 &dy, F32 &th);
	void getOdomSpeed(F32 &vx,F32 &vy, F32 &th);

	void setOdomSpeed(F32 vx, F32 vy, F32 vw);

	

private:

	F32 wheel_dia_;	
	F32 axle_;		
	

	F32 Speed2RPM( F32 speed );
	F32 RPM2Speed( F32 RPM );
	

protected:
private:
};



#endif//_CHASSIS_2WD_DIFF_WANGHONGTAO_2015_09_17_
