#ifndef _PF_DRIVER_WANGHONGTAO_20151021_
#define _PF_DRIVER_WANGHONGTAO_20151021_

#include <string>
//#include <boost/serialization/singleton.hpp>

#include "RobotStruct.h"
#include "mmem.h"



class pf_driver{
public:

	pf_driver();
	~pf_driver();
	void clear_irq();

	bool init_particlefilter_device(std::string dev = "/dev/particlefilter");	//init /dev/particle_filter

	bool wait_w_irq();				//wait for weight irq
	bool wait_o_irq();				//wait for odom irq

	int read_particle(unsigned char* data,int len);
	int write_particle(unsigned char* data,int len);

private:



	int pf_fd_;						//fd of particlefilter


};

//typedef boost::serialization::singleton<pf_driver> Singleton_IRQ;

#endif//_PF_DRIVER_WANGHONGTAO_20151021_

