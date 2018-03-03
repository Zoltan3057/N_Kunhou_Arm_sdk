#ifndef _OMNI_45_DRIVER_DONGLEI_2017_12_23_
#define _OMNI_45_DRIVER_DONGLEI_2017_12_23_



#include "TimerDiff.h"
#include "chassis/driver_base.h"

class cTransferDevice;

class omni_45_driver : Driver_base
{
public:
	omni_45_driver();
	~omni_45_driver();

protected:

private:

	std::string port_;
	//SerialPort* sport_;
	cTransferDevice* pTransDevice_;
	F32 reduction_ratio_;
	U32 counts_;
	S32 Row_POS[4];

	U32 one_wheel_counts_;

private://virtual customized :TODO
	void setPara(std::string name ,std::string value);
	bool open_transfer_device();
	bool init_driver();
	void close_driver();

	void setRPM(int id,F32 v);
	F32 getSpeed(int id);

	F32 getDiffAngle(int id);

//	bool first_odo_;

private:
	//copley chassis example, customer should not do like this
	void init_customer_para();

	S32 Get_Begin_Pos(U8 id);
	F32 GetDiffPos(int id );

	void SendData( U8* s_data,U16 len);
	void ReadData( U8* r_data,int &len);
// 	void GetSpeedL();
// 	void GetSpeedR();
	

	F32 Fleft_speed_;
	F32 Fright_speed_;
	F32 Bleft_speed_;
    F32 Bright_speed_;

 //前面两个轮子
	cTimerDiff Fleft_dt_;
	S32 Fleft_pos_;
	S32 Flast_left_pos_;
	S32 Fdeta_left_pos_;

	F32 Fleft_set_v_;
	F32 Fleft_current_v_;

	cTimerDiff Fright_dt_;
	S32 Fright_pos_ ;		    //pos
	S32 Flast_right_pos_;
	S32 Fdeta_right_pos_;

	F32 Fright_set_v_;
	F32 Fright_current_v_;

//后面两个轮子
	cTimerDiff Bleft_dt_;


	S32 Bleft_pos_;
	S32 Blast_left_pos_;
	S32 Bdeta_left_pos_;

	F32 Bleft_set_v_;
	F32 Bleft_current_v_;

	cTimerDiff Bright_dt_;
	S32 Bright_pos_ ;		    //pos
	S32 Blast_right_pos_;
	S32 Bdeta_right_pos_;

	F32 Bright_set_v_;
	F32 Bright_current_v_;



	F32 Fc_left_;
	F32 Fc_right_;
	F32 Bc_left_;
	F32 Bc_right_;

	S32 buffer_speed[4];

};



#endif//_OMNI_45_DRIVER_DONGLEI_2017_12_23_
