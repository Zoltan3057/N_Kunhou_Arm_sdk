#ifndef _WINNER_RBT_DRIVER_H_
#define _WINNER_RBT_DRIVER_H_

#include "TimerDiff.h"
#include "chassis/driver_base.h"

//
#define  SPEED_SEND_SIZE   9   
#define  COUNT_SEND_SIZE   7

//
class cTransferDevice;

//
class WinnerRbtDriver: Driver_base
{
public:
	WinnerRbtDriver();
	~WinnerRbtDriver();

protected:
	void setPara(std::string  name,
		         std::string  value);

	bool open_transfer_device();

	bool init_driver();



public:
	void close_driver();

	void setRPM(S32 id,
		        F32 v);

	F32 getSpeed(S32 id);

	F32 getDiffAngle(S32 id);

private: 
	int calValue(U8  *res,
		         S32 start,
		         S32 end);

	void geneSpeedCMD(F32 left_speed,
			          F32 right_speed);

	void createPkg();

	bool prasePkg(U8  *res,
		          S32  len);

	F32 getDiffPos(S32 id );

private:
	void sendData(U8   *s_data, 
		          U16   len);

	void readData(U8  *r_data,
		          int  expect_len,
		          int &len);

	U16 calCRC16(U8  *p,
		         U32  len);

private:	
	std::string      port_;
	F32              reduction_ratio_;
	U32              counts_;
	cTransferDevice *pTransDevice_;
 
private:
	// left
	cTimerDiff left_dt_;
	S32        left_pos_;
	S32        last_left_pos_;
	F32        left_speed_;
	F32        c_left_;

	// right
	cTimerDiff right_dt_;
	S32        right_pos_;	
	S32        last_right_pos_;
	F32        right_speed_;
	F32        c_right_;
	
private:	
	U8  set_speed_cmd_[SPEED_SEND_SIZE];
	U8  get_counts_cmd_[COUNT_SEND_SIZE];  // 1b 20 01 00 9a 71 05
};


#endif//_WINNER_RBT_DRIVER_H_
