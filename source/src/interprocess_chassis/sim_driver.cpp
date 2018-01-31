
#include "math.h"
#include "iostream"
#include "Comm.h"

#include "sim_driver.h"

#define LEN_MAX 1024



sim_driver::sim_driver():counts_(10000),reduction_ratio_(60),one_wheel_counts_(0)
{
	left_speed_ = 0;
	right_speed_ = 0;
	one_wheel_counts_ = reduction_ratio_*counts_;

}

sim_driver::~sim_driver()
{


}

void sim_driver::setPara( std::string name ,std::string value )
{

	
	if(name == "counts"){
		cComm::ConvertToNum(counts_,value);
		cComm::RangeIt(counts_,(U32)0,(U32)100000);
		std::cout<<"counts_:"<<counts_<<std::endl;
	}else if(name == "reduction"){
		cComm::ConvertToNum(reduction_ratio_,value);
		cComm::RangeIt(reduction_ratio_,(F32)0,(F32)200.0);
		std::cout<<"reduction_ratio_:"<<reduction_ratio_<<std::endl;
	}else{
		std::cout<<"err para:"<<name<<" value:"<<value<<std::endl;
	}

	one_wheel_counts_ = reduction_ratio_*counts_;
	std::cout<<"one_wheel_counts_:"<<one_wheel_counts_<<std::endl;
}

void sim_driver::setRPM( int id,F32 v )
{
	//set left driver speed
	S32 speed = 0 ;  //0.1 counts/s

	if (id == 0)
	{
		left_speed_ = v * reduction_ratio_ * counts_ / 60 ;	//counts / s
		c_left_ = v;

	}else{

		right_speed_ = v * reduction_ratio_ * counts_ / 60; //counts / s
		c_right_ = v;
	}

	
}
F32 sim_driver::getSpeed( int id )
{
	if (id == 0)
	{
		return c_left_;
	}else{
		return -c_right_;
	}
}

F32 sim_driver::getDiffAngle( int id )
{
	
	F32 dangle = 0;

	dangle = GetDiffPos(id);
	dangle /= one_wheel_counts_;

	return dangle;

}

F32 sim_driver::GetDiffPos( U8 id )
{
	if (id == 0)
	{
		F64 dt = left_dt_.GetTime();
		dt /= 1000000;
		left_dt_.Begin();

		if(dt > 10){
			dt = 0;
		}
		return left_speed_*dt;
	}else{

		F64 dt = right_dt_.GetTime();
		dt /= 1000000;
		right_dt_.Begin();

		if(dt > 10){
			dt = 0;
		}
		return -right_speed_*dt;
	}
}

void sim_driver::close_driver()
{

}

bool sim_driver::open_transfer_device()
{
	return true;
}

bool sim_driver::init_driver()
{
	left_dt_.Begin();
	right_dt_.Begin();
	return true;
}

void sim_driver::setAngle( int id,F32 rad_angle )
{

}

F32 sim_driver::getAngle( int id )
{
	return 0;
}
