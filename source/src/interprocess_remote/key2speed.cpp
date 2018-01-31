#include "math.h"
#include <boost/thread.hpp>
#include "MyDefine.h"
#include "Comm.h"
#include "Acc.h"

#include "key2speed.h"

#define MAX_AD 2048
#define AD1_MIN_RANGE 2000
#define AD1_MAX_RANGE 2000
#define AD2_MIN_RANGE 2000
#define AD2_MAX_RANGE 2000

key2speed::key2speed()
{
	sta_zero_ = 0;
	sta_range_ = 0;

	vx_max_ = 1.0;
	vx_min_ = -0.4;
	//radius_ = 5.0;
	vw_max_ = 1.2;
	radius_max_ = 10;

	rangle_.ad1_min_ = 9999;
	rangle_.ad1_max_ = 0;
	rangle_.ad2_min_ = 9999;
	rangle_.ad2_max_ = 0;

	rangle_.ad1_zero_ = 0;
	rangle_.ad2_zero_ = 0;

	rangle_.ad1_min_range_ = 0;
	rangle_.ad1_max_range_ = 0;
	rangle_.ad2_min_range_ = 0;
	rangle_.ad2_max_range_ = 0;

	vx_ = 0;
	vw_ = 0;

	Config::getConfig("FIX_max_line_speed", vx_max_);
	Config::getConfig("FIX_max_angle_speed", vw_max_);

	std::cout<<"vx_max:"<<vx_max_<<std::endl;
	std::cout<<"vx_min:"<<vx_min_<<std::endl;
	std::cout<<"vw_max:"<<vw_max_<<std::endl;
}

key2speed::~key2speed()
{

}
bool key2speed::init( const SRemote_data &data ){



	if(sta_zero_ < 2){
		if( data.key1_ ){
			rangle_.ad1_zero_ = ad1_filter_.Average(data.new_ad1_);
			rangle_.ad2_zero_ = ad2_filter_.Average(data.new_ad2_);
			sta_zero_ = 1;
		}else{
			if(sta_zero_ == 1){
				sta_zero_ = 2;
			}
			
		}
	}
	if(sta_zero_ < 2){
		return false;
	}

	if(sta_range_ < 2){
		if( data.key2_ ){
			
			if ( data.new_ad1_ < rangle_.ad1_min_ )
			{
				rangle_.ad1_min_ = data.new_ad1_;
			}
			if ( data.new_ad1_ > rangle_.ad1_max_ )
			{
				rangle_.ad1_max_ = data.new_ad1_;
			}

			if ( data.new_ad2_ < rangle_.ad2_min_ )
			{
				rangle_.ad2_min_ = data.new_ad2_;
			}
			if ( data.new_ad2_ > rangle_.ad2_max_ )
			{
				rangle_.ad2_max_ = data.new_ad2_;
			}

			sta_range_ = 1;
		}else{
			if(sta_range_ == 1){

				rangle_.ad1_min_range_ = rangle_.ad1_zero_ - rangle_.ad1_min_;
				rangle_.ad1_max_range_ = rangle_.ad1_max_ - rangle_.ad1_zero_;
				rangle_.ad2_min_range_ = rangle_.ad2_zero_ - rangle_.ad2_min_;
				rangle_.ad2_max_range_ = rangle_.ad2_max_ - rangle_.ad2_zero_;

				sta_range_ = 2;
			}

		}
	}
	if(sta_range_ == 2){

		
		if( abs(rangle_.ad1_min_range_ - AD1_MIN_RANGE ) >200) {
			rangle_.ad1_min_range_ = AD1_MIN_RANGE;
		}
		if( abs(rangle_.ad1_max_range_ - AD1_MIN_RANGE ) >200) {
			rangle_.ad1_max_range_ = AD1_MAX_RANGE;
		}
		if( abs(rangle_.ad2_min_range_ - AD1_MIN_RANGE ) >200) {
			rangle_.ad2_min_range_ = AD2_MIN_RANGE;
		}
		if( abs(rangle_.ad2_max_range_ - AD1_MIN_RANGE ) >200) {
			rangle_.ad2_max_range_ = AD2_MAX_RANGE;
		}

		sta_range_ = 3;
		
	}else if(sta_range_ == 3){
		return true;
	}
	return false;
	
}
void key2speed::set_data( const SRemote_data &data )
{

	

	key1_ = data.key1_;
	key2_ = data.key2_;
	key3_ = data.key3_;


	if ( data.new_ad1_ < rangle_.ad1_zero_)
	{
		ad1_pre_ = F32(rangle_.ad1_zero_ - data.new_ad1_) / rangle_.ad1_min_range_;
	}else{
		ad1_pre_ = F32(rangle_.ad1_zero_ - data.new_ad1_) / rangle_.ad1_max_range_;
	}
	
	if ( data.new_ad2_ < rangle_.ad2_zero_)
	{
		ad2_pre_ = F32(data.new_ad2_ - rangle_.ad2_zero_) / rangle_.ad2_min_range_;
	}else{
		ad2_pre_ = F32(data.new_ad2_ - rangle_.ad2_zero_) / rangle_.ad2_max_range_;
	}
	


 	if (fabs(ad1_pre_) < 0.005)
 	{
 		ad1_pre_ = 0;
 	}
 
 	if (fabs(ad2_pre_) < 0.005)
 	{
 		ad2_pre_ = 0;
 	}
 
 	if (ad1_pre_ > 1.0)
 	{
 		ad1_pre_ = 1.0;
 	}else if(ad1_pre_ < -1.0){
 		ad1_pre_ = -1.0;
 	}
 
 	if (ad2_pre_ > 1.0)
 	{
 		ad2_pre_ = 1;
 	}else if(ad2_pre_ < -1.0){
 		ad2_pre_ = -1;
 	}


	//std::cout<<"key1_:"<<int(key1_)<<" key2_:"<<int(key2_)<<" key3_:"<<int(key3_)<<std::endl;
	//std::cout<<"ad1_pre_:"<<ad1_pre_<<" ad2_pre_:"<<ad2_pre_<<std::endl;
}

void key2speed::get_speed( F32 &vx,F32 &vw )
{
	
	if (key2_)
	{
		F32 v_acc = 0;

		if ( ad2_pre_ > 0)
		{
			v_acc = vx_max_ * ad2_pre_ * 0.005;

			if ( ( vx_ + v_acc ) < vx_max_ )
			{
				vx_ += v_acc;
			}else{
				vx_ = vx_max_;
			}

		}
		if ( ad2_pre_ < 0)
		{
			if( vx_ > 0 ){
				v_acc = vx_min_ * fabs(ad2_pre_) * 0.02;
			}else{
				v_acc = vx_min_ * fabs(ad2_pre_) * 0.01;
			}
			

			if ( ( vx_ + v_acc ) > vx_min_ )
			{
				vx_ += v_acc;
			}else{
				vx_ = vx_min_;
			}

		}

		
		
	}
	
	if (key1_)
	{
		if ( vx_ > 0)
		{
			F32 v_acc = 0;
			v_acc = -vx_max_ * 0.02;

			if ( ( vx_ + v_acc ) > 0 )
			{
				vx_ += v_acc;
			}else{
				vx_ = 0;
			}

		}

		if ( vx_ < 0)
		{
			F32 v_acc = 0;
			v_acc = -vx_min_ * 0.1;

			if ( ( vx_ + v_acc ) < 0 )
			{
				vx_ += v_acc;
			}else{
				vx_ = 0;
			}

		}
	}
	vx = vx_;

	if( fabs( vx ) < 1e-6){

		vw = vw_max_* ad1_pre_;

	}else{

		F32 radius = 0;
		if( fabs( ad1_pre_ ) > 1e-6){
			if (ad1_pre_ > 0)
			{
				radius = radius_max_ * ( 1.01 - ad1_pre_); 
			}else{
				radius = radius_max_ * ( -1.01 - ad1_pre_);
			}
			F32 set_vw = vx / radius;

			if(set_vw > vw_max_){
				set_vw = vw_max_;
			}
			if(set_vw < -vw_max_){
				set_vw = -vw_max_;
			}
			
			vw = Acc::CalAcc(0.02,vw,1.0,2.0,set_vw);
			//vw = vx / radius;
		}else{
			vw = 0;
		}
		
	}
	if (key1_ || key2_)
	{
		vw = 0;
	}
	
	/*if (ad2_pre_ < 0)
	{
		vx = vx_min_ * ad2_pre_;
	}else{
		vx = vx_max_ * ad2_pre_;
	}
	if (ad1_pre_ < 0)
	{
		vw = vw_max_ * ad1_pre_;
	}else{
		vw = vw_max_ * ad1_pre_;
	}*/
//	std::cout<<"ad1_pre_:"<<ad1_pre_<<std::endl;
//
//
//
//	if( fabs(ad1_pre_) < 0.01){
//		vw = 0;
//	}else{
//
//		if (ad1_pre_ < 0){
//			ad1_pre_ = (-1 - ad1_pre_);
//		}else{
//			ad1_pre_ = (1 - ad1_pre_);
//		}
//
//		F32 do_radius = ad1_pre_*radius_;
//		std::cout<<"ad1_pre_222:"<<ad1_pre_<<std::endl;
//		std::cout<<"do_radius:"<<do_radius<<std::endl;
//
//		vw = vx / do_radius;
//
//	}
}

void key2speed::setMaxMin( const F32 &vx_max,const F32 &vx_min,const F32 &vw_max )
{
	vx_max_ = vx_max;
	vx_min_ = vx_min;
	//radius_ = radius;
	vw_max_ = vw_max;
}
