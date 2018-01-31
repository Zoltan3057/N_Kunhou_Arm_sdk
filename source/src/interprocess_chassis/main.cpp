
#include <signal.h>


#include <iostream>


#include <string>
#include <cassert>

#include "Log4cppArm.hpp"

#include "Comm.h"
#include "Geometry.h"
#include "TimerDiff.h"

#include "RobotStruct.h"
#include "buffer_con.hpp"
#include "interprocess_core/reg_msg.h"
#include "interprocess/shared_data.hpp"
#include "interprocess/shared_pool.hpp"

#include "Acc.h"

#include "chassis/chassis_2wd_diff.h"
#include "chassis/chassis_forklift.h"
#include "chassis/chassis_forklift_ex.h"
#include "chassis/chassis_forklift_diff.h"
#include "chassis/chassis_forklift_diff_kt.h"
#include "chassis/chassis_forklift_yn.h"
#include "chassis/chassis_forklift_ld_analog.h"
#include "chassis/chassis_forklift_diff_huaxiao.h"
#include "chassis/bias_steer_wheel.h"
#include "chassis/chassis_bias_steer.h"
#include "chassis/chassis_4wd_muilt.h"
#include "chassis/chassis_omni_45.h"


#include "chassis/my_driver.h"
#include "chassis/zhongda_driver.h"
#include "chassis/zhaowei_driver.h"
#include "chassis/zhidi_driver.h"
#include "chassis/WinnerRbtDriver.h"
#include "chassis/chuangmeng_driver_multi.h"
#include "chassis/plc_driver.h"
//#include "chassis/huaxiao_driver.h"
#include "chassis/vision_driver.h"
#include "chassis/chuangmeng_driver.h"
#include "chassis/chuangmeng_protocl.h"
#include "chassis/oriental_driver.h"
#include "chassis/ketai_driver.h"
#include "chassis/anjing_driver.h"
#include "chassis/huaxiao_driver2.h"
#include "chassis/huaxiao_driver3.h"
#include "chassis/smj_driver.h"
#include "chassis/moons_CANplus.h"
#include "chassis/moons_driver.h"
#include "chassis/moons_canalyst.h"
#include "chassis/anjing_steering_driver.h"

#include "chassis/t20_half_driver.h"
#include "chassis/anjing_zhongli_driver.h"
#include "chassis/taren_small_driver.h"
#include "chassis/half_driver.h"
#include "chassis/aichong_driver.h"
#include "chassis/CopleyCanTest.h"
#include "chassis/jiateng_driver.h"
#include "chassis/yutou_driver.h"
#include "chassis/yuguan_driver.h"
//#include "Xml_serialization.h"
#include "chassis/anjing_2wd_driver.h"
#include "chassis/omni_45_driver.h"
#include "chassis/songling.h"

#include "sim_driver.h"
#include "copley_driver.h"


#ifndef _WIN32
#include "chassis/can_socket.h"
#include "chassis/t20_can_driver.h"
#endif


bool brun = true;
//std::string chassis_type = "T20analog";
//std::string chassis_type = "chuangmeng60";
//std::string chassis_type = "anjing";
//std::string chassis_type = "huaxiaoforkdiff";
//std::string chassis_type = "anjing_steering_driver";
std::string chassis_type = "T20KH";
Chassis_base* bchassis = 0;
Driver_base* driver = 0;
boost::mutex mu_speed;
SSpeed g_speed;

cTimerDiff acc_dt;
F32 g_v_acc = 0.2;
F32 g_v_dec = 1;
F32 g_w_acc = 0.5;
F32 g_w_dec = 0.5;

void shutdown(int sig)
{
	std::cout<<"ctrl c shut down"<<std::endl;
	brun = false;
	shared_pool::destructor();
	SLEEP(500);

	exit(0);

	return;
}

void callback(const sclose &cl){


	if (cl.over)
	{
		std::cout<<"core shut down:"<<shared_pool::name()<<std::endl;
		shutdown(1);
	}

}
int manual_ctl = 0;
void call_speed(const SSpeed &speed){

	F64 accdt = acc_dt.GetTime();
	accdt /= 1000000;
	acc_dt.Begin();
	//std::cout<<"accdt:"<<accdt<<std::endl;

	boost::mutex::scoped_lock lock(mu_speed);
	if(manual_ctl){
		--manual_ctl;
		std::cout<<"manunal control! no access auto control!"<<std::endl;
		return;
	}
	//std::cout<<"bafore acc!!! set speed call back vx:"<<speed.vx_<<" w:"<<speed.vw_<<std::endl;
//	g_speed.vx_  = Acc::CalAcc(accdt,g_speed.vx_,g_v_acc,g_v_dec,speed.vx_);
//	g_speed.vy_  = Acc::CalAcc(accdt,g_speed.vy_,g_v_acc,g_v_dec,speed.vy_);
//	g_speed.vw_  = Acc::CalAcc(accdt,g_speed.vw_,g_w_acc,g_w_dec,speed.vw_);
	g_speed.vx_  = speed.vx_;
	g_speed.vy_  = speed.vy_;
	g_speed.vw_  = speed.vw_;
	//std::cout<<"after acc!!!! set speed call back vx:"<<g_speed.vx_<<" w:"<<g_speed.vw_<<std::endl;
	//
}
void call_remote_speed(const SSpeed &manunal_speed){
	boost::mutex::scoped_lock lock(mu_speed);
	manual_ctl = 20;
	g_speed.vx_  = manunal_speed.vx_;
	g_speed.vy_  = manunal_speed.vy_;
	g_speed.vw_  = manunal_speed.vw_;
	LOGS_DEBUG("chassis")<<"remote control set speed call back vx:"<<g_speed.vx_<<" w:"<<g_speed.vw_;
}
void call_dio(const SDI &dio){

	//std::cout<<"plc set dio call back!"<<std::endl;
	if (driver)
	{
		driver->set_dio(dio);
	}

}
void call_back_forktask(const SFork_state & fork_state)
{
	std::string fnc_code = cComm::ConvertToString(fork_state.fnc_code_);
	std::string para = cComm::ConvertToString(fork_state.para_);
	std::string current_status = cComm::ConvertToString(fork_state.current_status_);

	if(driver)
	{
		driver->setPara("fnc_code",fnc_code);
		driver->setPara("para",para);
		driver->setPara("status",current_status);
	}
	std::cout<<"fork task, code:"<<fnc_code<<" para:"<<para<<std::endl;
}

void call_back_amcl( const SPos &amcl_pos ){

	driver->setAmclPos(amcl_pos);

}
void init_shared_pool(char *argv[]){

	shared_pool::init(argv);
	std::cout<<"chassis init call back"<<std::endl;

	boost::function<void( const sclose &cl)> fnc_shutdown;
	fnc_shutdown = boost::bind(callback,_1);
	shared_pool::Subscribe(shared_pool::name(),"shutdown",fnc_shutdown);

	boost::function<void( const SSpeed &speed )> fnc_speed;
	fnc_speed = boost::bind(call_speed,_1);
	shared_pool::Subscribe(shared_pool::name(),"setspeed",fnc_speed);

	boost::function<void( const SSpeed &speed )> fnc_remote_speed;
	fnc_remote_speed = boost::bind(call_remote_speed,_1);
	shared_pool::Subscribe(shared_pool::name(),"remote_setspeed",fnc_remote_speed);

	boost::function<void( const SDI &dio )> fnc_io;
	fnc_io = boost::bind(call_dio,_1);
	shared_pool::Subscribe(shared_pool::name(),"gpio_di",fnc_io);

	if(chassis_type == "ketai"){
		boost::function<void(  const SPos &amcl_pos )> fnc_amcl;
		fnc_amcl = boost::bind(&call_back_amcl,_1);
		shared_pool::Subscribe(shared_pool::name(),"amcl_pos",fnc_amcl);
	}

	boost::function<void(const SFork_state & fork_state)> fnc_forktask;
	fnc_forktask  = boost::bind(&call_back_forktask,_1);
	shared_pool::Subscribe(shared_pool::name(),"set_fork_status",fnc_forktask);
	std::cout<<"chassis init call back over "<<std::endl;

}

void inti_chassis(Chassis_base* &base){


	std::string chassis_type = "songling";

	Config::getConfig("chassis_type",chassis_type);

	//copley std::string chassis_para = "dia:0.18;axle:0.560";
	//copley_can std::string chassis_para = "dia:0.220;axle:0.335"
	//kps std::string chassis_para = "dia:0.202;axle:0.385";
	//ketai1 std::string chassis_para = "dia:0.156;axle:0.530";
	//ketai2 std::string chassis_para = "adia:0.152;D:0.620;L:0.0;axle:0.309";

	//std::string chassis_para = "adia:0.23;fdia:0.08;D:2.368;L:0.0;axle:1.04";
	//chuangmeng std::string chassis_para = "adia:0.128;D:0.627;L:0.0;axle:0.339";
	//std::string chassis_para = "dia:0.126;axle:0.339";
	//yn std::string chassis_para = "adia:0.18;D:1.32;L:0.0;Pid_p:3.0";
	//anjing 2wd std::string chassis_para = "adia:0.255;D:1.57;L:0.0";
	//huaxiao2 std::string chassis_para = "adia:0.128;D:0.627;L:0.0;axle:0.339";
	//std::string chassis_para = "dia:0.15;axle:0.26";
	//jd std::string chassis_para = "adia:0.255;D:1.52;L:0.0"
	//std::string chassis_para = "dia:0.130;axle:0.300;D1:0.521;L1:0.0;D2:-0.521;L2:0.0";
	//std::string chassis_para = "dia:0.064;axle:0.165;";
	std::string chassis_para = "dia:0.250;wheel_base:2.0;wheel_axle:0.58";

	Config::getConfig("chassis_para",chassis_para);
	//kps std::string chassis_com = "port:ttyUSB0;counts:60;reduction:4.3";
	//yn std::string chassis_com = "port:ttyUSB0;counts:24;reduction:9900-31.4";
	//jd std::string chassis_com = "port:ttyS3;counts:5900;reduction:1";
	//colpey_can std::string chassis_com = "port:CAN;counts:10000;reduction:20";
	std::string chassis_com = "port:USB_BOX;counts:2000;reduction:16";
	//std::string chassis_com = "port:ttyUSB0;ratio:0.021";
	Config::getConfig("chassis_com",chassis_com);

	std::cout<<"chassis_type:"<<chassis_type<<std::endl;
	std::cout<<"chassis_para:"<<chassis_para<<std::endl;
	std::cout<<"chassis_com"<<chassis_com<<std::endl;

	Config::getConfig("chassis_vacc",g_v_acc);
	Config::getConfig("chassis_vdec",g_v_dec);
	Config::getConfig("chassis_wacc",g_w_acc);
	Config::getConfig("chassis_wdec",g_w_dec);

	std::cout<<"chassis_vacc:"<<g_v_acc<<std::endl;
	std::cout<<"chassis_vdec:"<<g_v_dec<<std::endl;
	std::cout<<"chassis_wacc"<<g_w_acc<<std::endl;
	std::cout<<"chassis_wdec"<<g_w_dec<<std::endl;

	if(chassis_type == "zhidi"){
		base =(Chassis_base*)new Chassis_forklift();
	}else if(chassis_type == "huaxiaoforkdiff"){
		base =(Chassis_base*)new Chassis_forklift_diff_huaxiao();
	}else if(chassis_type == "chuangmeng"){
		base =(Chassis_base*)new Chassis_forklift_diff();
	}else if(chassis_type == "chuangmeng60"){
		base =(Chassis_base*)new Chassis_forklift_yn();
	}else if(chassis_type == "T20analog"){
		base =(Chassis_base*)new Chassis_forklift_ld_analog();
	}else if(chassis_type == "oriental"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "ketai"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "anjing"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "anjing_steering_driver"){
		base =(Chassis_base*)new bias_steer_wheel();
	}else if(chassis_type == "anjing_2wd"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "daji"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "T20Hdriver"){
		base =(Chassis_base*)new Chassis_forklift_ex();
	}else if(chassis_type == "huaxiao3"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "moons_driver"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "moons_can"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "moons_canalyst"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "half_driver"){
		base =(Chassis_base*)new Chassis_forklift_ex();
	}else if(chassis_type == "T20KH"){
		base =(Chassis_base*)new Chassis_forklift_ex();
	}else if(chassis_type == "L16KH"){
		base =(Chassis_base*)new Chassis_bias_steer();
	}else if(chassis_type == "motatech"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "copley_can"){
		base =(Chassis_base*)new Chassis_2wd_diff();
	}else if(chassis_type == "yuguan"){
		base =(Chassis_base*)new Chassis_4wd_muilt();
	}else if(chassis_type == "omni"){
		base =(Chassis_base*)new Chassis_omni_45();
	}else if(chassis_type == "songling"){
		base =(Chassis_base*)new Chassis_omni_45();
//	}else if(chassis_type == "yutou"){
//		base =(Chassis_base*)new Chassis_forklift_ex();
	}else{
		base =(Chassis_base*)new Chassis_2wd_diff();
	}
	

	if ((base) && ( base->init( chassis_para )))
	{
		if(chassis_type == "copley"){
			driver = (Driver_base*)new copley_driver();
		}else if(chassis_type == "kps"){
			driver = (Driver_base*)new My_driver();
		}else if(chassis_type == "zhongda"){
			driver = (Driver_base*)new zhongda_driver();
		}else if(chassis_type == "zhaowei"){
			driver = (Driver_base*)new zhaowei_driver();
		}else if(chassis_type == "zhidi"){
			driver = (Driver_base*)new zhidi_driver();
		}else if(chassis_type == "WinnerRbt"){
			driver = (Driver_base*)new WinnerRbtDriver();
		}else if(chassis_type == "huaxiaoforkdiff"){
			driver = (Driver_base*)new huaxiao_driver2();
		}else if(chassis_type == "huaxiao3"){
			driver = (Driver_base*)new huaxiao_driver3();
		}else if(chassis_type == "vision"){
			driver = (Driver_base*)new vision::vision_driver();
		}else if(chassis_type == "chuangmeng"){
			driver = (Driver_base*)new chuangmeng_driver();
		}else if(chassis_type == "T20analog"){
			driver = (Driver_base*)new plc_driver();
		}else if(chassis_type == "chuangmeng60"){
			driver = (Driver_base*)new chuangmeng_driver_multi();
		}else if(chassis_type == "oriental"){
			driver = (Driver_base*)new oriental_driver();
		}else if(chassis_type == "ketai"){
			driver = (Driver_base*)new ketai::ketai_driver();
		}else if(chassis_type == "anjing"){
			driver = (Driver_base*)new anjing::anjing_driver();
		}else if(chassis_type == "anjing_2wd"){
			driver = (Driver_base*)new anjing_2wd_driver();
		}else if(chassis_type == "anjing_steering_driver"){
			driver = (Driver_base*)new anjing_steering_driver();
		}else if(chassis_type == "daji"){
			driver = (Driver_base*)new smj_driver();
		}else if(chassis_type == "T20Hdriver"){
			driver = (Driver_base*)new t20_half_driver();
		}else if(chassis_type == "moons_can"){
			driver = (Driver_base*)new moons_CANplus();
		}else if(chassis_type == "moons_canalyst"){
			driver = (Driver_base*)new moons_canalyst();
		}else if(chassis_type == "moons_driver"){
			driver = (Driver_base*)new moons_driver();
		}else if(chassis_type == "half_driver"){
			driver = (Driver_base*)new half_driver();
		}else if(chassis_type == "T20KH"){
			driver = (Driver_base*)new t20_can_driver();
		}else if(chassis_type == "L16KH"){
			driver = (Driver_base*)new t20_can_driver();
		}else if(chassis_type == "motatech"){
			driver = (Driver_base*)new aichong_driver();
		}else if(chassis_type == "copley_can"){
			driver = (Driver_base*)new copley_CANplus();
		}else if(chassis_type == "jiateng"){
			driver = (Driver_base*)new jiateng_driver();
		}else if(chassis_type == "yuguan"){
			driver = (Driver_base*)new yuguan_driver();
		}else if(chassis_type == "omni"){
			driver = (Driver_base*)new omni_45_driver();
		}else if(chassis_type == "songling"){
			driver = (Driver_base*)new songling_driver();
		//}else if(chassis_type == "yutou"){
		//	driver = (Driver_base*)new yutou_driver();
		}else{
			driver = (Driver_base*)new sim_driver();
		}
		std::cout<<"init driver type:"<<chassis_type<<std::endl;

#ifdef _WIN32
		//if( (driver) && ( driver->init("port:COM1;counts:30;reduction:30")))
		if( (driver) && ( driver->init("port:COM1;ratio:0.003393")))
#else
		if( (driver) && ( driver->init( chassis_com )))
#endif
		{

			base->initDriver(driver);
			base->run(81);
		}
		else{
			std::cout<<"driver init err "<<chassis_com<<" !!!!!!"<<std::endl;
			SLEEP(2000);
			exit(0);
		}

	}else{
		std::cout<<"device init err!!"<<std::endl;

	}
}
void init_log(){

	log4cpp::Priority::Value priority = log4cpp::Priority::INFO;
	Config::getConfig("chassis_priority",priority);
	std::cout<<"chassis_priority:"<<int(priority)<<std::endl;
	LOG.getLog("chassis").setPriority(log4cpp::Priority::Value(priority));  //LOGS_PRIORITY_INFO("planner_tray");

	//
	LOGS_INFO("chassis")<<"chassis start";
}

int main(int argc, char *argv[])
{


	::signal(SIGINT, shutdown);

	init_log();

	//init shared pool : shutdown call back 
	if(argc > 1){
		init_shared_pool(argv);
	}




	if(argc > 0){

		inti_chassis(bchassis);
		assert(bchassis);

		bchassis->setSpeed(0.0,0.0,0.0);
		SLEEP(100);
	}

	cTimerDiff dt;

	F32 x = 0.0;
	F32 y = 0.0;
	F32 a = 0.0;
	SOdomSpeed odom_speed;


	while(brun){
		dt.Begin();
		
		F32 ox,oy,oa;
		F32 ovx,ovy,ow;


		{
			boost::mutex::scoped_lock lock(mu_speed);
			x = g_speed.vx_;
			y = g_speed.vy_;
			a = g_speed.vw_;
 			//x = 0.0;
			//std::cin>>x;
 			//a = 0.1;
		}

		bchassis->setSpeed(x,y,a);

		bchassis->getOdom(ox,oy,oa);

		bchassis->getSpeed(ovx,ovy,ow);

		odom_speed.x_ = ox;
		odom_speed.y_ = oy;
		odom_speed.th_ = oa;
		odom_speed.vx_ = ovx;
		odom_speed.vy_ = ovy;
		odom_speed.vw_ = ow;

		if(argc > 1){
			shared_pool::Publish(shared_pool::name(),"odom",odom_speed);
			//std::cout<<"x="<<ox<<"  y="<<oy<<"  w="<<oa<<std::endl;
		}
		dt.ms_loop(20);
	    dt.End();
		std::stringstream ss;
		//ss<<" cx:"<<ovx<<" cw:"<<ow<<std::endl;
		//ss<<"chassis do time dt:"<<(dt.GetTime() / 1e6)<<" sx:"<<x<<" sa:"<<a<<" cx:"<<ovx<<" cw:"<<ow<<" odox:"<<ox<<" odoy:"<<oy<<" odoa:"<<oa;
		ss<<" sx:"<<x<<" sa:"<<a<<" odox:"<<ox<<" odoy:"<<oy<<" odoa:"<<oa;
		//std::cout<<ss.str()<<std::endl;

		LOGS_DEBUG("chassis")<<ss.str();

	}

	if(bchassis){

		bchassis->stop();

		SDelete(bchassis);
	}

	return 0;
}
