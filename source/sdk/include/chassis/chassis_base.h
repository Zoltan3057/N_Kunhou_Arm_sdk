#ifndef _CHASSIS_BASE_WANGHONGTAO_2015_09_16_
#define _CHASSIS_BASE_WANGHONGTAO_2015_09_16_



#include "MyDefine.h"
#include <boost/thread.hpp>

#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

#include "robot/angle_calibration.h"

// class Chassis_para;
// {
// public:
// 	Chassis_para(){
// 		name_ = "";
// 		type_ = Chassis_para::STR;
// 		dp_ = 0.0;
// 		lp_ = 0;
// 		sp_ = "";
// 	};
// 	~Chassis_para();
// 
// 	typedef enum{FLOAT,INT,STR} PARA_TYPE;
// 
// protected:
// private:
// 	std::string name_;
// 	PARA_TYPE type_;
// 	double dp_;
// 	long   lp_;
// 	std::string sp_;
// 
// };
class Driver_base;

class Chassis_base
{
public:

	Chassis_base();
	~Chassis_base();

	void setSpeed(F32 vx, F32 vy, F32 vw);
	void setSpeed(SSpeed speed);

	void getOdom(F32 &ox, F32 &oy, F32 &oth);
	void getOdom(SPos &odom);

	void getSpeed(F32 &vx, F32 &vy, F32 &vw);
	void getSpeed(SSpeed &speed);

	void set_dio(const SDIAI &dio);

	bool init( std::string para );
	bool initDriver(Driver_base* driver);

	void run(int ms_loop);
	void stop();

protected:

	void init();

	virtual void setPara(std::string name , std::string value) = 0;

	//std::map<std::string name,Chassis_para> map_para_;

private:

	//get odom in Base link (origin point is robot rotation center)
	virtual void getOdomInBaseLink(F32 &dx,F32 &dy, F32 &th) = 0;
	virtual void getOdomSpeed(F32 &vx,F32 &vy, F32 &th) = 0;
	virtual void setOdomSpeed(F32 vx, F32 vy, F32 vw) = 0;
	

	SBuffer<SSpeed> set_speed_list_; //set speed list
	boost::mutex mu_odom_;
	SPos odom_;						 //robot odom
	SSpeed current_speed_;			 //robot current speed
	

private:

	cTimerDiff dt_;
	U16 ms_loop_;	//thread loop sleep
	bool thread_run_;
	void thread_loop();

	void calNewOdom(const F32 &dx,const F32 &dy,const F32 &dth);
	void setNewSpeed(const F32 &vx,const F32 &vy,const F32 &th);

protected:// driver base

	//virtual void setDriverPara( std::string para ) = 0;
	
	
	Driver_base* driver_;
	boost::mutex mu_dio_;
	SDIAI extern_dio_;						 //extern io
	U32 extern_dio_count_;
	
	bool get_angle_ori( const S32 &id,F32 &angle );
	bool get_angle(const S32 &id,F32 &rad);
	bool get_pos(const S32 &id,S32 &pos);

	U32  motion_type_;

private:

	angle_manage angle;

};


#endif//_CHASSIS_BASE_WANGHONGTAO_2015_09_16_
