/*
 * Obstacle_Detector.h
 *  Describe: 输入激光数据、速度数据，计算避障框内有无障碍物,返回 障碍物信息
 *  Created on: Dec 21, 2017
 *      Author: neo
 */

#ifndef _OBSTACLE_DETECTOR_H_
#define _OBSTACLE_DETECTOR_H_


#include <math.h>
#include <vector>
#include <list>
#include <map>
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "RobotStruct.h"
#include "Geometry.h"
#include <iostream>
#include "boost/thread/mutex.hpp"


namespace NS_Laser_Safe{

/*核心结构体:  包含实时激光数据、24个避障框*/
typedef struct _Laser_safe
{
	SLaser laser_data_;    //s实时激光数据

	SLaser laser_range_straight_redu1_;
	SLaser laser_range_straight_redu2_;
	SLaser laser_range_straight_buff_;
	SLaser laser_range_straight_stop_;
	SLaser laser_range_left_redu1_;
	SLaser laser_range_left_redu2_;
	SLaser laser_range_left_buff_;
	SLaser laser_range_left_stop_;
	SLaser laser_range_right_redu1_;
	SLaser laser_range_right_redu2_;
	SLaser laser_range_right_buff_;
	SLaser laser_range_right_stop_;

	SLaser laser_range_straight_redu1_r_;
	SLaser laser_range_straight_redu2_r_;
	SLaser laser_range_straight_buff_r_;
	SLaser laser_range_straight_stop_r_;
	SLaser laser_range_left_redu1_r_;
	SLaser laser_range_left_redu2_r_;
	SLaser laser_range_left_buff_r_;
	SLaser laser_range_left_stop_r_;
	SLaser laser_range_right_redu1_r_;
	SLaser laser_range_right_redu2_r_;
	SLaser laser_range_right_buff_r_;
	SLaser laser_range_right_stop_r_;

	SLaser laser_range_left_straight_redu1_;
	SLaser laser_range_left_straight_redu2_;
	SLaser laser_range_left_straight_buff_;
	SLaser laser_range_left_straight_stop_;

	SLaser laser_range_right_straight_redu1_;
	SLaser laser_range_right_straight_redu2_;
	SLaser laser_range_right_straight_buff_;
	SLaser laser_range_right_straight_stop_;
	Direction  laser_location_;     //激光在车体上的位置

} Laser_safe;

 //避障框体方向
typedef enum{
	FRONT_STRAIGHT = 0,
	FRONT_LEFT     = 1,
	FRONT_RIGHT    = 2,
	BACK_STRAIGHT  = 3,
	BACK_LEFT      = 4,
	BACK_RIGHT     = 5,
	LEFT_STRAIGHT  = 6,
	RIGHT_STRAIGHT = 7,
	UNKNOWN        = 8    //当planner_tray处于IDEL状态时，框体方向是未知的，不作障碍物判断
}SHAPE_DIR;

//避障框体类型
typedef enum{
	REDU1_AREA  = 0,
	REDU2_AREA  = 1,
	STOP_BUFFER = 2,
	STOP_AREA   = 3,
}SHAPE_TYPE;

//障碍物检测结果
typedef enum{
	IDEL   = 0,           // laser_safe 不可以 ////
	SET_STOP   =1,        // 设置这两种状态    ////
	OBSTACLE_REDUCE1  =2,
	OBSTACLE_REDUCE2  =3,
	OBSTACLE_STOP  = 4,
	NO_OBSTACLE  = 5,

}Obstacle_Status;

//障碍物传感器类型
typedef enum{
	LASER = 0,
	PHOTOSENSOR =1,

}Obstacle_SensorType;


typedef struct _Safe_Frames_{
	 std::vector<Sxy> front_straight_redu1_;
	 std::vector<Sxy> front_straight_redu2_;
	 std::vector<Sxy> front_straight_buff_;
	 std::vector<Sxy> front_straight_stop_;
	 std::vector<Sxy> front_left_redu1_;
	 std::vector<Sxy> front_left_redu2_;
	 std::vector<Sxy> front_left_buff_;
	 std::vector<Sxy> front_left_stop_;
	 std::vector<Sxy> front_right_redu1_;
	 std::vector<Sxy> front_right_redu2_;
	 std::vector<Sxy> front_right_buff_;
	 std::vector<Sxy> front_right_stop_;

	 std::vector<Sxy> back_straight_redu1_;
	 std::vector<Sxy> back_straight_redu2_;
	 std::vector<Sxy> back_straight_buff_;
	 std::vector<Sxy> back_straight_stop_;
	 std::vector<Sxy> back_left_redu1_;
	 std::vector<Sxy> back_left_redu2_;
	 std::vector<Sxy> back_left_buff_;
	 std::vector<Sxy> back_left_stop_;
	 std::vector<Sxy> back_right_redu1_;
	 std::vector<Sxy> back_right_redu2_;
	 std::vector<Sxy> back_right_buff_;
	 std::vector<Sxy> back_right_stop_;

	 std::vector<Sxy> left_straight_redu1_;
	 std::vector<Sxy> left_straight_redu2_;
	 std::vector<Sxy> left_straight_buff_;
	 std::vector<Sxy> left_straight_stop_;

	 std::vector<Sxy> right_straight_redu1_;
	 std::vector<Sxy> right_straight_redu2_;
	 std::vector<Sxy> right_straight_buff_;
	 std::vector<Sxy> right_straight_stop_;


}Safe_Frames;

}


class Obstacle_Detector
{
public:
	Obstacle_Detector();
	~Obstacle_Detector();
	void setPara(F32 obstacle_min);
	static void s_check_Obstacle(const NS_Laser_Safe::Laser_safe&laser_safe_info,
			                    NS_Laser_Safe::Obstacle_Status &cs,int set,int &total);

	bool check_Obstacle(const F32 &vx, const F32 &vy,const F32 &vw,
						const NS_Laser_Safe::Laser_safe&laser_safe_shape,
						NS_Laser_Safe::Obstacle_Status &cs);

	NS_Laser_Safe::Obstacle_Status GetStatus();
	void usephotosensor(bool use);
	void SetDI(const SDI di);
	void UpdateRunStatus(const SRunStatus&runstate );
	void init_lasersafe(SLaser &used_laser_data, NS_Laser_Safe::Laser_safe &laser_safe, SLaser_para &laser_para);

	void setFrames(Shape_xy robot_shape,F32 r1_x,F32 r2_x,F32 b_x,F32 s_x,F32 r1_y,F32 r2_y,F32 b_y,F32 s_y,F32 rotate_angle);
private:
	void initfnc();
	void check_FrontStraight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
							NS_Laser_Safe::Obstacle_Status &cs);
	void check_FrontLeft(const NS_Laser_Safe::Laser_safe&laser_safe_info,
							NS_Laser_Safe::Obstacle_Status &cs);
	void check_FrontRight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
							NS_Laser_Safe::Obstacle_Status &cs);
	void check_BackStraight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
							NS_Laser_Safe::Obstacle_Status &cs);
	void check_BackLeft(const NS_Laser_Safe::Laser_safe&laser_safe_info,
							NS_Laser_Safe::Obstacle_Status &cs);
	void check_BackRight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
							NS_Laser_Safe::Obstacle_Status &cs);

	void check_LeftStraight(const NS_Laser_Safe::Laser_safe&laser_safe_info,NS_Laser_Safe::Obstacle_Status &cs);
	void check_RightStraight(const NS_Laser_Safe::Laser_safe&laser_safe_info,NS_Laser_Safe::Obstacle_Status &cs);
	/*   Laser Range Finder -> check_data()
	 *  only laser_data  < laser_range ,return true;*/
	bool check_data( SLaser &laser_data, SLaser &laser_range,const F32 angle = 0.0);

	/* PhotoSensor ->check_data()
	 * only di is '1',return true */
	bool check_data(const SDI di);

	void preProcessSpeed(const F32 &vx,const F32 &vy,const F32 &vw,NS_Laser_Safe::SHAPE_DIR &dir);

	//function: get laser_range
	void lasersafe_init_shape( const std::string &str_shape );
	void lasersafe_init_shape(const std::vector<Sxy> &shape);
	void lasersafe_get_range(SLaser& min_range,F32 dx,F32 dy);
	bool lasersafe_getCrossLine2(Line &ln,VecPosition ray,VecPosition& shape_vertex1,VecPosition& shape_vertex2,VecPosition&close_shape_vertex,bool& check);
	bool lasersafe_find_ray_in_seg(std::vector<Sxy>::iterator &it_begin,std::vector<Sxy>::iterator &it_end,const VecPosition& ray,VecPosition& close_to,bool& check);

	void shapexy_to_vector(const Shape_xy shapexy/*In:Robotshape*/,std::vector<Sxy> &shape/*Out:Sxy vector*/,F32 x_diff,F32 y_diff,bool front/*true:left, false: right*/);
	void shapexy_to_vector(const Shape_xy shapexy/*In:Robotshape*/,std::vector<Sxy> &shape/*Out:Sxy vector*/,F32 x_diff,F32 y_diff,F32 angle/*In:rotate_angle*/,bool front/*true:front, false: back*/);
	void outputshape(SLaser shape,F32 laser_dx,F32 laser_dy);

	//for yg only
	static bool check_data(SLaser &laser_data, SLaser &laser_range,int set,int &total);
private:
	typedef boost::function< void(const NS_Laser_Safe::Laser_safe&laser_safe_info,
								  	  NS_Laser_Safe::Obstacle_Status &cs) > CALL;

	//bind direction with check_function();
	std::map<NS_Laser_Safe::SHAPE_DIR, Obstacle_Detector::CALL > check_Calls_;

	NS_Laser_Safe::SHAPE_DIR last_dir_;
	NS_Laser_Safe::SHAPE_DIR current_dir_;

	NS_Laser_Safe::Obstacle_Status last_state_;
	NS_Laser_Safe::Obstacle_Status current_state_;
	bool planner_is_idel_;

	F32 rotation_angle_;
	S32 obstacle_min_;
	bool use_photosensor_;
	SDI SDi_;
	boost::mutex mu_run_state;

	bool b_test_;

	std::vector<Sxy> shape_list_;
	std::ofstream test_out;
	NS_Laser_Safe::Safe_Frames frames_;
};








#endif

