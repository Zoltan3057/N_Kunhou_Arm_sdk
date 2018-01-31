#ifndef _RECODER_WANGHONGTAO_20160419_
#define _RECODER_WANGHONGTAO_20160419_

#include <string>
#include <vector>
#include <fstream>

#include <boost/thread.hpp>
#include "buffer_con.hpp"

#include "RobotStruct.h"


class recoder{
	
public:
	recoder();
	~recoder();
	
	void begin_recoder(std::string file_name);
	void end_recoder();

	void put_amcl(const SPos &amcl_pos );
	void set_odom(const SOdomSpeed &speed );
	bool load_one(SOdomSpeed &pos);
	bool load_plan(SOdomSpeed &pos,std::string path_name);
	bool load_path(std::list<SOdomSpeed> &res_list,std::string path_name);
private:
	std::string recoder_file_name_;

	bool first_recoder_;
	SPos last_pos_;

	

	CSBuffer<SOdomSpeed,10> pos_buf_;

	std::fstream fs_;

	void init_thread();
	void end_thread();
	void th_run();


	boost::thread* recoder_th_;
	bool b_run_;

	bool save_one(const SOdomSpeed &pos);
//private:
	//odom
	boost::mutex odom_mu_;
	SOdomSpeed odom_pos_;
};


#endif//_RECODER_WANGHONGTAO_20160419_
