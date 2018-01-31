#ifndef _PF_LASER_WANGHONGTAO_20151204_
#define _PF_LASER_WANGHONGTAO_20151204_

#include <string>

#include "RobotStruct.h"
#include "buffer_con.hpp"


#include "amcl/hd_particle.h"


class pf_laser{
	
public:
	pf_laser();
	~pf_laser();

	void init(U32 lsize);
	void destructor();
	//---------------------input-------------------
	//call back when interprocess_laser (50hz)
	void updata_laser(const SLaser &new_laser);

	//call back when UI set init pos
	void update_init_over(const U32 &w);
	//call when weight_engine do particle discrete over
	void weight_engine_over(const U32 &w);
	//call when discrete IRQ
	void IRQ_discrete_over();

	//

private:
	//---------------------output-------------------
	void set_laser_BRAM();
	void trigger_cal_AW();

private:

	CSBuffer<SLBRAM,1> laser_buffer_;
	CSBuffer<U32,1> do_signal_;

	SLBRAM new_laser_;

	SLBRAM tmp_laser_;

	U32 lsize_ ;

	void wait_new_laser();
	
	bool transform(U32 &lsize,const SLaser &new_laser);

	bool first_theta_;
	void create_angle(const SLaser &new_laser);
	S16 hangle_[LASER_COUNT];
// 	void theta_transform(const SLaser &new_laser);
// 	void range_transform(const SLaser &new_laser);

	//bool transform_used( U32 &lsize , const SLaserAR &used_laser);

	boost::mutex mu_dis_;
	bool discrete_ok_ ;
	bool particle_ok_ ;
	bool particle_ok();
	
private:// thread
	boost::thread* laser_th_;
	bool b_run_;
	void init_thread();
	void end_thread();
	void th_run();
public:
	//test
	//pf_resample* resample_;

	void bind_laser_size(boost::function<void(const U32 &lsize)> fnc);

private:

	boost::function<void(const U32 &lsize)> fnc_laser_size_;

};

#endif//_PF_LASER_WANGHONGTAO_20151204_

