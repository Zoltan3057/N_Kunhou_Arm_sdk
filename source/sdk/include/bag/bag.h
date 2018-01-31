#ifndef _BAG_WANGHONGTAO_20151230_
#define _BAG_WANGHONGTAO_20151230_

#include <string>
#include <vector>
#include <fstream>

#include <boost/thread.hpp>
#include "buffer_con.hpp"

#include "RobotStruct.h"


typedef struct _SBAG
{
	SOdomSpeed odom;
	SLaser laser;
	SPos amcl_pos;
}SBAG;



class cbag{
	
public:
	cbag();
	~cbag();
	
	int get_size(std::string file_name);

	bool load_Bag(SBAG &itbag);
	bool load_Bag(std::string file_name,SBAG &itbag);

	void put_odom( const SOdomSpeed &odom,const bool &force_input = false);
	void put_laser(const SLaser &laser);
	void put_amcl(const SPos &amcl_pos );

	void save(bool begin);
	bool save_one(const SBAG &bag);
	
	void get_one(SBAG &bag);

	void begin_save();
	void end_save(std::string file_name);

	void show_pro();
	void show_over();

	void get_original_diff(F32 &ox,F32 &oy,F32 &oa);

private:

	SBAG last_bag_;
	std::vector<SBAG>::iterator it;
	int index;

	CSBuffer<SBAG,10> bag_buf_;

	std::fstream fs_;

	void init_thread();
	void end_thread();
	void th_run();
	


	boost::thread* bag_th_;
	bool b_run_;

	std::string log_file_name_;

	std::string json_path_;
	int itotal_;
	int icount_;

private:
	void set_max_min(const F32 &x,const F32 &y);
	F32 x_max_;
	F32 x_min_;
	F32 y_max_;
	F32 y_min_;
	F32 rotation_;
};


#endif//_BAG_WANGHONGTAO_20151230_
