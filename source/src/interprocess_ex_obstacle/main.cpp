#include <signal.h>

#include <iostream>
#include <fstream>

#include "interprocess_core/reg_msg.h"
#include "interprocess/shared_data.hpp"
#include "interprocess/shared_pool.hpp"


#include "Comm.h"
#include "Log.h"
#include "mapserver/MapServer.h"

#include "ultrasonic_ks106.h"


bool b_run = true;

void shutdown(int sig)
{
	std::cout<<"ctrl c shut down"<<std::endl;
	b_run = false;
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
	std::cout<<"chassis init call back"<<std::endl;

	boost::function<void( const sclose &cl)> fnc_shutdown;
	fnc_shutdown = boost::bind(callback,_1);
	shared_pool::Subscribe(shared_pool::name(),"shutdown",fnc_shutdown);

}

int send_count = 5;
bool last_pause = true;

void set_pause_continue(const bool &bflag){

	if ( last_pause != bflag )
	{
		send_count = 5;
		last_pause = bflag;
	}

	if ( send_count > 0)
	{
		send_count--;
	}else
	{
		return;
	}

	U8 data[] = {1};
	U32 len = 1;



	std::string str;

	if ( bflag )
	{
		data[0] = 1;
		str = "pause";

	}else{
		data[0] = 0;
		str = "continue";
	}

	std::cout<<"ultrasonic obstacle set:"<<str<<std::endl;

	shared_pool::Shared_Pipe_Push( PROHIBIT_MAP ,"layer1",data,len);
}
int main(int argc, char *argv[])
{

	::signal(SIGINT, shutdown);

	//init shared pool : shutdown call back
	if(argc > 1){
		init_shared_pool(argv);
	}
	std::string port = "ttyS0";
	Config::getConfig("ultra_com",port);

	ultrasonic_ks106 ks106;
	ks106.init(port);


	while(b_run){

		bool b_obs = ks106.get_data();

		set_pause_continue(b_obs);

		SLEEP(100);

	}
}
