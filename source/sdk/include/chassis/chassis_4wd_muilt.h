#ifndef _CHASSIS_4WD_MUILT_
#define _CHASSIS_4WD_MUILT_
#include "Geometry.h"

#include "chassis/chassis_base.h"
#include "chassis/driver_base.h"
#include "chassis_control_unit.h"
#include "Pid.h"


class Chassis_4wd_muilt : Chassis_base
{
public:

	Chassis_4wd_muilt();
	~Chassis_4wd_muilt();

private://virtual
	//
	void setPara(std::string name , std::string value);
	bool initDriver(std::string para);

	//get odom in Base link (origin point is robot rotation center)
	void getOdomInBaseLink(F32 &dx,F32 &dy, F32 &th);
	void getOdomSpeed(F32 &vx,F32 &vy, F32 &th);

	void setOdomSpeed(F32 vx, F32 vy, F32 vw);

private:

	Control_unit unit[2];
	VecPosition Point1;
	VecPosition Point2;
	VecPosition r;
	VecPosition r1;
	VecPosition r_1;
	VecPosition r_2;
	Line getodomLine1;
	Line getodomLine2;

	Pid angle_pid_;

	F32 O_a;
	F32 Lx, Ly, L;
	F32 axle_, wheel_dia_;

	F32 k1,k2;
	F32 Y1,Y2,Last_Y1,Last_Y2,dY1,dY2;      //angle of sensor

	F32 vx_;
	F32 vy_;
	F32 vth_;

	F32 set_vx_;
	F32 set_vy_;
	F32 set_vth_;
	cTimerDiff dt_;

	void Control_unit_init();

	F32 Speed2RPM( F32 speed );
	F32 RPM2Speed( F32 RPM );


protected:
private:

};


#endif//












