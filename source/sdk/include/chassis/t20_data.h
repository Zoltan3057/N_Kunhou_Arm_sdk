#ifndef _CHASSIS_T20_DATA_WANGHONGTAO_2017_06_05_
#define _CHASSIS_T20_DATA_WANGHONGTAO_2017_06_05_

#include <linux/can.h>

#include <boost/thread.hpp>

#include "TimerDiff.h"
#include "buffer_con.hpp"
#include "chassis/driver_base.h"

typedef struct  _ST20_Can_data
{
	//feedback
	F32 wheel_speed_rpm_;
	F32 wheel_angle_deg_;

	//control speed
	F32 ctl_speed_ad_;
	bool handle_down_;
	bool press_down_;

	//control angle
	F32 ctl_angle_ad1_;
	F32 ctl_angle_ad2_;

	//control fork
	bool fork_up_;
	bool fork_down_;

}ST20_Can_data;


class t20_data
{
public:
	t20_data();
	~t20_data();

	void state_machine(U8* data);
	ST20_Can_data get_data(const F32 &reduction_ratio);

	bool ReadData(std::vector<can_frame> &frame_list);

protected:


private://virtual customized :TODO
	void clear_mathine();

	typedef enum{HEAD,SE,RL,LEN,ID,DATA} T20STA;
	T20STA t20sta_;

	U32 COMB_ID_;
	U8  Data8[8];
	U8  id_len_;
	U8  data_len_;

	boost::mutex mu_data_;
	void parse_data();
	void parse_181();
	void parse_182();
	void parse_183();

private://T20 data
	ST20_Can_data can_data_;

	CSBuffer<can_frame,100> can_list_;

};



#endif//_CHASSIS_T20_DATA_WANGHONGTAO_2017_06_05_

