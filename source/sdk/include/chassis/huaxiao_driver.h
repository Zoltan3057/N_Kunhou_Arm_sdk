#ifndef _CHASSIS_HUAMIN_DRIVER_WANGHONGTAO_2016_08_22_
#define _CHASSIS_HUAMIN_DRIVER_WANGHONGTAO_2016_08_22_

#include "TimerDiff.h"
#include "chassis/driver_base.h"

class cTransferDevice;

class huaxiao_driver : Driver_base
{
public:
	huaxiao_driver();
	~huaxiao_driver();

protected:


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

	S32 Get_counts();

	F32 GetDiffPos(int id );
	

	void SendData( U8* s_data,U16 len );
	void ReadData( U8* r_data,int &len,int need);
	

	S32 awheel_counts_;
	S32 last_awheel_counts_;
	S32 deta_awheel_counts_;


private:

	std::string port_;

	cTransferDevice* pTransDevice_;
	
	F32 reduction_ratio_;
	U32 counts_;

	U32 one_wheel_counts_;


private:
	void SendComm();

	bool WriteVD(int stationNO, int address, int value);
	bool WriteVW(int stationNO, int address, int value);
	int ReadVD(int stationNO, int length, int address);
};



#endif//_CHASSIS_HUAMIN_DRIVER_WANGHONGTAO_2016_08_22_
