#ifndef _CHASSIS_ZHIDI_DRIVER_WANGHONGTAO_2016_08_22_
#define _CHASSIS_ZHIDI_DRIVER_WANGHONGTAO_2016_08_22_

#include "TimerDiff.h"
#include "chassis/driver_base.h"

class cTransferDevice;

class zhidi_driver : Driver_base
{
public:
	zhidi_driver();
	~zhidi_driver();

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

	void create_speed_pkg(const F32& speed);
	void create_angle_pkg(const F32& angle);
	void prase_pkg(U8* res,int len);

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
	
// 	void GetSpeedL();
// 	void GetSpeedR();

	void SendData( U8* s_data,U16 len );
	void ReadData( U8* r_data,int &len,int need);
	
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

private:

	U16 CRC16(U8 *p,U32 len);
	U8 set_speed_cmd_[13];  // ff 06 10 B0 00 00 04 01 02 03 04 a5 5a
	U8 set_angle_cmd_[13];  // ff 06 10 C0 00 00 04 01 02 03 04 a5 5a
	U8 get_counts_cmd_[9]; // ff 06 03 a0 00 00 0c a5 5a

};



#endif//_CHASSIS_ZHIDI_DRIVER_WANGHONGTAO_2016_08_22_
