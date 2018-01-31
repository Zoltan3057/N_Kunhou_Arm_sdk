#ifndef _LASER_TF_WANGHONGTAO_20151021_
#define _LASER_TF_WANGHONGTAO_20151021_

#include <string>
//#include <boost/serialization/singleton.hpp>

#include "RobotStruct.h"
#include "mmem.h"
#include "amcl/hd_reg.h"
#include "linux_driver/laser_driver.h"



class laser_tf
{
public:
	
	laser_tf();
	~laser_tf();

	void init();
	void destructor();

	void get_laser();

	void set_laser_size(const U32& lsize);

protected:
private:
	void save_laser();

	bool b_laser_tf_log_;

	static const U32 ADD_LASER_TF		 	= LASERHIT_ADD;

	mmem m_laser_tf_;
	S16xy* laser_tf_;

	//dynamic laser size
	U32 laser_size_;

	laser_driver laser_driver_;

	//thread
	boost::thread* th_;
	bool b_run_;
	void init_thread();
	void end_thread();
	void th_run();
	
};


#endif//_LASER_TF_WANGHONGTAO_20151021_

