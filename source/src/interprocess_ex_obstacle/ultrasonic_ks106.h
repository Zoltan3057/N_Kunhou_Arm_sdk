#ifndef _ULTRASONIC_KS106_WANGHONGTAO_2017_07_27_
#define _ULTRASONIC_KS106_WANGHONGTAO_2017_07_27_


#include <vector>
#include <map>
#include <boost/thread.hpp>

#include "MyDefine.h"
#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

typedef struct _SKs106{
	U8 add;
	U16 range;
	F32 dis;
}SKs106;

class cTransferDevice;

class ultrasonic_ks106
{
public:

	ultrasonic_ks106();
	~ultrasonic_ks106();

	void init(const std::string port);

	bool get_data();

private:

	bool open_transfer_device();
	void close_device();

	cTransferDevice* pTransDevice_;
	std::string port_;

private:

	void set_channel_fieldangle(int angle);
	bool get_channel_data(int ichannel);
	std::map<int,SKs106> m_channel_;
};


#endif//_ULTRASONIC_KS106_WANGHONGTAO_2017_07_27_
