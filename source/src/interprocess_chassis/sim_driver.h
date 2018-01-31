#ifndef _CHASSIS_SIM_DRIVER_WANGHONGTAO_2015_11_30_
#define _CHASSIS_SIM_DRIVER_WANGHONGTAO_2015_11_30_

#include "TimerDiff.h"
#include "chassis/driver_base.h"

class sim_driver : Driver_base
{
public:
	sim_driver();
	~sim_driver();

protected:

private:
	
	
	F32 reduction_ratio_;
	U32 counts_;

	U32 one_wheel_counts_;

private://virtual customized :TODO
	void setPara(std::string name ,std::string value);

	bool init_driver();
	bool open_transfer_device();
	void close_driver();

	void setRPM(int id,F32 v);
	F32 getSpeed(int id);

	F32 getDiffAngle(int id);

	void setAngle(int id,F32 rad_angle);
	F32 getAngle(int id);

private:

	F32 GetDiffPos(U8 id );
	
	
	cTimerDiff left_dt_;
	F32 left_speed_;
	F32 right_speed_;


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

};



#endif//_CHASSIS_SIM_DRIVER_WANGHONGTAO_2015_11_30_
