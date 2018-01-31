#ifndef _CHASSIS_AICHONG_DRIVER_
#define _CHASSIS_AICHONG_DRIVER_

#include "TimerDiff.h"
#include "chassis/driver_base.h"

class cTransferDevice;

class aichong_driver : Driver_base
{
public:
	aichong_driver();
	~aichong_driver();

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
	void setAngle(int, float);
	float getAngle(int);


//	bool first_odo_;

private:
	//copley chassis example, customer should not do like this
	void init_customer_para();


	F32 GetDiffPos(int id );
	
	unsigned short CRC16(unsigned char *p, unsigned short len);

	void SendData( U8* s_data,U16 len );
	void ReadData( U8* r_data,int &len,int need,int timeout );
	
	cTimerDiff left_dt_;

	cTimerDiff ct_;
	F32 left_speed_;
	S32 left_pos_;  //pos			  
	S32 last_left_pos_;
	S32 deta_left_pos_;

	F32 left_set_v_;
	F32 left_current_v_;
	F32 c_left_;
	
	cTimerDiff right_dt_;	
	F32 right_speed_;
	S32 right_pos_ ;
	S32 last_right_pos_;
	S32 deta_right_pos_;

	F32 right_set_v_;
	F32 right_current_v_;
	F32 c_right_;

};



#endif
