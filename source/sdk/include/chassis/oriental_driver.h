#ifndef _CHASSIS_ORIENTAL_DRIVER_WANGHONGTAO_2016_12_07_
#define _CHASSIS_ORIENTAL_DRIVER_WANGHONGTAO_2016_12_07_

#include "TimerDiff.h"

#include "driver_base.h"
#include "chassis_control_unit.h"

class cTransferDevice;

class oriental_driver : Driver_base
{
public:
	oriental_driver();
	~oriental_driver();

protected:

private:

	std::string port_;
	cTransferDevice* pTransDevice_;

	F32 reduction_ratio_;
	U32 counts_;

	U32 one_wheel_counts_;

private://virtual customized :TODO
	void setPara(std::string name ,std::string value);
	bool open_transfer_device();
	bool init_driver();
	void close_driver();

	void setRPM(int id,F32 v);
	F32 getSpeed(int id);

	F32 getDiffAngle(int id);

	void setAngle(int id,F32 rad_angle);
	F32 getAngle(int id);

	void SendData( U8* s_data,U16 len );
	void ReadData( U8* r_data,int &len,const int &need );

private:

	F32 GetDiffPos(int id );

private:

	cTimerDiff dt_test;
	cTimerDiff dt_[4];
	diff_pos id_[4];

	void create_speed(int id, F32 v);
	void choose_dir(int id, int v);
	void get_c_speed(int id);

	U8 ch_speed_[8];	//speed send date
	U8 ch_FWD_REV_[8];
	U8 ch_get_speed_[8];
	U8 ch_mode_0[8];
	U8 ch_N_NO3[8];
	U8 ch_A_NO3[8];
	U8 ch_D_NO3[8];
	U8 ch_F_NO3[8];

};


#endif//_CHASSIS_ORIENTAL_DRIVER_WANGHONGTAO_2016_12_07_
