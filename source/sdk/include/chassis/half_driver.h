#ifndef _CHASSIS_HALF_DRIVER_CHENG_2017_7_31
#define _CHASSIS_HALF_DRIVER_CHENG_2017_7_31

#include "TimerDiff.h"

#include "chassis/driver_base.h"
#include "chassis/t20_dio.h"

class cTransferDevice;

class half_driver : Driver_base
{
public:
	half_driver();
	~half_driver();

protected:

public:
	modbus_10_data half_MB_10_;
	modbus_03_data half_MB_03_;

	U8 Send_10_[32];
	U8 Send_03_[8];
	U8 num_10_;

private:

	S16 SetAangle_;
	S16 SetSpeed_;
	S16 SetUpDown_;
	bool is_cmd_mode_;//bit0
	bool is_up_;//bit1
	bool is_forward_;//bit2

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
	F32 GetDiffPos(int id );

private:

	void SendData( U8* s_data,U16 len );
	void ReadData(  U8* r_data,int &len,const int &need  );
	bool ModBus_Read(U8* read_data, const int &len);

};


#endif//_CHASSIS_HALF_DRIVER_CHENG_2017_7_31
