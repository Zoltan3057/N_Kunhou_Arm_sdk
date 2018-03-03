#ifndef _CHASSIS_OMNI_45_DONGLEI_2017_12_13_
#define _CHASSIS_OMNI_45_DONGLEI_2017_12_13_


#include "TimerDiff.h"
#include "chassis_base.h"


class Chassis_omni_45 : Chassis_base
{
public:

	Chassis_omni_45();
	~Chassis_omni_45();

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
	F32 wheel_base_;	//distance between front and back wheel
	F32 wheel_axle_;	//distance between left and right wheel

	cTimerDiff dt_;
	
	F32 cvX_;
	F32 cvY_;
	F32 cvW_;

	F32 vFL_;
	F32 vFR_;
	F32 vBL_;
	F32 vBR_;

	F32 Speed2RPM( F32 speed );
	F32 RPM2Speed( F32 RPM );
	
	void speed_disassemble(const F32 &vx, const F32 &vy, const F32 &vw);

protected:
	
private:
	//front matrix
	void cal_front_matrix();
	F32 FM00_;
	F32 FM01_;
	F32 FM02_;
	F32 FM10_;
	F32 FM11_;
	F32 FM12_;
	F32 FM20_;
	F32 FM21_;
	F32 FM22_;
	F32 FM30_;
	F32 FM31_;
	F32 FM32_;

	//back matrix
	void cal_back_matrix();
	F32 BM00_;
	F32 BM01_;
	F32 BM02_;
	F32 BM03_;
	F32 BM10_;
	F32 BM11_;
	F32 BM12_;
	F32 BM13_;
	F32 BM20_;
	F32 BM21_;
	F32 BM22_;
	F32 BM23_;


};

#endif   //_CHASSIS_OMNI_45_DONGLEI_2017_12_13_

