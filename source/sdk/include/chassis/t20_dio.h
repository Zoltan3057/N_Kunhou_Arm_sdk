#ifndef _CHASSIS_T20_DIO_CHENG_2017_07_29_
#define _CHASSIS_T20_DIO_CHENG_2017_07_29_

#include <boost/thread.hpp>
#include <vector>

#include "TimerDiff.h"

class cTransferDevice;

typedef struct  _ModBus_10_Data
{
	U8 id_;
	U8 Cmd_;
	U8 Fun_code_;
	U16 reg_addr_;
	U16 reg_num_;
	U8 bit_num;
	std::vector<S16> Send_10_data;

}modbus_10_data;
typedef struct  _ModBus_03_Data
{
	U8 id_;
	U8 Cmd_;
	U16 reg_addr_;
	U16 reg_num_;

}modbus_03_data;


class t20_dio
{
public:
	t20_dio();
	~t20_dio();

protected:

public:

	static void ModBus_10_Send(modbus_10_data &data_10, U8* Send_10_, U8 &num_10_);
	static void ModBus_03_Send(modbus_03_data &data_03, U8* Send_03_);

};

#endif//_CHASSIS_T20_DIO_CHENG_2017_07_29_

