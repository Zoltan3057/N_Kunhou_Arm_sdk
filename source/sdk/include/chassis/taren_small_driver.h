#ifndef _CHASSIS_TAREN_CHENGRG_H_
#define _CHASSIS_TAREN_CHENGRG_H_

#include "TimerDiff.h"
#include "chassis/driver_base.h"
#include "can_socket.h"

class cTransferDevice;

class taren_small_driver : Driver_base
{
public:
	taren_small_driver();
	~taren_small_driver();

protected:

private:

	std::string port_;
	//SerialPort* sport_;
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
//	bool first_odo_;

private:
	//copley chassis example, customer should not do like this
	void init_customer_para();

	F32 GetDiffPos(int id );
	
	cTimerDiff left_dt_;
	F32 left_speed_;
	S16 left_speed_1;
	S16 left_speed_2;
	F32 right_speed_;
	S16 right_speed_1;
	S16 right_speed_2;

	S32 left_pos_;
	S32 last_left_pos_;
	S32 deta_left_pos_;

	F32 left_set_v_;
	F32 left_current_v_;

	cTimerDiff right_dt_;
	S32 right_pos_ ;		    //pos
	S32 last_right_pos_;
	S32 deta_right_pos_;

	F32 right_set_v_;
	F32 right_current_v_;

	F32 c_left_;
	F32 c_right_;

private:
	struct can_frame taren_small_s_frame; //struct: send data
	struct can_frame taren_small_r_frame; //struct: read data
	std::vector<can_frame> taren_r_frame_list;//vector: read data

	bool first_odo_l;
	bool first_odo_r;
	bool b_run_;
};



#endif
