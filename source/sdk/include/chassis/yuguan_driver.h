#ifndef _YUGUAN_DRIVER_CHENG_H_
#define _YUGUAN_DRIVER_CHENG_H_

#include "TimerDiff.h"
#include "chassis/driver_base.h"

class cTransferDevice;

class yuguan_driver : Driver_base
{

public:
	yuguan_driver();
	~yuguan_driver();

protected:

private://virtual
	void setPara(std::string name ,std::string value);

	bool init_driver();
	void close_driver();
	void setRPM(int id,F32 v);
	//void setAngle(int id,F32 rad_angle);
	F32 GetDiffPos(int id );
	F32 getAngle( int id );
	bool open_transfer_device();
	bool open_transfer_device_Plc();

	F32 getSpeed(int id);
	F32 getDiffAngle(int id);

private:
	std::string port_;
	cTransferDevice* pTransDevice_;
	std::string port_plc_;
	cTransferDevice* pTransDevice_plc_;
	F32 reduction_ratio_;
	U32 counts_;

	U32 one_wheel_counts_;
	typedef struct _Mot
	{
		S16 state_;
		S16 fwd_;
		S16 speed_;
		F32 c_speed_;
		F32 deta_pos_;

		cTimerDiff dt_;
	}Mot;

	Mot Left1_;
	Mot Right1_;
	Mot Left2_;
	Mot Right2_;

	F32 Angle1_;
	F32 Angle2_;

	U8 send_speed[30];
	U8 get_speed[20];
	U8 get_angle[15];
	U8 ch_read[20];

	void init_customer_para();

	void SendData( U8* s_data,U16 len );
	void ReadData(  U8* r_data,int &len,const int &need  );
	void SendData_Plc( U8* s_data,U16 len );
	void ReadData_Plc(  U8* r_data,int &len,const int &need  );

	void Send_Speed(int id,Mot motor1,Mot motor2);
	bool Get_speed(int id);
	bool Reset_driver(int id);
};

#endif /* _HUAXIAO_DRIVER3_CHENG_H_ */
