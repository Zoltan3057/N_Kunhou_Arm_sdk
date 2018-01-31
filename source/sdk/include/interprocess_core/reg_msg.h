#ifndef _SHARED_REG_WANGHONGTAO_2015_11_04_
#define _SHARED_REG_WANGHONGTAO_2015_11_04_

#include "RobotStruct.h"
#include "interprocess/shared_data.hpp"
#include "interprocess/shared_pool.hpp"

#define LASER_TF_1_STR "laser_tf_1"
#define LASER_TF_2_STR "laser_tf_2"

struct big_data{

	char data[1024*10];
};

typedef struct _sclose{
	int over;
}sclose;


#endif//_SHARED_REG_WANGHONGTAO_2015_11_04_
