
#include <signal.h>


#include <iostream>
#include <fstream>
//#include <unistd.h>

#include <string>
#include <cassert>
#include "Comm.h"
#include "Geometry.h"
#include "TimerDiff.h"



#include "RobotStruct.h"
#include "interprocess_core/reg_msg.h"
#include "interprocess/shared_data.hpp"
#include "interprocess/shared_pool.hpp"

#include "linux_driver/gpio.h"

bool brun = true; 

gpio g_gpio ;

void shutdown(int sig)
{
	std::cout<<"ctrl c shut down"<<std::endl;
	brun = false;
	shared_pool::destructor();
	SLEEP(500);

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
	boost::function<void( const sclose &cl)> fnc_shutdown;
	fnc_shutdown = boost::bind(callback,_1);
	shared_pool::Subscribe(shared_pool::name(),"shutdown",fnc_shutdown);

}
void test_do_gpio(){

	g_gpio.export_gpio(192,true);
	g_gpio.set_gpio_on(192);
	usleep(3000000);
	g_gpio.set_gpio_off(192);
	usleep(3000000);
	g_gpio.unexport_gpio(192);
	//exit(0);


}
void test_di_gpio(){

	g_gpio.export_gpio(221,true);
	g_gpio.set_gpio_on(221);

	g_gpio.export_gpio(222,false);
	int value = g_gpio.get_gpio(222);
	printf("get di:%d value:%d \n",222,value);
	g_gpio.unexport_gpio(222);


	g_gpio.set_gpio_off(221);
	g_gpio.unexport_gpio(221);
}

#define REBOOT_PIN 222

int main(int argc, char *argv[])
{
	

	::signal(SIGINT, shutdown);

	//init shared pool
	if(argc > 1){
		init_shared_pool(argv);
	}

	g_gpio.export_gpio(221,true);
	g_gpio.set_gpio_on(221);
	g_gpio.unexport_gpio(221);
	//test_do_gpio();

	bool on_off = false;

	while(brun){

		for(int i  = 0 ; i < 8 ; ++i ){

			int dout_num = 200 + i;

			g_gpio.export_gpio(dout_num,true);
			if(on_off){
				g_gpio.set_gpio_on(dout_num);
			}else{
				g_gpio.set_gpio_off(dout_num);
			}

			//usleep(3000000);
			g_gpio.unexport_gpio(dout_num);
		}
		if(on_off){
			on_off = false;
		}else{
			on_off = true;
		}

		std::cout<<"out count!"<<std::endl;
		usleep(3000000);
	}

	cTimerDiff dt;

	g_gpio.export_gpio(REBOOT_PIN,false);

	while(brun){

		//test_di_gpio();
		
		int value = g_gpio.get_gpio(REBOOT_PIN);
		printf("get di:%d value:%d \n",REBOOT_PIN,value);
		if (value)
		{
			STRING cmd = "reboot";
			if (-1 != system(cmd.c_str()))
			{
				brun = false;
			}else{
				std::cout<<"gpio required reboot err!"<<std::endl;
			} 


		}

		dt.ms_loop(100);   //10ms loop , 100hz

	}

	g_gpio.unexport_gpio(REBOOT_PIN);

	return 0;
}
