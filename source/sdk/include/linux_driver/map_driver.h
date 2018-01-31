#ifndef _MAP_DRIVER_WANGHONGTAO_20151021_
#define _MAP_DRIVER_WANGHONGTAO_20151021_

#include <string>
//#include <boost/serialization/singleton.hpp>

#include "RobotStruct.h"
#include "mmem.h"


class map_driver{
public:

	map_driver();
	~map_driver();
	void clear_irq();

	bool init_map_device(std::string dev = "/dev/map_inflation");	//init /dev/map_inflation


	bool wait_laser_tf_irq();			//wait for map inflation irq 63

	bool wait_obstacle_irq();			//wait for map inflation irq 64
	bool wait_path_irq();				//wait for map inflation irq 65
	bool wait_goal_irq();				//wait for map inflation irq 66


private:

	int map_fd_;						//fd of particlefilter

};

//typedef boost::serialization::singleton<pf_driver> Singleton_IRQ;

#endif//_MAP_DRIVER_WANGHONGTAO_20151021_

