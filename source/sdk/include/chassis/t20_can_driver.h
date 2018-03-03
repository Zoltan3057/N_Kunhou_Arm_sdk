#ifndef _CHASSIS_T20_CAN_DRIVER_WANGHONGTAO_2017_08_11_
#define _CHASSIS_T20_CAN_DRIVER_WANGHONGTAO_2017_06_11_

#include "TimerDiff.h"
#include "Comm/buffer_con.hpp"
#include "driver_base.h"
#include "t20_dio.h"
#include "chassis/t20_k_rpm.h"
#include "chassis/t20_k_angle.h"
#include "chassis/l16_k_fork.h"
#include "chassis/usb_can_ld.h"
#include "chassis/canalyst2.h"

class cTransferDevice;

class t20_can_driver : Driver_base
{
public:
	t20_can_driver();
	~t20_can_driver();

	void setAngleOri(int id,F32 rad_angle);
	void setRPMOri(int id,F32 v);
	S32 ReadL16Fork(void);
	void setfork(U32 fun_code_ri, S32 fork_hight_ri);
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

//	F32 c_action_wheel_speed_;
//	F32 c_action_wheel_angle_;

private:
	bool b_run_;
	void th_read();

//	boost::mutex mu_feedback_;
//	ST20_FeedBack feedback_data_;
//	CSBuffer<F32,1> list_speed_;
	SBuffer<F32> list_speed_;
	boost::mutex mu_feedback_;
	F32 wheel_angle_deg_;
	F32 wheel_speed_rpm_;
	int alive_count_;

	void parse_181(U8* Data8);
	void parse_183(U8* Data8);

private:
	cTransferDevice* pTransDevice_;

	void ReadTruckStatus(void);
	void SendData( U8* s_data,U16 len );
	void ReadData(  U8* r_data,int &len,const int &need  );
	bool ModBus_Read(U8* read_data, const int &len);

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
	//bool fork_status_;//bit1
	bool is_forward_;//bit2
	bool is_tiller_;
	bool is_guard_;
	bool is_pedal_;
	bool is_stand_;

	t20_k_rpm k_para;
	t20_k_angle k_angle_para;
	l16_k_fork k_fork_para;
	F32 angle_zero_;
//PID
    F32 set_rpm_;
    F32 rpm_Kp_;
    F32 rpm_Ki_;
    F32 rpm_last_error_;
    F32 rpm_current_error_;
    F32 rpm_error_;

	U8 fork_status_;
	U32 fnc_code_;
	S32 para_;
	S32 dio_fork_status_;
	boost::mutex mu_action;
	boost::mutex mu_st;
	bool hasTask;
	bool isWorking;
	int  ct_;
	int forkpos_offset_;

	cTimerDiff test_dt_;

private://usb can
	usb_can_ld usb_ld_;

	typedef enum{ON_BOARD,USB_LD,USB_BOX} CAN_TYPE;
	CAN_TYPE can_data_type_;
	std::string can_port_;

private://canalyst2
	canalyst2 canalyst2_;

};



#endif//_CHASSIS_T20_HALF_DRIVER_WANGHONGTAO_2017_06_05_
