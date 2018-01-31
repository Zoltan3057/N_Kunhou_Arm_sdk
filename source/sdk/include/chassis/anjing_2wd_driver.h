#ifndef _ANJIN_2WD_DRIVER_H_
#define _ANJIN_Ed_H_

#include "TimerDiff.h"

#include "chassis/driver_base.h"

#include "linux_driver/gpio.h"
class cTransferDevice;

class anjing_2wd_driver : Driver_base
{
public:
	anjing_2wd_driver();
	~anjing_2wd_driver();



protected:

private://virtual
	void setPara(std::string name ,std::string value);

	bool init_driver();
	void close_driver();

	void setRPM(int id,F32 v);
	void setAngle(int id,F32 rad_angle);
	F32 GetDiffPos(int id );
	F32 getAngle( int id );
	bool open_transfer_device();

	F32 getSpeed(int id);
	F32 getDiffAngle(int id);



private:
	std::string port_;
	//SerialPort* sport_;
	cTransferDevice* pTransDevice_;
	F32 reduction_ratio_;
	U32 counts_;

	U32 one_wheel_counts_;


	bool first_odo_;
	bool first_do;

	//my chassis example, customer should not do like this
	void init_customer_para();


	void GetSpeedLR();

	void SendData( U8* s_data,U16 len );
	void ReadData(U8* r_data,int &len,int need,int timeout );


	F32 wheel_speed_;
	S32 wheel_speed_1;
	S32 wheel_speed_2;
	S32 wheel_speed_3;
	S32 wheel_speed_4;
	F32 steer_angle_;
	F32 steer_angle_1;
	F32 steer_angle_2;
	F32 steer_angle_3;
	F32 steer_angle_4;
	U32 IO_data_;
	F32 IO_data_1;
	F32 IO_data_2;
	F32 IO_data_3;
	F32 IO_data_4;

	S32 wheel_pos_;
	S32 last_wheel_pos_;
	S32 deta_wheel_pos_;

	F32 feedback_angle_;

	U8 send_data[24];
	U8 ch_read[40];
	S32 line;

	cTimerDiff dt_;
	cTimerDiff test_dt_;

	F32 c_wheel_;

	gpio g_gpio_;


	cTimerDiff left_dt_;

	cTimerDiff ct_;
	S32 left_speed_;
	S32 left_pos_;  //pos
	S32 last_left_pos_;
	S32 deta_left_pos_;

	F32 left_set_v_;
	F32 left_current_v_;
	F32 c_left_;

	cTimerDiff right_dt_;
	S32 right_speed_;
	S32 right_pos_ ;
	S32 last_right_pos_;
	S32 deta_right_pos_;

	F32 right_set_v_;
	F32 right_current_v_;
	F32 c_right_;
	U32 cont;
};

//

#endif
