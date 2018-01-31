#ifndef _GPIO_WANGHONGTAO_20160603_
#define _GPIO_WANGHONGTAO_20160603_

#include <boost/serialization/singleton.hpp>

#include <string>

#include "RobotStruct.h"




class gpio{
public:

	gpio();
	~gpio();
	

	void init_DI(std::vector<int> pin_list);
	void init_DO(std::vector<int> pin_list);

	bool set_gpio_on(int pin);
	bool set_gpio_off(int pin);

	bool export_gpio(int pin,bool b_out);
	bool unexport_gpio(int pin);

	int get_gpio(int pin);

};


typedef boost::serialization::singleton<gpio> Singleton_GPIO;

#endif//_GPIO_WANGHONGTAO_20160603_

