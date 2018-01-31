#ifndef _CHASSIS_T20_HALF_DRIVER_WANGHONGTAO_2017_06_05_
#define _CHASSIS_T20_HALF_DRIVER_WANGHONGTAO_2017_06_05_

#include "TimerDiff.h"
#include "driver_base.h"
#include "t20_data.h"

class cTransferDevice;

class t20_half_driver : Driver_base
{
public:
	t20_half_driver();
	~t20_half_driver();

	void test();

protected:


private://virtual customized :TODO
	void setPara(std::string name ,std::string value);

	bool init_driver();
	bool open_transfer_device();
	void close_driver();
	std::string port_;
	F32 reduction_ratio_;
	//SerialPort* sport_;
	cTransferDevice* pTransDevice_;


	void setRPM(int id,F32 v);
	F32 getSpeed(int id);

	F32 getDiffAngle(int id);

	void setAngle(int id,F32 rad_angle);
	F32 getAngle(int id);

private:

	cTimerDiff dt_;
	F32 dangle_;

	F32 c_action_wheel_speed_;
	F32 c_action_wheel_angle_;

private:
	bool b_run_;
	void th_read();
	t20_data data_;
};



#endif//_CHASSIS_T20_HALF_DRIVER_WANGHONGTAO_2017_06_05_
