
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

#include "remote_ctl.h"
#include "key2speed.h"

#ifdef _WIN32
//#include "keyboard/keypress.h"
#include "UI/cui.h"


//keypress kp;
CUI cui;
#else
#include "keyboard/keyboard.h"
	keyboard kb;
#endif

remote_ctl g_remote;

key2speed g_key2speed;
//#define MAP_MEM_BASE_ADDRESS 0x10000000
//
//#define MAP_SIZE 4096UL
//#define MAP_MASK (MAP_SIZE - 1)


bool brun = true;


//#include "SPort.h"
void shutdown(int sig)
{
	std::cout<<"ctrl c shut down"<<std::endl;
	g_remote.stop();

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
void init_shared_pool(char *argv[]){

	shared_pool::init(argv);
	std::cout<<"amcl init call back"<<std::endl;

	boost::function<void( const sclose &cl)> fnc_shutdown;
	fnc_shutdown = boost::bind(callback,_1);
	shared_pool::Subscribe(shared_pool::name(),"shutdown",fnc_shutdown);

	


}
int main(int argc, char *argv[])
{


	::signal(SIGINT, shutdown);


	//init shared pool : shutdown call back 
	if(argc > 1){
		init_shared_pool(argv);
	}

	cTimerDiff timer;



	SSpeed speed;
	speed.vx_ = 0;
	speed.vy_ = 0;
	speed.vw_ = 0;
	
#ifdef _WIN32
	std::string str_remote_ctl_port = "COM1";
#else
	std::string str_remote_ctl_port = "ttyUSB1";
#endif

	Config::getConfig("remote_ctl_port",str_remote_ctl_port);
	g_remote.init(str_remote_ctl_port);
	std::cout<<"remote_ctl_port:"<<str_remote_ctl_port<<std::endl;

	int continue_set_speed_zero = 20;

	F32 vx = 0;
	F32 vw = 0;

	while(brun){

 		SRemote_data data;


 		if (g_remote.get_data(data))
 		{
			if (g_key2speed.init(data))
			{
				g_key2speed.set_data(data);
				g_key2speed.get_speed(vx,vw);

			}
	
 		}else{
			vx = 0;
			vw = 0;
		}

 		if( (fabs(vx) + fabs(vw)) > 0){
 			continue_set_speed_zero = 20;
 		}

 		if(continue_set_speed_zero){
 			--continue_set_speed_zero;
 			std::cout<<"remote control publish vx:"<<vx<<" vw:"<<vw<<std::endl;
 			speed.vx_ = vx;
 			speed.vy_ = 0;
 			speed.vw_ = vw;
 			shared_pool::Publish(shared_pool::name(),"remote_setspeed",speed);
 		}
 		
		
		//std::cout<<"publich set speed x:"<<speed.vx_<<" w:"<<speed.vw_<<std::endl;

		timer.ms_loop(20);


	}



	return 0;
}
