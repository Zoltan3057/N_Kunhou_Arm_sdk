#ifndef SONGLING_CHENG_H_
#define SONGLING_CHENG_H_

#include "TimerDiff.h"
#include "chassis/driver_base.h"
#include "chassis/canalyst2.h"

class cTransferDevice;

class songling_driver: Driver_base
{
public:
	songling_driver();
	~songling_driver();

protected:

private:

	std::string port_;
	//SerialPort* sport_;
	cTransferDevice* pTransDevice_;
	F32 reduction_ratio_;
	U32 counts_;

	U32 one_wheel_counts_;


private:
	typedef struct _wh_para
	{
		cTimerDiff dt_;

		S32 pos_;
		S32 last_pos_;
		S32 deta_pos_;
		F32 c_speed_;
	}wh_para;

	wh_para left1_;
	wh_para right1_;
	wh_para left2_;
	wh_para right2_;

	S32 l1_pos_;
	S32 r1_pos_;
	S32 l2_pos_;
	S32 r2_pos_;

	U32 COBID;
	U8 CANmsg_send[16];
	U8 CANmsg_receive[1024];

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

	F32 GetDiffPos(int id, wh_para &wheel);

// 	void GetSpeedL();
// 	void GetSpeedR();

	void SendData( U8* s_data,U16 len );
	void ReadData(   U8* r_data,int &len,const int &need );


	void Canopen_NMT_setOP ();
    void CANopen_Send_SDO ( U8 OP, U8 Index_low, U8 Index_high , U8 SubIndex , U8 data1);
    void CANopen_Send_PDO ( S16 data);
    void CANopen_readPos ( );

	bool b_run_;

	bool first_odo_[4];

private://canalyst2
	canalyst2 canalyst2_;

};




#endif /* SONGLING_CHENG_H_ */
