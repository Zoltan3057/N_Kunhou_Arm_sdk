#ifndef __CHM_EV_DRIVER_H__
#define __CHM_EV_DRIVER_H__

#include "TimerDiff.h"

#include "chassis/driver_base.h"

class cTransferDevice;

class chm_EV_driver : Driver_base
{
public:
	chm_EV_driver();
	~chm_EV_driver();

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
	//copley chassis example, customer should not do like this
	//void init_customer_para();

	F32 GetDiffPos(int id );



	cTimerDiff left_dt_;
	F32 left_speed_;
	F32 right_speed_;

	S32 left_pos_;
	S32 last_left_pos_;
	F32 deta_left_pos_;

	F32 left_set_v_;
	F32 left_current_v_;

	cTimerDiff right_dt_;
	S32 right_pos_ ;		    //pos
	S32 last_right_pos_;
	F32 deta_right_pos_;

	F32 right_set_v_;
	F32 right_current_v_;

	F32 c_left_;
	F32 c_right_;

	//cTimerDiff t_dt_;

private:

	#define stop_mood (1<<0)		//
	#define ebrake    (1<<1)		//q stop
	#define d1_enable (1<<2)
	#define d1_ccw    (1<<3)
	#define d2_enable (1<<4)
	#define d2_ccw    (1<<5)

	void create_speed();
	void choose_dir();
	U8 creat_bit();
	void get_odom(int id);


	U8 ch_speed_[13];			//speed send date
	U8 ch_FWD_REV_[8];
	U8 ch_get_speed_[8];

};



#endif//_CHASSIS_ORIENTAL_DRIVER_WANGHONGTAO_2016_12_07_
