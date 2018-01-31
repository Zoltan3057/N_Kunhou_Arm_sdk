#ifndef _LASER_DRIVER_WANGHONGTAO_20151021_
#define _LASER_DRIVER_WANGHONGTAO_20151021_

#include <string>
//#include <boost/serialization/singleton.hpp>

#include "RobotStruct.h"
#include "mmem.h"



class laser_driver{
public:

	laser_driver();
	~laser_driver();
	void clear_irq();

	bool init_laser_device(std::string dev = "/dev/laser_tf");	//init /dev/map_inflation


	bool wait_laser_tf_irq();			//wait for map inflation irq 63

	bool wait_laser_tf_1_irq();         //wait for map inflation irq 87
	bool wait_laser_tf_2_irq();         //wait for map inflation irq 88

private:

	int laser_fd_;						//fd of particlefilter

};

//typedef boost::serialization::singleton<pf_driver> Singleton_IRQ;

#endif//_LASER_DRIVER_WANGHONGTAO_20151021_

