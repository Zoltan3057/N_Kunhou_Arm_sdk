#ifndef _CHASSIS_CHUANGMENG_DRIVER_WANGHONGTAO_2015_11_30_
#define _CHASSIS_CHUANGMENG_DRIVER_WANGHONGTAO_2015_11_30_

#include "TimerDiff.h"

#include "chassis/driver_base.h"

class cTransferDevice;

class chuangmeng_driver : Driver_base
{
public:
	chuangmeng_driver();
	~chuangmeng_driver();

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

//	void setAngle(int id,F32 rad_angle);
//	F32 getAngle(int id);
//	bool first_odo_;

private:
	//copley chassis example, customer should not do like this
	void init_customer_para();


	F32 GetDiffPos(int id );
	
// 	void GetSpeedL();
// 	void GetSpeedR();

	void SendData( U8* s_data,U16 len );
	void ReadData( U8* r_data,int &len,const int &need);
	
	cTimerDiff dt_;
	F32 left_speed_;
	F32 right_speed_;

	bool First_pos_;

	S16 left_pos_;
	S16 last_left_pos_;
	S16 deta_left_pos_;

	F32 left_set_v_;
	F32 left_current_v_;

	cTimerDiff right_dt_;
	S16 right_pos_ ;		    //pos
	S16 last_right_pos_;
	S16 deta_right_pos_;

	F32 right_set_v_;
	F32 right_current_v_;

	F32 c_left_;
	F32 c_right_;

private:
	
	bool b1_Enable_;
	bool b1_CCW_;
	bool b2_Enable_;
	bool b2_CCW_;
	bool b1_ST_;
	bool b2_ST_;

	U8 ch_enable_cw[8];//00 06 14 00 00 05 4C 39
	U8 ch_speed[13];  //00 10 3F 08 00 02 04 01 F4 00 FA 23 49
	U8 ch_odom_1[8];     //01 03 46 12 00 01 31 47
	U8 ch_odom_2[8];     //02 03 46 12 00 01 31 74

	U8 ch_get_status_1[8];     //01 03 00 03 00 01 74 0A
	U8 ch_get_status_2[8];     //02 03 00 03 00 01 74 39

	U8 ch_clear_fault_1[8];     //01 06 0A 00 00 01 4B D2
	U8 ch_clear_fault_2[8];     //02 06 0A 00 00 01 4B E1

	void create_bit();
	void create_enable_cw();
	void create_speed();
	void create_odom();

private:
	S8 left_status_;
	S8 right_status_;

	bool get_status();
	void clear_fault();


};



#endif//_CHASSIS_CHUANGMENG_DRIVER_WANGHONGTAO_2015_11_30_
