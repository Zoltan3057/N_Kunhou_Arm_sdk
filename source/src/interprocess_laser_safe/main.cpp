/*
 * main.cpp
 *  Describe :  interprocess_laser_safe  main.cpp
 *  Created on: Dec 21, 2017
 *      Author: neo
 */


#include <signal.h>


#include <iostream>
#include <fstream>

#include <string>
#include <cassert>
#include "TimerDiff.h"

#include "interprocess_core/reg_msg.h"
#include "interprocess/shared_data.hpp"
#include "interprocess/shared_pool.hpp"

#include "mapserver/MapServer.h"

#include "robot/angle_calibration.h"
#include "polygon_ex.h"
#include "Obstacle_Detector.h"

using namespace NS_Laser_Safe;


bool brun = true;

U32 obstacle_min_ = 4;      //框内光点 阀值
F32 rotation_angle_ = 0;   //弧线时框体旋转角度
U32 send_count = 3;         //检测结果变化时，写入prohibitmap次数

Obstacle_Detector obstacle_finder;
Obstacle_Status last_pause = Obstacle_Status::NO_OBSTACLE;   //记录上一次障碍物检测结果

int used_laser_ = 0;
bool use_laser_amcl = false;
bool use_laser_tf1 = false;
bool use_laser_tf2 = false;
bool use_photosensor = false;

SLaser_para amcl_para;
SLaser_para tf1_para;
SLaser_para tf2_para;

Laser_safe amcl_;
Laser_safe tf1_;
Laser_safe tf2_;

int unstop_ = 0;

polygon_ex amcl_polygon_ex_;  //计算激光 min_range
polygon_ex tf1_polygon_ex_;  //计算激光 min_range
polygon_ex tf2_polygon_ex_;  //计算激光 min_range

bool b_first_run_amcl = true;
bool b_first_run_tf1 = true;
bool b_first_run_tf2 = true;

SLaser get_amcl_laser_data;
SLaser get_tf1_laser_data;
SLaser get_tf2_laser_data;

boost::mutex mu_setspeed;
boost::mutex mu_dio;
boost::mutex mu_laser_amcl;
boost::mutex mu_laser_tf1;
boost::mutex mu_laser_tf2;
boost::mutex mu_srun;


SSpeed get_speed;
SDI Sdi;

//laser_safe_shape Parameters
F32 x_redu1 = 0.8,x_stop = 0.4,y_redu1 = 0.2,y_stop = 0.05;
F32 x_redu2 = 0.6,x_buff = 0.6,y_redu2 = 0.2,y_buff = 0.1;

Shape_xy robot_shape;

//24 lase_shape
Shape_xy laser_frontstraight_redu1_shape;
Shape_xy laser_frontstraight_redu2_shape;
Shape_xy laser_frontstraight_buff_shape;
Shape_xy laser_frontstraight_stop_shape;

Shape_xy laser_frontleft_redu1_shape;
Shape_xy laser_frontleft_redu2_shape;
Shape_xy laser_frontleft_buff_shape;
Shape_xy laser_frontleft_stop_shape;

Shape_xy laser_frontright_redu1_shape;
Shape_xy laser_frontright_redu2_shape;
Shape_xy laser_frontright_buff_shape;
Shape_xy laser_frontright_stop_shape;

Shape_xy laser_backstraight_redu1_shape;
Shape_xy laser_backstraight_redu2_shape;
Shape_xy laser_backstraight_buff_shape;
Shape_xy laser_backstraight_stop_shape;

Shape_xy laser_backleft_redu1_shape;
Shape_xy laser_backleft_redu2_shape;
Shape_xy laser_backleft_buff_shape;
Shape_xy laser_backleft_stop_shape;

Shape_xy laser_backright_redu1_shape;
Shape_xy laser_backright_redu2_shape;
Shape_xy laser_backright_buff_shape;
Shape_xy laser_backright_stop_shape;

//手动输入框体顶点坐标, exp robot_shape: "1.62:-0.5;1.62:0.5;-0.2:0.5;-0.2:-0.5;1.62:-0.5;"
std::string test_shapestring = "1.0:-0.5;1.0:-0.1;-1.0:0.5;-1.0:0.1;1.0:-0.5";
std::string str_safe_shape_straight_cut1 = "2.8:-0.5;2.8:0.5;-0.2:0.5;-0.2:-0.5;2.8:-0.5";
std::string str_safe_shape_straight_cut2 = "2.6:-0.5;2.6:0.5;-0.2:0.5;-0.2:-0.5;2.6:-0.5";
std::string str_safe_shape_straight_buff = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_straight_stop = "2.3:-0.5;2.3:0.5;-0.2:0.5;-0.2:-0.5;2.3:-0.5";

std::string str_safe_shape_left_cut1 = "2.8:-0.5;2.8:0.5;-0.2:0.5;-0.2:-0.5;2.8:-0.5";
std::string str_safe_shape_left_cut2 = "2.6:-0.5;2.6:0.5;-0.2:0.5;-0.2:-0.5;2.6:-0.5";
std::string str_safe_shape_left_buff = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_left_stop = "2.3:-0.5;2.3:0.5;-0.2:0.5;-0.2:-0.5;2.3:-0.5";

std::string str_safe_shape_right_cut1 = "2.8:-0.5;2.8:0.5;-0.2:0.5;-0.2:-0.5;2.8:-0.5";
std::string str_safe_shape_right_cut2 = "2.6:-0.5;2.6:0.5;-0.2:0.5;-0.2:-0.5;2.6:-0.5";
std::string str_safe_shape_right_buff = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_right_stop = "2.3:-0.5;2.3:0.5;-0.2:0.5;-0.2:-0.5;2.3:-0.5";


///////////////////////////////////

std::string str_safe_shape_straight_cut1_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_straight_cut2_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_straight_buff_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_straight_stop_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";

std::string str_safe_shape_left_cut1_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_left_cut2_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_left_buff_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_left_stop_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";

std::string str_safe_shape_right_cut1_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_right_cut2_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_right_buff_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";
std::string str_safe_shape_right_stop_r = "2.5:-0.5;2.5:0.5;-0.2:0.5;-0.2:-0.5;2.5:-0.5";


void set_pause_continue( Obstacle_Status &bflag);

void get_laser_para(const std::string pub_str,SLaser_para &para){

	para.laser_ip_ = "0.0.0.0";
	para.reverse_ = false;
	para.laser_start_angle_ = -135;
	para.laser_range_max_ = 50.0;
	para.laser_range_min_ = 0.03;
	para.laser_dx_ = 0;
	para.laser_dy_ = 0;

	if( pub_str == LASER_TF_1_STR ){
		Config::getConfig("laser_tf1_ip",para.laser_ip_);
		Config::getConfig("laser_tf1_reverse",para.reverse_);
		Config::getConfig("laser_tf1_start_angle",para.laser_start_angle_);
		Config::getConfig("laser_tf1_range_max",para.laser_range_max_);
		Config::getConfig("laser_tf1_range_min",para.laser_range_min_);
		Config::getConfig("laser_tf1_dx",para.laser_dx_);
		Config::getConfig("laser_tf1_dy",para.laser_dy_);
	}else if( pub_str == LASER_TF_2_STR ){
		Config::getConfig("laser_tf2_ip",para.laser_ip_);
		Config::getConfig("laser_tf2_reverse",para.reverse_);
		Config::getConfig("laser_tf2_start_angle",para.laser_start_angle_);
		Config::getConfig("laser_tf2_range_max",para.laser_range_max_);
		Config::getConfig("laser_tf2_range_min",para.laser_range_min_);
		Config::getConfig("laser_tf2_dx",para.laser_dx_);
		Config::getConfig("laser_tf2_dy",para.laser_dy_);
	}else{
		Config::getConfig("laser_ip",para.laser_ip_);
		Config::getConfig("laser_reverse",para.reverse_);
		Config::getConfig("laser_start_angle",para.laser_start_angle_);
		Config::getConfig("laser_range_max",para.laser_range_max_);
		Config::getConfig("laser_range_min",para.laser_range_min_);
		Config::getConfig("laser_dx",para.laser_dx_);
		Config::getConfig("laser_dy",para.laser_dy_);
	}
	std::cout<<"laser >>> "<<pub_str<<std::endl;
	std::cout<<"laser ip:"<<para.laser_ip_<<std::endl;
	std::cout<<"laser reverse_:"<<para.reverse_<<std::endl;
	std::cout<<"laser_start_angle_:"<<para.laser_start_angle_<<std::endl;
	std::cout<<"laser_range_max_:"<<para.laser_range_max_<<std::endl;
	std::cout<<"laser_range_min_:"<<para.laser_range_min_<<std::endl;
	std::cout<<"laser_dx_:"<<para.laser_dx_<<std::endl;
	std::cout<<"laser_dy_:"<<para.laser_dy_<<std::endl;

}

void get_robot_shape(U8 i, std::string value1 , std::string value2)
{
	switch(i){
		case 1:
			cComm::ConvertToNum(robot_shape.x1_,value1);
			cComm::ConvertToNum(robot_shape.y1_,value2);
			//cComm::RangeIt(x1,F32(0.0),F32(1.5));
			std::cout<<"x1:"<<robot_shape.x1_<<"  y1:"<<robot_shape.y1_<<std::endl;
			break;
		case 2:
			cComm::ConvertToNum(robot_shape.x2_,value1);
			cComm::ConvertToNum(robot_shape.y2_,value2);
			//cComm::RangeIt(x1,F32(0.0),F32(1.5));
			std::cout<<"x2:"<<robot_shape.x2_<<"  y2:"<<robot_shape.y2_<<std::endl;
			break;
		case 3:
			cComm::ConvertToNum(robot_shape.x3_,value1);
			cComm::ConvertToNum(robot_shape.y3_,value2);
			//cComm::RangeIt(x1,F32(0.0),F32(1.5));
			std::cout<<"x3:"<<robot_shape.x3_<<"  y3:"<<robot_shape.y3_<<std::endl;
			break;
		case 4:
			cComm::ConvertToNum(robot_shape.x4_,value1);
			cComm::ConvertToNum(robot_shape.y4_,value2);
			//cComm::RangeIt(x1,F32(0.0),F32(1.5));
			std::cout<<"x4:"<<robot_shape.x4_<<"  y4:"<<robot_shape.y4_<<std::endl;
			break;
	}
}

//  --从网页中的距离值，计算所有避障框体的定点坐标
void get_laser_shape_dis(std::string name , std::string value)
{
	if(name == "x_redu1"){
		cComm::ConvertToNum(x_redu1,value);
		cComm::RangeIt(x_redu1,F32(0.0),F32(3));
		std::cout<<"x_redu1:"<<x_redu1<<std::endl;
	}else if(name == "x_redu2"){
		cComm::ConvertToNum(x_redu2,value);
		cComm::RangeIt(x_redu2,F32(0.0),F32(3));
		std::cout<<"x_redu2:"<<x_redu2<<std::endl;
	}else if(name == "x_buff"){
		cComm::ConvertToNum(x_buff,value);
		cComm::RangeIt(x_buff,F32(0.0),F32(3));
		std::cout<<"x_buff:"<<x_buff<<std::endl;
	}else if(name == "x_stop"){
		cComm::ConvertToNum(x_stop,value);
		cComm::RangeIt(x_stop,F32(0.0),F32(3));
		std::cout<<"x_stop:"<<x_stop<<std::endl;
	}else if(name == "y_redu1"){
		cComm::ConvertToNum(y_redu1,value);
		cComm::RangeIt(y_redu1,F32(0.0),F32(3));
		std::cout<<"y_redu1:"<<y_redu1<<std::endl;
	}else if(name == "y_redu2"){
		cComm::ConvertToNum(y_redu2,value);
		cComm::RangeIt(y_redu2,F32(0.0),F32(3));
		std::cout<<"y_redu2:"<<y_redu2<<std::endl;
	}else if(name == "y_stop"){
		cComm::ConvertToNum(y_stop,value);
		cComm::RangeIt(y_stop,F32(0.0),F32(3));
		std::cout<<"y_stop:"<<y_stop<<std::endl;
	}else if(name == "y_buff"){
		cComm::ConvertToNum(y_buff,value);
		cComm::RangeIt(y_buff,F32(0.0),F32(3));
		std::cout<<"y_buff:"<<y_buff<<std::endl;
	}else if(name == "rotation_angle"){
		cComm::ConvertToNum(rotation_angle_,value);
		cComm::RangeIt(rotation_angle_,F32(0.0),F32(90));
		std::cout<<"rotation_angle:"<<rotation_angle_<<std::endl;
	}else{
		std::cout<<"err name:"<<name<<" para:"<<value<<std::endl;
	}
}

void getConfigPara()
{
	std::cout<<"Start Config Para!***"<<std::endl;
	Config::getConfig("obstacle_min",obstacle_min_);

	std::string robot_shape = "1.2:-0.25;1.2:0.25;-1.2:0.25;-1.2:-0.25;1.2:-0.25";
	Config::getConfig("robot_shape",robot_shape);
	std::vector<std::string> robot_sh;
	cComm::SplitString(robot_shape,";",robot_sh);
	std::vector<std::string>::iterator it_r = robot_sh.begin();
	U8 i=1;
	for ( ; it_r != robot_sh.end() ; ++it_r)
	{
		std::string &para_pair = *it_r;
		if (para_pair.length())
		{
			std::vector<std::string> vpara2;
			cComm::SplitString(para_pair,":",vpara2);
			if (vpara2.size() > 1)
			{
				get_robot_shape(i, vpara2[0], vpara2[1]);
			}
		}
		i++;
	}


	std::string laser_shape = "x_redu1:0.6;x_redu2:0.5;x_buff:0.5;x_stop:0.4;y_redu1:0.2;y_redu2:0.1;y_buff:0.1;y_stop:0.05;rotation_angle:30";
	Config::getConfig("laser_shape_distance",laser_shape);
	std::vector<std::string> laser_sh;
	cComm::SplitString(laser_shape,";",laser_sh);
	std::vector<std::string>::iterator it_l = laser_sh.begin();
	for ( ; it_l != laser_sh.end() ; ++it_l)
	{
		std::string &para_pair = *it_l;
		if (para_pair.length())
		{
			std::vector<std::string> vpara2;
			cComm::SplitString(para_pair,":",vpara2);
			if (vpara2.size() > 1)
			{
				get_laser_shape_dis(vpara2[0],vpara2[1]);
			}
		}
	}


	std::string str_laser_inf_msg = "laser";
	Config::getConfig("laser_inf_msg",str_laser_inf_msg);
	std::cout<<"laser_inf_msg:"<<str_laser_inf_msg<<std::endl;
	std::vector<std::string> vmsg;
	cComm::SplitString(str_laser_inf_msg,":",vmsg);
	std::vector<std::string>::iterator it = vmsg.begin();
	for ( ; it != vmsg.end() ; ++it)
	{
		if (*it == "laser")
		{
			used_laser_++;
			use_laser_amcl = true;
			std::cout<<"laser inf used: amcl laser"<<std::endl;
		}else if(*it == LASER_TF_1_STR){
			used_laser_++;
			use_laser_tf1 = true;
			std::cout<<"laser inf used: tf1 laser"<<std::endl;
		}else if(*it == LASER_TF_2_STR){
			used_laser_++;
			use_laser_tf2 = true;
			std::cout<<"laser inf used: tf2 laser"<<std::endl;
		}else if(*it == "photosensor"){
			use_photosensor = true;
			obstacle_finder.usephotosensor(true);
			std::cout<<"use photosensor"<<std::endl;
		}
	}

	get_laser_para("laser",amcl_para);
	get_laser_para(LASER_TF_1_STR,tf1_para);
	get_laser_para(LASER_TF_2_STR,tf2_para);

	amcl_.laser_location_  = Direction::FRONT;
	tf1_.laser_location_   = Direction::BACK;
	tf2_.laser_location_   = Direction::FRONT;
	obstacle_finder.setPara(obstacle_min_);
	//----- Test  ----  ////
	//------------------////

	std::cout<<"Finish Config Para!***"<<std::endl;
}

void shutdown(int sig)
{
	std::cout<<"ctrl c shut down"<<std::endl;
	brun = false;
	shared_pool::destructor();
	SLEEP(500);

	return;
}

void amcl_laser_first(const SLaser &used_laser_data, Laser_safe &laser_safe, SLaser_para &laser_para);
void tf1_laser_first(const SLaser &used_laser_data, Laser_safe &laser_safe, SLaser_para &laser_para);
void tf2_laser_first(const SLaser &used_laser_data, Laser_safe &laser_safe, SLaser_para &laser_para);


void update_amcl_laser(const SLaser &used_laser_data){
	boost::mutex::scoped_lock lock(mu_laser_amcl);
	if (b_first_run_amcl)
	{
		get_amcl_laser_data = used_laser_data;
		b_first_run_amcl = false;
		std::cout<<"call back first run ,init amcl_laser range."<<std::endl;
	}else{
		amcl_.laser_data_ = used_laser_data;
	}
}
void update_tf1_laser(const SLaser &used_laser_data){
	boost::mutex::scoped_lock lock(mu_laser_tf1);
	if (b_first_run_tf1)
	{
		get_tf1_laser_data = used_laser_data;
		b_first_run_tf1 = false;
		std::cout<<"call back first run ,init tf1 range."<<std::endl;
	}else{

	tf1_.laser_data_ = used_laser_data;
	}
}
void update_tf2_laser(const SLaser &used_laser_data){
	boost::mutex::scoped_lock lock(mu_laser_tf2);
	if(b_first_run_tf2 )
	{
		get_tf2_laser_data = used_laser_data;
		b_first_run_tf2 = false;
		std::cout<<"call back first run ,init tf2 range."<<std::endl;
	}else{
		tf2_.laser_data_ = used_laser_data;
	}

}

void callback(const sclose &cl){

	if (cl.over)
	{
		std::cout<<"core shut down:"<<shared_pool::name()<<std::endl;
		shutdown(1);
	}

}

void callSpeed(const SSpeed &speed)
{
	boost::mutex::scoped_lock lock(mu_setspeed);
	get_speed = speed;
}

void calldio(const SDI &sdi)
{
	boost::mutex::scoped_lock lock(mu_dio);
	Sdi = sdi;
}
void update_amcl_entropy(const SAMCL_RES &res){
//	std::cout<<"res.entropy_:"<<res.entropy_<<std::endl;
	F32 f_safe_entropy = 1000;
	Config::getConfig("safe_entropy",f_safe_entropy);
//	std::cout<<"f_safe_entropy:"<<f_safe_entropy<<std::endl;

	if( res.entropy_ > f_safe_entropy ){
		F32 vx = 0;
		F32 vw = 0;
		{
			boost::mutex::scoped_lock lock(mu_setspeed);
			vx = get_speed.vx_;
			vw = get_speed.vw_;
		}
		if( (fabs(vx) > 0.001) || (fabs(vw) > 0.001)){

			U8 data[] = {1};
			U32 len = 1;

			shared_pool::Shared_Pipe_Push( PROHIBIT_MAP ,"layer1",data,len);
			std::cout<<"update_amcl_entropy set pause!"<<std::endl;
//			Obstacle_Status status_comb = Obstacle_Status::OBSTACLE_STOP;
//			set_pause_continue(status_comb);
		}

	}
}
void init_shared_pool(char *argv[]){

	shared_pool::init(argv);
	boost::function<void( const sclose &cl)> fnc_shutdown;
	fnc_shutdown = boost::bind(callback,_1);
	shared_pool::Subscribe(shared_pool::name(),"shutdown",fnc_shutdown);

	boost::function<void( const SSpeed &speed )> fnc_SetSpeed;
	fnc_SetSpeed = boost::bind(callSpeed,_1);
	shared_pool::Subscribe(shared_pool::name(),"setspeed",fnc_SetSpeed);

	boost::function<void( const SDI &sdi )> fnc_SDI;
	fnc_SDI = boost::bind(calldio,_1);
	shared_pool::Subscribe(shared_pool::name(),"gpio_di",fnc_SDI);

	boost::function<void( const SRunStatus &runstate )> fnc_runstate;
	fnc_runstate = boost::bind(&Obstacle_Detector::UpdateRunStatus,&obstacle_finder,_1);
	shared_pool::Subscribe(shared_pool::name(),"run_status",fnc_runstate);

	//subscribe laser amcl data
	if (use_laser_amcl)
	{
		boost::function<void( const SLaser &laser_data )> fnc_amcl;
		fnc_amcl = boost::bind(update_amcl_laser,_1);
		shared_pool::Subscribe(shared_pool::name(),"laser",fnc_amcl);
	}

	//subscribe laser tf1 data
	if(use_laser_tf1){
		boost::function<void( const SLaser &laser_data )> fnc_tf1;
		fnc_tf1 = boost::bind(update_tf1_laser,_1);
		shared_pool::Subscribe(shared_pool::name(),LASER_TF_1_STR,fnc_tf1);
	}

	//subscribe laser tf2 data
	if(use_laser_tf2){
		boost::function<void( const SLaser &laser_data )> fnc_tf2;
		fnc_tf2 = boost::bind(update_tf2_laser,_1);
		shared_pool::Subscribe(shared_pool::name(),LASER_TF_2_STR,fnc_tf2);
	}

	boost::function<void( const SAMCL_RES &pos )> fnc_amcl_res;
	fnc_amcl_res = boost::bind(update_amcl_entropy,_1);
	shared_pool::Subscribe(shared_pool::name(),"amcl_entropy",fnc_amcl_res);

}

bool check_status(Obstacle_Status st,Obstacle_Status s1,Obstacle_Status s2,Obstacle_Status s3){
	if((st==s1)||(st==s2)||(st==s2)){
		return true;
	}
	return false;

}

void set_pause_continue( Obstacle_Status &bflag){

	if ( last_pause != bflag )
	{
		send_count = 3;
		if(last_pause == Obstacle_Status::OBSTACLE_STOP){
			if(unstop_>2){
				unstop_=0;
				last_pause = bflag;
			}else{
				bflag = last_pause;
			}
			unstop_ ++;
		}else{
			last_pause = bflag;
		}
	}

	if ( send_count > 0)
	{
		send_count--;
	}else
	{
		return;
	}

	U8 data[] = {5};
	U32 len = 1;

	data[0] = bflag;

	//std::cout<<"status: "<<(int)data[0]<<std::endl;
	shared_pool::Shared_Pipe_Push( PROHIBIT_MAP ,"layer1",data,len);
	if(bflag == Obstacle_Status::OBSTACLE_STOP){
		SLEEP(500);
	}
//	Srecoder pause;
//	memset(pause.cmd_file_,0,200);
//	memcpy(pause.cmd_file_,str.c_str(),str.length() );
//	shared_pool::Publish(shared_pool::name(),"pause",pause);

}

int main(int argc, char *argv[])
{
	bool b_test = false;

	::signal(SIGINT, shutdown);

	getConfigPara();

	//init shared pool
	if(argc > 1){

		init_shared_pool(argv);

	}
	//SLEEP(100);

	cTimerDiff dt;

	Obstacle_Status status_comb = Obstacle_Status::NO_OBSTACLE;

	Obstacle_Status status_amcl = Obstacle_Status::NO_OBSTACLE;
	Obstacle_Status status_tf1 = Obstacle_Status::NO_OBSTACLE;
	Obstacle_Status status_tf2 = Obstacle_Status::NO_OBSTACLE;


	obstacle_finder.setFrames(robot_shape,x_redu1,x_redu2,x_buff,x_stop,y_redu1,y_redu2,y_buff,y_stop,rotation_angle_);

	bool init_lasershape = false;
	int cnt = 0;
#if 0
	while(brun){

		if(init_lasershape== false){
			{
				boost::mutex::scoped_lock lock(mu_laser_amcl);
				if(b_first_run_amcl == false){
					amcl_laser_first(get_amcl_laser_data, amcl_, amcl_para);
					cnt++;
				}
			}

			{
				boost::mutex::scoped_lock lock(mu_laser_tf1);
				if(b_first_run_tf1 == false){
					tf1_laser_first(get_tf1_laser_data, tf1_, tf1_para);
					cnt++;
				}
			}
			{
				boost::mutex::scoped_lock lock(mu_laser_tf2);
				if(b_first_run_tf2==false){
					tf2_laser_first(get_tf2_laser_data, tf2_, tf2_para);
					cnt++;
				}
			}
			if(cnt>=used_laser_){
				init_lasershape = true;
				std::cout<<"************************laser count: "<<cnt<<std::endl;
			}
			SLEEP(50);
		}else{
				F32 vx,vw;
				{
					boost::mutex::scoped_lock lock(mu_setspeed);
					vx = get_speed.vx_;
					vw = get_speed.vw_;
				}

				if(use_photosensor){
					boost::mutex::scoped_lock lock(mu_dio);
					obstacle_finder.SetDI(Sdi);
				}
				if ((use_laser_amcl)&&(b_first_run_amcl == false))
				{
					boost::mutex::scoped_lock lock(mu_laser_amcl);
					obstacle_finder.check_Obstacle(vx,vw,amcl_,status_amcl);
					//std::cout<<">>>>>Amcl status<<<: "<<(int)status_amcl<<std::endl;
				}
				if ((use_laser_tf1)&&(b_first_run_tf1==false))
				{
					boost::mutex::scoped_lock lock(mu_laser_tf1);
					obstacle_finder.check_Obstacle(vx,vw,tf1_,status_tf1);
					//std::cout<<">>>>>Tf1 status<<<<<<<<: "<<(int)status_tf1<<std::endl;

				}
				if ((use_laser_tf2)&&(b_first_run_tf2==false))
				{
					boost::mutex::scoped_lock lock(mu_laser_tf2);
					obstacle_finder.check_Obstacle(vx,vw,tf2_,status_tf2);
					//std::cout<<"Tf2 status: "<<(int)status_tf2<<std::endl;
				}

				if(check_status(Obstacle_Status::OBSTACLE_STOP,status_amcl,status_tf1,status_tf2)){
					status_comb = Obstacle_Status::OBSTACLE_STOP;
				}else if(check_status(Obstacle_Status::OBSTACLE_REDUCE2,status_amcl,status_tf1,status_tf2)){
					status_comb = Obstacle_Status::OBSTACLE_REDUCE2;
				}else if(check_status(Obstacle_Status::OBSTACLE_REDUCE1,status_amcl,status_tf1,status_tf2)){
					status_comb = Obstacle_Status::OBSTACLE_REDUCE1;
				}else{
					status_comb = Obstacle_Status::NO_OBSTACLE;
				}

				if(argc > 1){
					set_pause_continue(status_comb);
				}

		}

		dt.ms_loop(50);   //20ms loop , 50hz

	}
#else
	while(brun){
		if(init_lasershape== false){
			{
				boost::mutex::scoped_lock lock(mu_laser_amcl);
				if(b_first_run_amcl == false){
					obstacle_finder.init_lasersafe(get_amcl_laser_data,amcl_,amcl_para);
					cnt++;
				}
			}

			{
				boost::mutex::scoped_lock lock(mu_laser_tf1);
				if(b_first_run_tf1 == false){
					obstacle_finder.init_lasersafe(get_tf1_laser_data,tf1_,tf1_para);
					cnt++;
				}
			}
			{
				boost::mutex::scoped_lock lock(mu_laser_tf2);
				if(b_first_run_tf2==false){
					obstacle_finder.init_lasersafe(get_tf2_laser_data,tf2_,tf2_para);
					cnt++;
				}
			}
			if(cnt>=used_laser_){
				init_lasershape = true;
				std::cout<<"************************laser count: "<<cnt<<std::endl;
			}
			SLEEP(50);

			//test
//			{
//				get_amcl_laser_data.start_angle_ = Deg2Rad(-135);
//				get_amcl_laser_data.resolution_ = Deg2Rad(0.5);
//				get_amcl_laser_data.range_max_  = 20;
//				get_amcl_laser_data.range_min_  = 0.01;
//				SLEEP(5000);
//				obstacle_finder.init_lasersafe(get_amcl_laser_data,amcl_,amcl_para);
//				return 0;
//
//			}
		}else{
				F32 vx,vw;
				{
					boost::mutex::scoped_lock lock(mu_setspeed);
					vx = get_speed.vx_;
					vw = get_speed.vw_;
				}

				if(use_photosensor){
					boost::mutex::scoped_lock lock(mu_dio);
					obstacle_finder.SetDI(Sdi);
				}
				if ((use_laser_amcl)&&(b_first_run_amcl == false))
				{
					boost::mutex::scoped_lock lock(mu_laser_amcl);
					obstacle_finder.check_Obstacle(vx,vw,amcl_,status_amcl);
					//std::cout<<">>>>>Amcl status<<<: "<<(int)status_amcl<<std::endl;
				}
				if ((use_laser_tf1)&&(b_first_run_tf1==false))
				{
					boost::mutex::scoped_lock lock(mu_laser_tf1);
					obstacle_finder.check_Obstacle(vx,vw,tf1_,status_tf1);
					//std::cout<<">>>>>Tf1 status<<<<<<<<: "<<(int)status_tf1<<std::endl;

				}
				if ((use_laser_tf2)&&(b_first_run_tf2==false))
				{
					boost::mutex::scoped_lock lock(mu_laser_tf2);
					obstacle_finder.check_Obstacle(vx,vw,tf2_,status_tf2);
					//std::cout<<"Tf2 status: "<<(int)status_tf2<<std::endl;
				}

				if(check_status(Obstacle_Status::OBSTACLE_STOP,status_amcl,status_tf1,status_tf2)){
					status_comb = Obstacle_Status::OBSTACLE_STOP;
				}else if(check_status(Obstacle_Status::OBSTACLE_REDUCE2,status_amcl,status_tf1,status_tf2)){
					status_comb = Obstacle_Status::OBSTACLE_REDUCE2;
				}else if(check_status(Obstacle_Status::OBSTACLE_REDUCE1,status_amcl,status_tf1,status_tf2)){
					status_comb = Obstacle_Status::OBSTACLE_REDUCE1;
				}else{
					status_comb = Obstacle_Status::NO_OBSTACLE;
				}

				if(argc > 1){
					set_pause_continue(status_comb);
				}

		}

		dt.ms_loop(50);   //20ms loop , 50hz
	}

#endif
	return 0;
}


void amcl_laser_first(const SLaser &used_laser_data, Laser_safe &laser_safe, SLaser_para &laser_para)
{
	laser_safe.laser_range_straight_redu1_ = used_laser_data;
	laser_safe.laser_range_straight_redu2_ = used_laser_data;
	laser_safe.laser_range_straight_buff_ = used_laser_data;
	laser_safe.laser_range_straight_stop_ = used_laser_data;
	laser_safe.laser_range_left_redu1_ = used_laser_data;
	laser_safe.laser_range_left_redu2_ = used_laser_data;
	laser_safe.laser_range_left_buff_ = used_laser_data;
	laser_safe.laser_range_left_stop_ = used_laser_data;
	laser_safe.laser_range_right_redu1_ = used_laser_data;
	laser_safe.laser_range_right_redu2_ = used_laser_data;
	laser_safe.laser_range_right_buff_ = used_laser_data;
	laser_safe.laser_range_right_stop_ = used_laser_data;

	laser_safe.laser_range_straight_redu1_r_ = used_laser_data;
	laser_safe.laser_range_straight_redu2_r_ = used_laser_data;
	laser_safe.laser_range_straight_buff_r_ = used_laser_data;
	laser_safe.laser_range_straight_stop_r_ = used_laser_data;
	laser_safe.laser_range_left_redu1_r_ = used_laser_data;
	laser_safe.laser_range_left_redu2_r_ = used_laser_data;
	laser_safe.laser_range_left_buff_r_ = used_laser_data;
	laser_safe.laser_range_left_stop_r_ = used_laser_data;
	laser_safe.laser_range_right_redu1_r_ = used_laser_data;
	laser_safe.laser_range_right_redu2_r_ = used_laser_data;
	laser_safe.laser_range_right_buff_r_ = used_laser_data;
	laser_safe.laser_range_right_stop_r_ = used_laser_data;

	std::cout<<"Laser Shape polygon with laser dx:"<<laser_para.laser_dx_<<" dy:"<<laser_para.laser_dy_<<std::endl;

	amcl_polygon_ex_.init_shape(str_safe_shape_straight_cut1);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_straight_cut2);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_straight_buff);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_straight_stop);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	amcl_polygon_ex_.init_shape(str_safe_shape_left_cut1);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_left_cut2);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_left_buff);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_left_stop);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	amcl_polygon_ex_.init_shape(str_safe_shape_right_cut1);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_right_cut2);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_right_buff);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_right_stop);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_stop_,laser_para.laser_dx_,laser_para.laser_dy_);


	amcl_polygon_ex_.init_shape(str_safe_shape_straight_cut1_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_straight_cut2_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_straight_buff_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_straight_stop_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_straight_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	amcl_polygon_ex_.init_shape(str_safe_shape_left_cut1_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_left_cut2_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_left_buff_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_left_stop_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_left_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	amcl_polygon_ex_.init_shape(str_safe_shape_right_cut1_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_right_cut2_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_right_buff_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	amcl_polygon_ex_.init_shape(str_safe_shape_right_stop_r);
	amcl_polygon_ex_.get_range(laser_safe.laser_range_right_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);


}

void tf1_laser_first(const SLaser &used_laser_data, Laser_safe &laser_safe, SLaser_para &laser_para)
{
	laser_safe.laser_range_straight_redu1_ = used_laser_data;
	laser_safe.laser_range_straight_redu2_ = used_laser_data;
	laser_safe.laser_range_straight_buff_ = used_laser_data;
	laser_safe.laser_range_straight_stop_ = used_laser_data;
	laser_safe.laser_range_left_redu1_ = used_laser_data;
	laser_safe.laser_range_left_redu2_ = used_laser_data;
	laser_safe.laser_range_left_buff_ = used_laser_data;
	laser_safe.laser_range_left_stop_ = used_laser_data;
	laser_safe.laser_range_right_redu1_ = used_laser_data;
	laser_safe.laser_range_right_redu2_ = used_laser_data;
	laser_safe.laser_range_right_buff_ = used_laser_data;
	laser_safe.laser_range_right_stop_ = used_laser_data;

	laser_safe.laser_range_straight_redu1_r_ = used_laser_data;
	laser_safe.laser_range_straight_redu2_r_ = used_laser_data;
	laser_safe.laser_range_straight_buff_r_ = used_laser_data;
	laser_safe.laser_range_straight_stop_r_ = used_laser_data;
	laser_safe.laser_range_left_redu1_r_ = used_laser_data;
	laser_safe.laser_range_left_redu2_r_ = used_laser_data;
	laser_safe.laser_range_left_buff_r_ = used_laser_data;
	laser_safe.laser_range_left_stop_r_ = used_laser_data;
	laser_safe.laser_range_right_redu1_r_ = used_laser_data;
	laser_safe.laser_range_right_redu2_r_ = used_laser_data;
	laser_safe.laser_range_right_buff_r_ = used_laser_data;
	laser_safe.laser_range_right_stop_r_ = used_laser_data;

	std::cout<<"Laser Shape polygon with laser dx:"<<laser_para.laser_dx_<<" dy:"<<laser_para.laser_dy_<<std::endl;

	tf1_polygon_ex_.init_shape(str_safe_shape_straight_cut1);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_straight_cut2);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_straight_buff);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_straight_stop);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf1_polygon_ex_.init_shape(str_safe_shape_left_cut1);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_left_cut2);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_left_buff);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_left_stop);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf1_polygon_ex_.init_shape(str_safe_shape_right_cut1);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_right_cut2);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_right_buff);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_right_stop);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_stop_,laser_para.laser_dx_,laser_para.laser_dy_);


	tf1_polygon_ex_.init_shape(str_safe_shape_straight_cut1_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_straight_cut2_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_straight_buff_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_straight_stop_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_straight_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf1_polygon_ex_.init_shape(str_safe_shape_left_cut1_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_left_cut2_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_left_buff_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_left_stop_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_left_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf1_polygon_ex_.init_shape(str_safe_shape_right_cut1_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_right_cut2_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_right_buff_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf1_polygon_ex_.init_shape(str_safe_shape_right_stop_r);
	tf1_polygon_ex_.get_range(laser_safe.laser_range_right_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);


}

void tf2_laser_first(const SLaser &used_laser_data, Laser_safe &laser_safe, SLaser_para &laser_para)
{
	laser_safe.laser_range_straight_redu1_ = used_laser_data;
	laser_safe.laser_range_straight_redu2_ = used_laser_data;
	laser_safe.laser_range_straight_buff_ = used_laser_data;
	laser_safe.laser_range_straight_stop_ = used_laser_data;
	laser_safe.laser_range_left_redu1_ = used_laser_data;
	laser_safe.laser_range_left_redu2_ = used_laser_data;
	laser_safe.laser_range_left_buff_ = used_laser_data;
	laser_safe.laser_range_left_stop_ = used_laser_data;
	laser_safe.laser_range_right_redu1_ = used_laser_data;
	laser_safe.laser_range_right_redu2_ = used_laser_data;
	laser_safe.laser_range_right_buff_ = used_laser_data;
	laser_safe.laser_range_right_stop_ = used_laser_data;

	laser_safe.laser_range_straight_redu1_r_ = used_laser_data;
	laser_safe.laser_range_straight_redu2_r_ = used_laser_data;
	laser_safe.laser_range_straight_buff_r_ = used_laser_data;
	laser_safe.laser_range_straight_stop_r_ = used_laser_data;
	laser_safe.laser_range_left_redu1_r_ = used_laser_data;
	laser_safe.laser_range_left_redu2_r_ = used_laser_data;
	laser_safe.laser_range_left_buff_r_ = used_laser_data;
	laser_safe.laser_range_left_stop_r_ = used_laser_data;
	laser_safe.laser_range_right_redu1_r_ = used_laser_data;
	laser_safe.laser_range_right_redu2_r_ = used_laser_data;
	laser_safe.laser_range_right_buff_r_ = used_laser_data;
	laser_safe.laser_range_right_stop_r_ = used_laser_data;

	std::cout<<"Laser Shape polygon with laser dx:"<<laser_para.laser_dx_<<" dy:"<<laser_para.laser_dy_<<std::endl;

	tf2_polygon_ex_.init_shape(str_safe_shape_straight_cut1);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_straight_cut2);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_straight_buff);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_straight_stop);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf2_polygon_ex_.init_shape(str_safe_shape_left_cut1);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_left_cut2);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_left_buff);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_left_stop);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf2_polygon_ex_.init_shape(str_safe_shape_right_cut1);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_right_cut2);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_right_buff);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_right_stop);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_stop_,laser_para.laser_dx_,laser_para.laser_dy_);


	tf2_polygon_ex_.init_shape(str_safe_shape_straight_cut1_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_straight_cut2_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_straight_buff_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_straight_stop_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_straight_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf2_polygon_ex_.init_shape(str_safe_shape_left_cut1_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_left_cut2_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_left_buff_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_left_stop_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_left_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	tf2_polygon_ex_.init_shape(str_safe_shape_right_cut1_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_right_cut2_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_right_buff_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	tf2_polygon_ex_.init_shape(str_safe_shape_right_stop_r);
	tf2_polygon_ex_.get_range(laser_safe.laser_range_right_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);
}

