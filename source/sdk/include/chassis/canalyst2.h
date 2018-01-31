#ifndef _CHASSIS_CANALYST_2018_08_28_
#define _CHASSIS_CANALYST_2018_08_28_

#include <boost/thread.hpp>
#include <vector>
#include <linux/can.h>

#include "buffer_con.hpp"
#include "controlcan.h"

#include "TimerDiff.h"

class canalyst2
{
public:
	canalyst2();
	~canalyst2();

	bool init_driver();

	bool ReadData(std::vector<can_frame> &frame_list);
	bool SendData(can_frame frame);

protected:

private:

	bool open_transfer_device();
	void close_driver();



private:

	bool b_run_;
	void th_read();
	boost::mutex mu_read_;
	boost::mutex mu_write_;
	boost::mutex mu_get_;

	VCI_BOARD_INFO pInfo_;
	VCI_CAN_OBJ rec_[2100];

	CSBuffer<can_frame,100> can_list_;
};

#endif//_CHASSIS_CANALYST_2018_08_28_

