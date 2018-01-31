/*
 * smj_driver.h
 *
 *  Created on: 21/05/2017
 *      Author: edwardzhang
 */

#ifndef smj_DRIVER_H_
#define smj_DRIVER_H_


#include "TimerDiff.h"
#include "chassis/driver_base.h"

class cTransferDevice;

class smj_driver : Driver_base
{
public:
	smj_driver();
	~smj_driver();

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

	void setAngle(int id,F32 rad_angle);
	F32 getAngle(int id);

private:

	void init_customer_para();


	F32 GetDiffPos(int id );
	void GetSpeedLR();

// 	void GetSpeedL();
// 	void GetSpeedR();

	void SendData( U8* s_data,U16 len );
	void ReadData(   U8* r_data,int &len,const int &need );

    void CANopen_Send_Command ( U32 OP, U32 Index_low, U32 Index_high ,U32 SubIndex, S32 data);
    void CANopen_readPos ( );

	bool first_odo_;
	cTimerDiff dt_;
	cTimerDiff left_dt_;

	S32 left_pos_;
	S32 last_left_pos_;
	S32 deta_left_pos_;

	S32 right_pos_ ;		    //pos
	S32 last_right_pos_;
	S32 deta_right_pos_;

	F32 c_left_;
	F32 c_right_;


	U32 COBID;
	U8 CANmsg_send[16];
	U8 CANmsg_receive[1024];


};


#endif /* smj_DRIVER_H_ */
