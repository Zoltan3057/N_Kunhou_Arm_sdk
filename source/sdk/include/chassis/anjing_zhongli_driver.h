#ifndef _ANJIN_ZHONGLI_CHENG_R_G_H_
#define _ANJIN_ZHONGLI_CHENG_R_G_H_

#include "TimerDiff.h"

#include "chassis/driver_base.h"
#include "can_socket.h"

class cTransferDevice;

class anjing_zhongli_driver : Driver_base
{
public:
	anjing_zhongli_driver();
	~anjing_zhongli_driver();

protected:

private://virtual
	void setPara(std::string name ,std::string value);
	bool open_transfer_device();
	bool init_driver();
	void close_driver();

	void setTargetAngle(int id,F32 rad_angle);
	void setRPM(int id,F32 v);
	F32 getSpeed(int id);
	F32 getDiffAngle(int id);
	F32 getAngle( int id );


private:
	std::string port_;
	//SerialPort* sport_;
	cTransferDevice* pTransDevice_;
	F32 reduction_ratio_;
	U32 counts_;

	U32 one_wheel_counts_;

	bool first_odo_;

	//my chassis example, customer should not do like this
	void init_customer_para();

	void creat_traction_data(S32 &traction_speed);
	void creat_turn_data(S32 &turn_speed,S32 &turn_angle);
	void creat_control_data();

	bool stuffing_;

	F32 traction_speed_;
	S32 traction_speed_1;
	S32 traction_speed_2;
	S32 traction_dir_;
	S32 acc_;
	S32 dec_;
	S32 brakepot_;

	F32 turn_speed_;
	S32 turn_speed_1;
	S32 turn_speed_2;
	S32 turn_angle_;
	S32 turn_angle_1;
	S32 turn_angle_2;
	S32 turn_bit1_;

	S32 AUX_control_;
	S32 EVP1_;
	S32 EVP2_;
	S32 pump_acc_;
	S32 pump_dec_;
	S32 pwm_;

	S32 traction_pos_;
	S32 last_traction_pos_;
	S32 deta_traction_pos_;

	S32 send_data[16];
	S32 ch_read[20];
	S32 line;

	cTimerDiff dt_;

	F32 c_traction_speed_;
	F32 c_turn_angle_;
	F32 dangle_;

private:
	void th_read();

	struct can_frame anjing_zhongli_s_frame; //struct: send data
	struct can_frame anjing_zhongli_r_frame; //struct: read data
	std::vector<can_frame> anjing_zhongli_frame_list;//vector: read data

	bool b_run_;

private:
	SPos amcl_pos_;
};

#endif
