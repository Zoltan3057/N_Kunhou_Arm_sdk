#ifndef _CHASSIS_PLC_DRIVER_MULTI_WANGHONGTAO_2017_3_13_
#define _CHASSIS_PLC_DRIVER_MULTI_WANGHONGTAO_2017_3_13_

#include <map>
#include "TimerDiff.h"
#include "chassis/diff_pos_multi.h"

#include "chassis/driver_base.h"

class cTransferDevice;

class plc_driver : Driver_base
{
public:
	plc_driver();
	~plc_driver();

protected:

private:
	
	

	std::string port_;
	//SerialPort* sport_;
	cTransferDevice* pTransDevice_;

	U32 counts_;

	//U32 one_wheel_counts_;

private://virtual customized :TODO
	void setPara(std::string name ,std::string value);
	bool open_transfer_device();
	bool init_driver();
	void close_driver();

	void setRPM(int id,F32 v);
	F32 getSpeed(int id);

	F32 getDiffAngle(int id);
	F32 getDiffAngle(int id,const S32 &pos);

	//angle loop

//	void setAngle(int id,F32 rad_angle);
//	F32 getAngle(int id);
//	bool first_odo_;

private:
	//copley chassis example, customer should not do like this
	void init_customer_para();


	F32 GetDiffPos(int id , S32 pos);
//	F32 GetDiffPosRPM( int id );
// 	void GetSpeedL();
// 	void GetSpeedR();
	F32 GetRatio(int id);
	void SetRatio( int id,const F32 &ratio);

	void SendData( U8* s_data,U16 len );
	void ReadData( U8* r_data,int &len,const int &need);
	
	S32 Rpm2AD(F32 rpm);
	void converToAD(SAO &ao , F32 angle ,const F32 &speed);
	void cal_para_k();
	F32 k_cw_ ;
	F32 k_ccw_ ;
private:

	void init_driver_count(const U32 &add_id);

	bool get_driver_set_ok();
	void clear_driver_set_ok();
	void get_driver_speed(std::map<int,F32> &mspeed);
 	std::map<U32,driver_unit> clt_unit_;

//	chuangmeng_protocl protocl_;

//	bool get_status(const U32 &id);
//	bool clear_fault(const U32 &id);


};



#endif//_CHASSIS_PLC_DRIVER_MULTI_WANGHONGTAO_2017_3_13_
