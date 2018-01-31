
#include <signal.h>


#include <iostream>


#include <string>
#include <cassert>


#include "Comm.h"

#include "TimerDiff.h"

#include "RobotStruct.h"

#include "interprocess_core/reg_msg.h"
#include "interprocess/shared_data.hpp"
#include "interprocess/shared_pool.hpp"
#include "Acc.h"

#ifdef _WIN32
#include "keyboard/keypress.h"
#include "UI/cui.h"

keypress kp;
CUI cui;
#else
#include "keyboard/keyboard.h"
#include "UI/cui.h"
	keyboard kb;
	CUI cui;
#endif

//#define MAP_MEM_BASE_ADDRESS 0x10000000
//
//#define MAP_SIZE 4096UL
//#define MAP_MASK (MAP_SIZE - 1)


bool brun = true;


//#include "SPort.h"
void shutdown(int sig)
{
	std::cout<<"ctrl c shut down"<<std::endl;
	brun = false;
	shared_pool::destructor();
	SLEEP(500);
#ifdef _WIN32
#else
	kb.close();
#endif

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

SSpeed g_speed;
boost::mutex mu;
void call_back_ui( const Srecoder& str ){

	boost::mutex::scoped_lock lock(mu);

	std::string str_key = str.cmd_file_;

	char keycode = 0;

#ifndef _WIN32
	if( "up" == str_key){
		keycode = KEYCODE_W;
	}else if( "down" == str_key){
		keycode = KEYCODE_S;
	}else if( "left" == str_key){
		keycode = KEYCODE_A;
	}else if( "right" == str_key){
		keycode = KEYCODE_D;
	}
#endif

	if (cui.set_keycode(keycode,g_speed.vx_,g_speed.vw_))
	{
		shared_pool::Publish(shared_pool::name(),"setspeed",g_speed);
		std::cout<<"publich set speed x:"<<g_speed.vx_<<" w:"<<g_speed.vw_<<std::endl;
	}
}
void call_back_plc( const SSpeed& speed ){

	boost::mutex::scoped_lock lock(mu);

	g_speed.vx_  = Acc::CalAcc(0.5,g_speed.vx_,0.02,0.1,speed.vx_);
	if(fabs(g_speed.vx_) < 0.1){
		g_speed.vx_ = 0;
	}
	g_speed.vw_ = Acc::CalAcc(0.5,g_speed.vw_,0.02,0.1,speed.vw_);
	if(fabs(g_speed.vw_) < 0.1){
		g_speed.vw_ = 0;
	}

	shared_pool::Publish(shared_pool::name(),"setspeed",g_speed);
}

void init_shared_pool(char *argv[]){

	shared_pool::init(argv);
	std::cout<<"amcl init call back"<<std::endl;

	boost::function<void( const sclose &cl)> fnc_shutdown;
	fnc_shutdown = boost::bind(callback,_1);
	shared_pool::Subscribe(shared_pool::name(),"shutdown",fnc_shutdown);


	//ui
	boost::function<void( const Srecoder& rec)> fnc_UI;
	fnc_UI = boost::bind(&call_back_ui,_1);
	shared_pool::Subscribe(shared_pool::name(),"set_control",fnc_UI);

	//plc
	boost::function<void( const SSpeed& speed)> fnc_PLC;
	fnc_PLC = boost::bind(&call_back_plc,_1);
	shared_pool::Subscribe(shared_pool::name(),"set_plc_control",fnc_PLC);

}


int main(int argc, char *argv[])
{


	::signal(SIGINT, shutdown);

	std::string model = "remote";

	//init shared pool : shutdown call back 
	if(argc > 1){
		init_shared_pool(argv);

		model = argv[1];
	}

	cui.init(0.2,1.2,0.1,0.1);


	cTimerDiff timer;

	g_speed.vx_ = 0;
	g_speed.vy_ = 0;
	g_speed.vw_ = 0;
#ifdef _WIN32

	kp.init();


#endif




	while(brun){

		
		char keycode = 0;
		
		
#ifdef _WIN32
		if (kp.getkey(keycode))
		{

			std::cout<<keycode<<std::endl;
			//do control user interface
			if (cui.set_keycode(keycode,g_speed.vx_,g_speed.vw_))
			{
				shared_pool::Publish(shared_pool::name(),"setspeed",g_speed);
				std::cout<<"publich set speed x:"<<g_speed.vx_<<" w:"<<g_speed.vw_<<std::endl;
			}
			

		}
		timer.ms_loop(20);

#else
		if(model == "shared"){
			bool keyboard_stop = false;
			if (!kb.keyboardLoop(0.02,g_speed.vx_,g_speed.vy_,g_speed.vw_,keyboard_stop))
			{
				shared_pool::Publish(shared_pool::name(),"setspeed",g_speed);
				std::cout<<"publich set speed x:"<<g_speed.vx_<<" y:"<<g_speed.vy_<<" w:"<<g_speed.vw_<<std::endl;
			}
			timer.ms_loop(20);
		}else{
//			std::cout<<"keycontrol remote!"<<std::endl;
			timer.ms_loop(1000);
		}
#endif

	}



	return 0;
}
