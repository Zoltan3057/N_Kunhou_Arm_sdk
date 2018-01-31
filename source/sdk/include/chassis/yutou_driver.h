#ifndef _YUTOU_DRIVER_H_
#define _YUTOU_DRIVER_H_

#include "TimerDiff.h"
#include "Comm/buffer_con.hpp"
#include "driver_base.h"
#include "chassis/canalyst2.h"

class cTransferDevice;

class yutou_driver : Driver_base
{
public:
	yutou_driver();
	~yutou_driver();

protected:

private://virtual customized :TODO
	void setPara(std::string name ,std::string value);

	bool init_driver();
	bool open_transfer_device();
	void close_driver();
	std::string port_;

	F32 reduction_ratio_;


	void setRPM(int id,F32 v);
	F32 getSpeed(int id);

	F32 getDiffAngle(int id);

	void setAngle(int id,F32 rad_angle);
	

	F32 getAngle(int id);

private:

	cTimerDiff dt_;
	F32 dangle_;

private:
	bool b_run_;
	void th_read();



	void parse_203(U8* Data8);
	void parse_207(U8* Data8);

private:
	cTransferDevice* pTransDevice_;

	void SendData( U8* s_data,U16 len );
	void ReadData(  U8* r_data,int &len,const int &need  );

private:

	S16 SetAangle_;
	S16 SetSpeed_;
	S16 SetUpDown_;
	boost::mutex speed_feedback_;
	boost::mutex angle_feedback_;
	F32 wheel_angle_deg_;
	F32 wheel_speed_rpm_;
	int alive_count_;

private://can type

	typedef enum{ON_BOARD,USB_LD,USB_BOX} CAN_TYPE;
	CAN_TYPE can_data_type_;
	std::string can_port_;

private://canalyst2
	canalyst2 canalyst2_;

};



#endif//_CHASSIS_T20_HALF_DRIVER_WANGHONGTAO_2017_06_05_
