#ifndef _CHASSIS_USB_CAN_LD_2017_07_29_
#define _CHASSIS_USB_CAN_LD_2017_07_29_

#include <boost/thread.hpp>
#include <vector>


#include "TimerDiff.h"

#include "t20_data.h"

class cTransferDevice;

class usb_can_ld
{
public:
	usb_can_ld();
	~usb_can_ld();

	bool init_driver(std::string port);

	bool ReadData(std::vector<can_frame> &frame_list);

protected:

private:



	bool open_transfer_device();
	void close_driver();


	cTransferDevice* pTransDevice_;
	std::string port_;

private:
	bool b_run_;
	void th_read();
	t20_data data_;



};

#endif//_CHASSIS_USB_CAN_LD_2017_07_29_

