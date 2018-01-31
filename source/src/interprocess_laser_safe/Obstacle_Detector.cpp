/*
 * Obstacle_Detector.cpp
 *
 *  Created on: Dec 21, 2017
 *      Author: neo
 */

#include "Obstacle_Detector.h"

#include "my_tf.h"

#include "Comm.h"

Obstacle_Detector::Obstacle_Detector():	rotation_angle_(20),obstacle_min_(5),use_photosensor_(false),planner_is_idel_(true)
{
	initfnc();
	b_test_ = false;
}
Obstacle_Detector::~Obstacle_Detector()
{


}
void Obstacle_Detector::setPara(F32 obstacle_min)
{
    if(obstacle_min>0){
    	obstacle_min_ = obstacle_min;
    }
	std::cout<<"obstacle_min_: "<<obstacle_min_<<std::endl;
}
bool Obstacle_Detector::check_Obstacle(const F32 &vx, const F32 &vw,
										const NS_Laser_Safe::Laser_safe&laser_safe_info,
										NS_Laser_Safe::Obstacle_Status &cs)
{
	preProcessSpeed(vx,vw, current_dir_);
	bool is_same = false;
	if(current_dir_<3){
		if(laser_safe_info.laser_location_ == Direction::FRONT){
			is_same = true;
		}
	}else{
		if(laser_safe_info.laser_location_ == Direction::BACK){
			is_same = true;
		}
	}

	if((use_photosensor_ == true)&&(current_dir_>2)&&(current_dir_<6)){
		if(check_data(SDi_)){
			//std::cout<<"di find obstacle!"<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return true;
		}
	}

	//激光方向和运行方向一致 且 planner处于非IDEL，才检测激光数据;否则不检测;
	if(is_same&&(current_dir_!=NS_Laser_Safe::SHAPE_DIR::UNKNOWN)){
		Obstacle_Detector::CALL checkCall = check_Calls_[current_dir_];
		checkCall(laser_safe_info,cs);
		//std::cout<<"Check result : "<<int(cs)<<std::endl;
		current_state_ = cs;
		last_state_ =  current_state_;

	}else{
		//std::cout<<" notsame   or  planneris idel"<<std::endl;
		cs = NS_Laser_Safe::Obstacle_Status::NO_OBSTACLE;
		current_state_ = cs;
		last_state_ =  current_state_;
	}

}

void Obstacle_Detector::initfnc()
{
	Obstacle_Detector::CALL fnc;

	// FRONT_STRAIGHT
	fnc = boost::bind(&Obstacle_Detector::check_FrontStraight, this, _1, _2);
	check_Calls_[NS_Laser_Safe::SHAPE_DIR::FRONT_STRAIGHT] = fnc;
	// FRONT_LEFT
	fnc = boost::bind(&Obstacle_Detector::check_FrontLeft, this, _1, _2);
	check_Calls_[NS_Laser_Safe::SHAPE_DIR::FRONT_LEFT] = fnc;
	// FRONT_RIGHT
	fnc = boost::bind(&Obstacle_Detector::check_FrontRight, this, _1, _2);
	check_Calls_[NS_Laser_Safe::SHAPE_DIR::FRONT_RIGHT] = fnc;
	// BACK_STRAIGHT
	fnc = boost::bind(&Obstacle_Detector::check_BackStraight, this, _1, _2);
	check_Calls_[NS_Laser_Safe::SHAPE_DIR::BACK_STRAIGHT] = fnc;
	//BACK_LEFT
	fnc = boost::bind(&Obstacle_Detector::check_BackLeft, this, _1, _2);
	check_Calls_[NS_Laser_Safe::SHAPE_DIR::BACK_LEFT] = fnc;
	// BACK_RIGHT
	fnc = boost::bind(&Obstacle_Detector::check_BackRight, this, _1, _2);
	check_Calls_[NS_Laser_Safe::SHAPE_DIR::BACK_RIGHT] = fnc;

}

void Obstacle_Detector::UpdateRunStatus(const SRunStatus&runstate )
{
	boost::mutex::scoped_lock lock(mu_run_state);
	if(runstate.status_ == 0){
		planner_is_idel_ = true;
	}else{
		planner_is_idel_ = false;
	}
}
void Obstacle_Detector::check_FrontStraight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
											NS_Laser_Safe::Obstacle_Status &cs)
{//std::cout<<"check frontstraight "<<std::endl;

	SLaser laser_data = laser_safe_info.laser_data_;
	SLaser laser_range_stop = laser_safe_info.laser_range_straight_stop_;
	SLaser laser_range_redu1 = laser_safe_info.laser_range_straight_redu1_;
	SLaser laser_range_redu2 = laser_safe_info.laser_range_straight_redu2_;
	SLaser laser_range_buff = laser_safe_info.laser_range_straight_buff_;

	//上次停止框障碍物
	if(last_state_ == NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP)
	{//std::cout<<"last find obstacle stop,,,check frontstraight buffer"<<std::endl;
		if(check_data(laser_data,laser_range_buff)){
			//std::cout<<"find obstacle in  frontstraight buffer "<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
		//std::cout<<"check frontstraight buffer res: "<<int(cs)<<std::endl;
	}

		//完整检测
		//检测制动框 有无障碍物
		if(check_data(laser_data,laser_range_stop)){
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			//std::cout<<"obstacle in  frontstraight stop"<<std::endl;
			return ;
		}

		//检测减速2框 有无障碍物
		if(check_data(laser_data,laser_range_redu2)){
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE2;
			//std::cout<<"obstacle in  frontstraight redu2"<<std::endl;
			return;
		}else{
			//检测减速1框 有无障碍物
			if(check_data(laser_data,laser_range_redu1)){
				cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE1;
				//std::cout<<"obstacle in  frontstraight redu1"<<std::endl;
			}else{
				cs = NS_Laser_Safe::Obstacle_Status::NO_OBSTACLE;
			}

		}

	//std::cout<<"check frontstraight over res: "<<int(cs)<<std::endl;
}

void Obstacle_Detector::check_FrontLeft(const NS_Laser_Safe::Laser_safe&laser_safe_info,
						NS_Laser_Safe::Obstacle_Status &cs)
{
	//std::cout<<"check frontleft "<<std::endl;
	SLaser laser_data = laser_safe_info.laser_data_;
	SLaser laser_range_stop = laser_safe_info.laser_range_left_stop_;
	SLaser laser_range_redu1 = laser_safe_info.laser_range_left_redu1_;
	SLaser laser_range_redu2 = laser_safe_info.laser_range_left_redu2_;
	SLaser laser_range_buff = laser_safe_info.laser_range_left_buff_;

	//上次停止框障碍物
	if(last_state_ == NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP)
	{//std::cout<<"last find obstacle stop,check frontleftbuffer"<<std::endl;
		if(check_data(laser_data,laser_range_buff)){
			//std::cout<<"obstacle in  frontleft buffer "<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
	}

	//完整检测
	{
		//检测制动框 有无障碍物
		if(check_data(laser_data,laser_range_stop)){
			//std::cout<<"find obstacle in  frontleft stop"<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
		//检测减速2框 有无障碍物
		if(check_data(laser_data,laser_range_redu2)){
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE2;
			//std::cout<<"obstacle in  frontleft redu2"<<std::endl;
			return;
		}else{
			//检测减速1框 有无障碍物
			if(check_data(laser_data,laser_range_redu1)){
				cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE1;
				//std::cout<<"obstacle in  frontleft redu1"<<std::endl;
			}else{
				cs = NS_Laser_Safe::Obstacle_Status::NO_OBSTACLE;
			}

		}
	}
	//std::cout<<"check frontleft over res: "<<int(cs)<<std::endl;
}
void Obstacle_Detector::check_FrontRight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
						NS_Laser_Safe::Obstacle_Status &cs)
{

	SLaser laser_data = laser_safe_info.laser_data_;
	SLaser laser_range_stop = laser_safe_info.laser_range_right_stop_;
	SLaser laser_range_redu1 = laser_safe_info.laser_range_right_redu1_;
	SLaser laser_range_redu2 = laser_safe_info.laser_range_right_redu2_;
	SLaser laser_range_buff = laser_safe_info.laser_range_right_buff_;

	//上次停止框障碍物
	if(last_state_ == NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP)
	{
		if(check_data(laser_data,laser_range_buff)){
			//std::cout<<"obstacle in  frontright buffer "<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
	}

	//完整检测
	{
		//检测制动框 有无障碍物
		if(check_data(laser_data,laser_range_stop)){
			//std::cout<<"find obstacle in  frontright stop"<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
		//检测减速2框 有无障碍物
		if(check_data(laser_data,laser_range_redu2)){
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE2;
			//std::cout<<"obstacle in  frontright redu2"<<std::endl;
			return;
		}else{
			//检测减速1框 有无障碍物
			if(check_data(laser_data,laser_range_redu1)){
				cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE1;
				//std::cout<<"obstacle in  frontright redu1"<<std::endl;
			}else{
				cs = NS_Laser_Safe::Obstacle_Status::NO_OBSTACLE;
			}

		}
	}
	//std::cout<<"check frontright over res: "<<int(cs)<<std::endl;
}
void Obstacle_Detector::check_BackStraight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
						NS_Laser_Safe::Obstacle_Status &cs)
{

	SLaser laser_data = laser_safe_info.laser_data_;
	SLaser laser_range_stop = laser_safe_info.laser_range_straight_stop_r_;
	SLaser laser_range_redu1 = laser_safe_info.laser_range_straight_redu1_r_;
	SLaser laser_range_redu2 = laser_safe_info.laser_range_straight_redu2_r_;
	SLaser laser_range_buff = laser_safe_info.laser_range_straight_buff_r_;

	//上次停止框障碍物
	if(last_state_ == NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP)
	{
		if(check_data(laser_data,laser_range_buff)){
			//std::cout<<"obstacle in  backstraight buffer "<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
	}

	//完整检测
	{
		//检测制动框 有无障碍物
		if(check_data(laser_data,laser_range_stop)){
			//std::cout<<"find obstacle in  backstraight stop"<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
		//检测减速2框 有无障碍物
		if(check_data(laser_data,laser_range_redu2)){
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE2;
			//std::cout<<"obstacle in  backstraight redu2"<<std::endl;
			return;
		}else{
			//检测减速1框 有无障碍物
			if(check_data(laser_data,laser_range_redu1)){
				cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE1;
				//std::cout<<"obstacle in  backstraight redu1"<<std::endl;
			}else{
				cs = NS_Laser_Safe::Obstacle_Status::NO_OBSTACLE;
			}

		}
	}

	//std::cout<<"check backstraight over res: "<<int(cs)<<std::endl;
}
void Obstacle_Detector::check_BackLeft(const NS_Laser_Safe::Laser_safe&laser_safe_info,
						NS_Laser_Safe::Obstacle_Status &cs)
{

	SLaser laser_data = laser_safe_info.laser_data_;
	SLaser laser_range_stop = laser_safe_info.laser_range_left_stop_r_;
	SLaser laser_range_redu1 = laser_safe_info.laser_range_left_redu1_r_;
	SLaser laser_range_redu2 = laser_safe_info.laser_range_left_redu2_r_;
	SLaser laser_range_buff = laser_safe_info.laser_range_left_buff_r_;

	//上次停止框障碍物
	if(last_state_ == NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP)
	{
		if(check_data(laser_data,laser_range_buff)){
			//std::cout<<"obstacle in  backleft buffer "<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
	}

	//完整检测
	{
		//检测制动框 有无障碍物
		if(check_data(laser_data,laser_range_stop)){
			//std::cout<<"find obstacle in  backleft stop"<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
		//检测减速2框 有无障碍物
		if(check_data(laser_data,laser_range_redu2)){
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE2;
			//std::cout<<"obstacle in  backleft redu2"<<std::endl;
			return;
		}else{
			//检测减速1框 有无障碍物
			if(check_data(laser_data,laser_range_redu1)){
				cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE1;
				//std::cout<<"obstacle in  backleft redu1"<<std::endl;
			}else{
				cs = NS_Laser_Safe::Obstacle_Status::NO_OBSTACLE;
			}

		}
	}
	//std::cout<<"check backleft over res: "<<int(cs)<<std::endl;
}
void Obstacle_Detector::check_BackRight(const NS_Laser_Safe::Laser_safe&laser_safe_info,
						NS_Laser_Safe::Obstacle_Status &cs)
{

	SLaser laser_data = laser_safe_info.laser_data_;
	SLaser laser_range_stop = laser_safe_info.laser_range_right_stop_r_;
	SLaser laser_range_redu1 = laser_safe_info.laser_range_right_redu1_r_;
	SLaser laser_range_redu2 = laser_safe_info.laser_range_right_redu2_r_;
	SLaser laser_range_buff = laser_safe_info.laser_range_right_buff_r_;

	//上次停止框障碍物
	if(last_state_ == NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP)
	{
		if(check_data(laser_data,laser_range_buff)){
			//std::cout<<"obstacle in  backright buffer "<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
	}

	//完整检测
	{
		//检测制动框 有无障碍物
		if(check_data(laser_data,laser_range_stop)){
			//std::cout<<"find obstacle in  backright stop"<<std::endl;
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_STOP;
			return ;
		}
		//检测减速2框 有无障碍物
		if(check_data(laser_data,laser_range_redu2)){
			cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE2;
			//std::cout<<"obstacle in  backright redu2"<<std::endl;
			return;
		}else{
			//检测减速1框 有无障碍物
			if(check_data(laser_data,laser_range_redu1)){
				cs = NS_Laser_Safe::Obstacle_Status::OBSTACLE_REDUCE1;
				//std::cout<<"obstacle in  backright redu1"<<std::endl;
			}else{
				cs = NS_Laser_Safe::Obstacle_Status::NO_OBSTACLE;
			}

		}
	}
	//std::cout<<"check backright over res: "<<int(cs)<<std::endl;
}



void Obstacle_Detector::preProcessSpeed(const F32 &vx, const F32 &vw,NS_Laser_Safe::SHAPE_DIR &dir)
{
	{
		boost::mutex::scoped_lock lock(mu_run_state);
		if(planner_is_idel_ == true){
			dir = NS_Laser_Safe::SHAPE_DIR::UNKNOWN;
			return ;
		}
	}

	if(vx>0.05){
		dir = NS_Laser_Safe::SHAPE_DIR::FRONT_STRAIGHT;
		if(vw>0.1){
			dir = NS_Laser_Safe::SHAPE_DIR::FRONT_LEFT;
		}else if(vw<-0.1){
			dir = NS_Laser_Safe::SHAPE_DIR::FRONT_RIGHT;
		}
		last_dir_ = dir;
	}else if(vx<-0.05){
		dir = NS_Laser_Safe::SHAPE_DIR::BACK_STRAIGHT;
		if(vw>0.1){
			dir = NS_Laser_Safe::SHAPE_DIR::BACK_LEFT;
		}else if(vw<-0.1){
			dir = NS_Laser_Safe::SHAPE_DIR::BACK_RIGHT;
		}
		last_dir_ = dir;
	}else{
		dir = last_dir_;
	}
	//std::cout<<"current dir: "<<int(dir)<<std::endl;
}

bool Obstacle_Detector::check_data( SLaser &laser_data, SLaser &laser_range,const F32 angle){

	int obstacle = 0;

	int current_index = 0;

	//std::cout<<"laser_data_.resolution_:"<<laser_data_.resolution_<<" laser_data_.range_min_:"<<laser_data_.range_min_ <<" laser_data_.range_max_:"<<laser_data_.range_max_<<std::endl;
	int diff_index = angle  / laser_data.resolution_;
	//std::cout<<"current angle:"<<angle<<" diff_index:"<<diff_index<<std::endl;

	for (int i = 0  ; i < LASER_COUNT ; ++i )
	{

		current_index = i + diff_index;

		if( (current_index < 0) || (current_index >= LASER_COUNT)){
			continue;
		}

		if( laser_data.data_[current_index] < laser_data.range_min_){
			//std::cout<<" laser:"<<laser_data_.data_[current_index]<<" min:"<<laser_data_.range_min_<<std::endl;
			continue;
		}
		if( laser_data.data_[current_index] > laser_data.range_max_){
			continue;
		}

		if (  laser_data.seg_[current_index]   )
		{

			if( laser_data.data_[current_index] < laser_range.data_[i] ){
				obstacle++;
				F32 angle = laser_data.start_angle_ + laser_data.resolution_*i;
				angle = Rad2Deg(angle);
				//std::cout<<"obstacle:"<<obstacle<<" angle:"<<angle<<" dis:"<<laser_data.data_[current_index];
				//std::cout<<" min_dis: "<<laser_range.data_[i]<<std::endl;
			}
		}

	}

	if ( obstacle > obstacle_min_ )
	{
		return true;
	}

	return false;
}

bool Obstacle_Detector::check_data(const SDI di)
{
	S32 di_2=0,di_3=0;
	di_2 = di.di_[2];
	di_3 = di.di_[3];
	//std::cout<<"di2 : "<<di_2<<" di3: "<<di_3<<std::endl;
	if(di_2 | di_3){
		return true;
	}
	else{
		return false;
	}
}

void Obstacle_Detector::SetDI(const SDI di)
{
	//std::cout<<"set di"<<std::endl;
	SDi_ = di;
}

NS_Laser_Safe::Obstacle_Status Obstacle_Detector::GetStatus()
{
	return current_state_;
}
void Obstacle_Detector::usephotosensor(bool use)
{
	use_photosensor_ = use;
}

void Obstacle_Detector::outputshape(SLaser shape,F32 laser_dx,F32 laser_dy)
{
	F32 diff_a =  shape.resolution_;
	F32 start_angle  =  shape.start_angle_;

	//std::ofstream out("3.txt");
	if(test_out!=NULL){
//		for(int i = 0;i<LASER_COUNT;i++){
//			float dx = shape.data_[i];
//			float angle = start_angle + diff_a*i;
//			out<<"          "<<dx<<"    "<<angle<<"  \n";
//		}
//		out<<"  "<<"            "<<"   \n\n";
		for(int i = 0;i<LASER_COUNT;i++){
			float range = shape.data_[i];
			float angle = start_angle + diff_a*i;
			float x_= range*cos(angle)+laser_dx;
			float y_= range*sin(angle)+laser_dy;
			//std::cout<<"i: "<<i<<" x: "<<x_<<" y: "<<y_<<" angle:"<<angle<<"  range:"<<range<<std::endl;
			test_out<<x_<<"  "<<y_<<"   \n";
			//test_out<<"  "<<dx<<"    "<<angle<<"   \n";
		}
	}
	//out.close();
	return ;
}

void Obstacle_Detector::init_lasersafe( SLaser &used_laser_data, NS_Laser_Safe::Laser_safe &laser_safe, SLaser_para &laser_para)
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
	//test_out.open("4allshapes.txt");
	std::cout<<"Laser Shape polygon with laser dx:"<<laser_para.laser_dx_<<" dy:"<<laser_para.laser_dy_<<std::endl;

	lasersafe_init_shape(frames_.front_straight_redu1_);
	lasersafe_get_range(laser_safe.laser_range_straight_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_straight_redu2_);
	lasersafe_get_range(laser_safe.laser_range_straight_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_straight_buff_);
	lasersafe_get_range(laser_safe.laser_range_straight_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_straight_stop_);
	lasersafe_get_range(laser_safe.laser_range_straight_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	lasersafe_init_shape(frames_.front_left_redu1_);
	lasersafe_get_range(laser_safe.laser_range_left_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_left_redu2_);
	lasersafe_get_range(laser_safe.laser_range_left_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_left_buff_);
	lasersafe_get_range(laser_safe.laser_range_left_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_left_stop_);
	lasersafe_get_range(laser_safe.laser_range_left_stop_,laser_para.laser_dx_,laser_para.laser_dy_);

	lasersafe_init_shape(frames_.front_right_redu1_);
	lasersafe_get_range(laser_safe.laser_range_right_redu1_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_right_redu2_);
	lasersafe_get_range(laser_safe.laser_range_right_redu2_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_right_buff_);
	lasersafe_get_range(laser_safe.laser_range_right_buff_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.front_right_stop_);
	lasersafe_get_range(laser_safe.laser_range_right_stop_,laser_para.laser_dx_,laser_para.laser_dy_);


	lasersafe_init_shape(frames_.back_straight_redu1_);
	lasersafe_get_range(laser_safe.laser_range_straight_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_straight_redu2_);
	lasersafe_get_range(laser_safe.laser_range_straight_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_straight_buff_);
	lasersafe_get_range(laser_safe.laser_range_straight_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_straight_stop_);
	lasersafe_get_range(laser_safe.laser_range_straight_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	lasersafe_init_shape(frames_.back_left_redu1_);
	lasersafe_get_range(laser_safe.laser_range_left_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_left_redu2_);
	lasersafe_get_range(laser_safe.laser_range_left_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_left_buff_);
	lasersafe_get_range(laser_safe.laser_range_left_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_left_stop_);
	lasersafe_get_range(laser_safe.laser_range_left_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);

	lasersafe_init_shape(frames_.back_right_redu1_);
	lasersafe_get_range(laser_safe.laser_range_right_redu1_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_right_redu2_);
	lasersafe_get_range(laser_safe.laser_range_right_redu2_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_right_buff_);
	lasersafe_get_range(laser_safe.laser_range_right_buff_r_,laser_para.laser_dx_,laser_para.laser_dy_);
	lasersafe_init_shape(frames_.back_right_stop_);
	lasersafe_get_range(laser_safe.laser_range_right_stop_r_,laser_para.laser_dx_,laser_para.laser_dy_);
//	test_out.close();
//	lasersafe_init_shape(shape_string);
//	lasersafe_get_range(used_laser_data,laser_para.laser_dx_,laser_para.laser_dy_);
	std::cout<<"*** Laser safe  calculate  min_range Over!***"<<std::endl;

}


void Obstacle_Detector::lasersafe_init_shape( const std::string &str_shape )
{
std::cout<<"shape string :  "<<str_shape<<std::endl;
	std::vector<Sxy> shape;

	std::vector<std::string> vres;
	cComm::SplitString(str_shape,";",vres);
	std::vector<std::string>::iterator it = vres.begin();
	for ( ; it !=  vres.end() ; ++it)
	{
		std::string str = *it;
		std::vector<std::string> vpos;
		cComm::SplitString(str,":",vpos);

		if (vpos.size() > 1)
		{
			Sxy xy;
			cComm::ConvertToNum(xy.x_,vpos[0]);
			cComm::ConvertToNum(xy.y_,vpos[1]);
			shape.push_back(xy);
		}
	}
	lasersafe_init_shape(shape);
}
void Obstacle_Detector::lasersafe_init_shape( const std::vector<Sxy> &shape )
{
	std::cout<<"begin init polygon para pos size:"<<shape.size()<<std::endl;
	if (shape.size() < 4)
	{
		Sxy xy;
		xy.x_ = 0.5;
		xy.y_ = 0.4;
		shape_list_.push_back(xy);

		xy.x_ = -0.3;
		xy.y_ = 0.4;
		shape_list_.push_back(xy);

		xy.x_ = -0.3;
		xy.y_ = -0.4;
		shape_list_.push_back(xy);

		xy.x_ = 0.5;
		xy.y_ = -0.4;
		shape_list_.push_back(xy);

		xy.x_ = 0.5;
		xy.y_ = 0.4;
		shape_list_.push_back(xy);
	}else
	{
		shape_list_ = shape;
	}

	std::vector<Sxy> shape_new;

	//recalculate the shape
	//1 find robot head in which shape range
	//2 redefine the shape_list,make sure the robot head is the first shape point
	//  find robot head ray line intersection with the shape range
	//  add first shape point to the list
	//  add last shape point to the list

	std::vector<Sxy>::iterator it1 = shape_list_.begin();
	std::vector<Sxy>::iterator it2 = shape_list_.begin();
	it2++;


	//printf shape
	it1 = shape_list_.begin();

	for ( ; it1 != shape_list_.end() ; ++it1 )
	{
		std::cout<<"  x: "<<it1->x_<<"  y: "<<it1->y_<<std::endl;
	}

	std::cout<<"******Sxy  shapelist in baselink !******"<<std::endl;
}

void Obstacle_Detector::lasersafe_get_range(SLaser& min_range,F32 dx,F32 dy){
//	std::cout<<"start angle  ! "<<min_range.start_angle_<<std::endl;
//	std::cout<<"resolution ! "<<min_range.resolution_<<std::endl;
//	std::cout<<"max range ! "<<min_range.range_max_<<std::endl;
//	std::cout<<"min range   ! "<<min_range.range_min_<<std::endl;
//	std::cout<<"laser dx:  "<<dx<<" laser dy: "<<dy<<std::endl;
	//clear all data
	memset(min_range.data_,0,sizeof(F32)*LASER_COUNT);
	//set base laser to base odom
	TF_SERVER.set_base_laser_in_base_link(dx,dy,0);

	//std::ofstream out1("2shapelist_inbase_laser.txt");
	//1 transfer shape list to laser base
	//2 find every ray of laser beems intersection to the shape
	std::vector<Sxy> shape_laser_;
	std::vector<Sxy>::iterator it = shape_list_.begin();
	for ( ; it != shape_list_.end();  ++it)
	{
		VecPosition v(it->x_,it->y_);
		TF_SERVER.tf_base_link_to_base_laser(v);
		Sxy xy;
		xy.x_ = v.getX();
		xy.y_ = v.getY();
		shape_laser_.push_back(xy);
		std::cout<<" x:  "<< v.getX()<<"  y:   "<<v.getY()<<std::endl;
	}
	std::cout<<"******Sxy  shapelist in baselaser !******"<<std::endl;
	//out1.close();
	shape_list_ = shape_laser_;

	F32 start_a = Rad2Deg(min_range.start_angle_);
	F32 diff_a = Rad2Deg(min_range.resolution_);
	F32 angle = 0 ;
	for ( int i = 0 ; i < LASER_COUNT ; ++i )
	{
		angle = start_a + diff_a*i;
		//std::cout<<"angle : "<<angle<<std::endl;
		VecPosition ray(1.0,angle,POLAR);

		Line seg_line;
		Line ray_line = Line::makeLineFromPositionAndAngle(VecPosition(0,0),ray.getDirection());

		if (lasersafe_getCrossLine2(seg_line,ray))
		{
			VecPosition intersection;
			ray_line.getIntersection(seg_line,intersection);
			min_range.data_[i] = intersection.getMagnitude();
			//std::cout<<"i: "<<i<<" range:"<<min_range.data_[i]<<std::endl;
		}else{
			//std::cout<<"i: "<<i<<" can't get crossline"<<std::endl;
		}
		//std::cout<<"i: "<<i<<" range:"<<min_range.data_[i]<<std::endl;
	}

	//fix err
	F32 last_range = 0;
 	for( int i = 0 ; i < LASER_COUNT ; ++i){
 		//std::cout<<"i: "<<i<<" range:"<<min_range.data_[i]<<std::endl;
 		if( fabs(min_range.data_[i]) < 1e-6){
 			min_range.data_[i] = last_range;
 		}
		if ( min_range.data_[i] < min_range.range_min_){

			F32 angle  = angle = start_a + diff_a*i;

			std::cout<<"err filter:"<<angle<<"  range:"<<min_range.data_[i]<<std::endl;

			min_range.data_[i] = min_range.range_min_;
		}
		last_range = min_range.data_[i];
		//std::cout<<"i: "<<i<<" range:"<<min_range.data_[i]<<std::endl;
	}

 	//output shape in base_link
 	//outputshape(min_range,dx,dy);

 	//std::cout<<"********Get range end!***********"<<std::endl;
}

bool Obstacle_Detector::lasersafe_find_ray_in_seg(std::vector<Sxy>::iterator &it_begin,std::vector<Sxy>::iterator &it_end,const VecPosition& ray){

	it_begin = shape_list_.begin();
	it_end = shape_list_.begin();
	it_end++;

	for ( ; it_end != shape_list_.end(); ++it_end)
	{
		VecPosition v1(it_begin->x_,it_begin->y_);
		VecPosition v2(it_end->x_,it_end->y_);

		F32 a1 = VecPosition::IntersectionAngle(v1,ray);
		F32 a2 = VecPosition::IntersectionAngle(v2,ray);
		//std::cout<<"a1 : "<<a1<<"  a2 :  "<<a2<<std::endl;
		if (  (a1 < 0) && (a2 > 0))
		{
//			std::cout<<"a1 : "<<a1<<"  a2 :  "<<a2<<std::endl;
//			std::cout<<"v1  x: "<<v1.getX()<<" y: "<<v1.getY()<<std::endl;
//			std::cout<<"v2  x: "<<v2.getX()<<" y: "<<v2.getY()<<std::endl;
			//if(((180-fabs(a1))<5)||(fabs(a2<5))){
			//	return false;
			//}else{
				return true;
			//}

		}
		it_begin++;
	}
	return false;
}
bool Obstacle_Detector::lasersafe_getCrossLine2( Line &ln,VecPosition ray )
{
	std::vector<Sxy>::iterator seg_begin = shape_list_.end();
	std::vector<Sxy>::iterator seg_end = shape_list_.end();

	if (lasersafe_find_ray_in_seg(seg_begin,seg_end,ray))
	{
		VecPosition vb(seg_begin->x_,seg_begin->y_);
		VecPosition ve(seg_end->x_,seg_end->y_);
		ln = Line::makeLineFromTwoPoints(vb,ve);
		return true;
	}
//    std::cout<<"can't find ray_in seg"<<std::endl;
	return false;
}

void Obstacle_Detector::shapexy_to_vector(const Shape_xy shapexy,std::vector<Sxy> &shape,F32 x_diff,F32 y_diff,bool front/*true:front, false: back*/)
{
	shape.clear();
	Sxy xy;
	if(front == true){
		xy.x_ = shapexy.x1_ + x_diff;
		xy.y_ = shapexy.y1_ - y_diff;
		shape.push_back(xy);
		xy.x_ = shapexy.x2_ + x_diff;
		xy.y_ = shapexy.y2_ + y_diff;
		shape.push_back(xy);
		xy.x_ = shapexy.x3_;
		xy.y_ = shapexy.y3_;
		shape.push_back(xy);
		xy.x_ = shapexy.x4_;
		xy.y_ = shapexy.y4_;
		shape.push_back(xy);
		xy.x_ = shapexy.x1_ + x_diff;
		xy.y_ = shapexy.y1_ - y_diff;
		shape.push_back(xy);

	}else{
		xy.x_ = shapexy.x1_;
		xy.y_ = shapexy.y1_;
		shape.push_back(xy);
		xy.x_ = shapexy.x2_;
		xy.y_ = shapexy.y2_;
		shape.push_back(xy);
		xy.x_ = shapexy.x3_ - x_diff;
		xy.y_ = shapexy.y3_ + y_diff;
		shape.push_back(xy);
		xy.x_ = shapexy.x4_ - x_diff;
		xy.y_ = shapexy.y4_ - y_diff;
		shape.push_back(xy);
		xy.x_ = shapexy.x1_;
		xy.y_ = shapexy.y1_;
		shape.push_back(xy);
	}
}

void Obstacle_Detector::shapexy_to_vector(const Shape_xy shapexy,std::vector<Sxy> &shape,F32 x_diff,F32 y_diff,F32 angle,bool front/*true:front, false: back*/)
{
	shape.clear();
	F32 rotate_angle = Deg2Rad(angle);
	Sxy xy;
	//turn_x=(t_x-o_x)*cos(ang)-(t_y-o_y)*sin(ang)+o_x;
	//turn_y=(t_x-o_x)*sin(ang)+(t_y-o_y)*cos(ang)+o_y;
	if(front == true){
		F32 new_x = shapexy.x1_ + x_diff;
		F32 new_y = shapexy.y1_ - y_diff;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x2_ + x_diff;
		new_y = shapexy.y2_ + y_diff;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x3_;
		new_y = shapexy.y3_;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x4_;
		new_y = shapexy.y4_;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x1_ + x_diff;
		new_y = shapexy.y1_ - y_diff;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

	}else{
		F32 new_x = shapexy.x1_;
		F32 new_y = shapexy.y1_;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x2_ ;
		new_y = shapexy.y2_ ;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x3_ - x_diff;
		new_y = shapexy.y3_ + y_diff;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x4_ - x_diff;
		new_y = shapexy.y4_ - y_diff;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);

		new_x = shapexy.x1_;
		new_y = shapexy.y1_ ;
		xy.x_ = new_x*cos(rotate_angle) - new_y*sin(rotate_angle);
		xy.y_ = new_x*sin(rotate_angle) + new_y*cos(rotate_angle);
		shape.push_back(xy);
	}
}
void Obstacle_Detector::setFrames(Shape_xy robot_shape,F32 r1_x,F32 r2_x,F32 b_x,F32 s_x,F32 r1_y,F32 r2_y,F32 b_y,F32 s_y,F32 rotate_angle)
{//std::cout<<"r1_x: "<<r1_x<<" r2_x:"<<r2_x<<" b_x:"<<b_x<<" s_x:"<<s_x<<"r1_y: "<<r1_y<<" r2_y:"<<r2_y<<" b_y:"<<b_y<<" s_y:"<<s_y<<std::endl;
	std::vector<Sxy>  oneshape;

	//preProcess value
	r1_x = fabs(r1_x);
	r2_x = fabs(r2_x);
	b_x = fabs(b_x);
	s_x = fabs(s_x);
	r1_y = fabs(r1_y);
	r2_y = fabs(r2_y);
	b_y = fabs(b_y);
	s_y = fabs(s_y);
	rotate_angle = abs(rotate_angle);
	//straight frame
	shapexy_to_vector(robot_shape,oneshape,r1_x,r1_y,0.0,true);
	frames_.front_straight_redu1_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,r2_x,r2_y,0.0,true);
	frames_.front_straight_redu2_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,b_x,b_y,0.0,true);
	frames_.front_straight_buff_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,s_x,s_y,0.0,true);
	frames_.front_straight_stop_ = oneshape;
//
	shapexy_to_vector(robot_shape,oneshape,r1_x,r1_y,0.0,false);
	frames_.back_straight_redu1_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,r2_x,r2_y,0.0,false);
	frames_.back_straight_redu2_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,b_x,b_y,0.0,false);
	frames_.back_straight_buff_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,s_x,s_y,0.0,false);
	frames_.back_straight_stop_ = oneshape;
//
	//left rotate frame
	shapexy_to_vector(robot_shape,oneshape,r1_x,r1_y,rotate_angle,true);
	frames_.front_left_redu1_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,r2_x,r2_y,rotate_angle,true);
	frames_.front_left_redu2_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,b_x,b_y,rotate_angle,true);
	frames_.front_left_buff_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,s_x,s_y,rotate_angle,true);
	frames_.front_left_stop_ = oneshape;

	shapexy_to_vector(robot_shape,oneshape,r1_x,r1_y,rotate_angle,false);
	frames_.back_left_redu1_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,r2_x,r2_y,rotate_angle,false);
	frames_.back_left_redu2_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,b_x,b_y,rotate_angle,false);
	frames_.back_left_buff_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,s_x,s_y,rotate_angle,false);
	frames_.back_left_stop_ = oneshape;
//
	//right rotate frame
	shapexy_to_vector(robot_shape,oneshape,r1_x,r1_y,-rotate_angle,true);
	frames_.front_right_redu1_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,r2_x,r2_y,-rotate_angle,true);
	frames_.front_right_redu2_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,b_x,b_y,-rotate_angle,true);
	frames_.front_right_buff_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,s_x,s_y,-rotate_angle,true);
	frames_.front_right_stop_ = oneshape;

	shapexy_to_vector(robot_shape,oneshape,r1_x,r1_y,-rotate_angle,false);
	frames_.back_right_redu1_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,r2_x,r2_y,-rotate_angle,false);
	frames_.back_right_redu2_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,b_x,b_y,-rotate_angle,false);
	frames_.back_right_buff_ = oneshape;
	shapexy_to_vector(robot_shape,oneshape,s_x,s_y,-rotate_angle,false);
	frames_.back_right_stop_ = oneshape;

	return ;
}

