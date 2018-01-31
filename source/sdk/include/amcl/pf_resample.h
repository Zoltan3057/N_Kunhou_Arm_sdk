#ifndef _PF_RESAMPLE_WANGHONGTAO_20151021_
#define _PF_RESAMPLE_WANGHONGTAO_20151021_

#include <string>

#include "RobotStruct.h"
#include "buffer_con.hpp"
#include "linux_driver/laser_tf.h"
#include "amcl/hd_particle.h"





typedef struct _Splaser
{
	U16 data[FPGA_LASER_BIN_CNT];

}Splaser;

class pf_resample{
public:

	pf_resample();
	~pf_resample();

	void init();
	void destructor();
	//---------------------input-------------------
	//call back interprocess_UI
	void update_init_pos(const SInit_Pos &init_pos);
	//call when IRQ calculate Absolutely Weight
	void IRQ_resample_AW();
	//set dynamic lsize
	void set_laser_tf_size(const U32 &lsize);

public:
	//---------------------output-------------------
	void bind_trigger_odom(boost::function<void(const U32 &psize)> fnc);
	void bind_trigger_laser(boost::function<void(const U32 &psize)> fnc);
	void bind_UI_discrete(boost::function<void(const U32 &UI)> fnc);

private:
	boost::function<void(const U32 &psize)> fnc_trigger_odom_;
	boost::function<void(const U32 &psize)> fnc_trigger_laser_;
	boost::function<void(const U32 &UI)> fnc_UI_;

private:
	//---------------------output-------------------
	//void set_particle_DDR();
	void set_particle_psize(const U32 &psize);
	void trigger_laser_odom();


private:
	CSBuffer<U32,1> irq_buffer_;

	//----------resample function list--------------
	std::map<int,SparticleAW> p_normalize_;

	//calculate normal weight
	void calculate_normal_weight();
	//no used now
	void sort();
	//remove zero weight
	U32 calresize(const U32 &no_zero_count);
	//check size
	U32 checksize(U32 new_psize);

	//resample
	void resample(const U32 &new_psize);
	void resample2(const U32 &new_psize);
	//

	//check particle health
	bool check_particle_health();
	//re initialize pos
	void reinitialize(const SInit_Pos &init_pos);

private:// thread

	boost::thread* resample_th_;
	bool b_run_;
	void init_thread();
	void end_thread();
	void th_run();



private:

	bool b_use_max_;
	bool b_resample_;
	bool b_laser_tf_log_;

	S32 i_amcl_x_;
	S32 i_amcl_y_;
	S32 i_amcl_th_;


	SPos res_pos_;
	U32 psize_;
	
	SInit_Pos init_pos_;

	F32 hz_;
	//test
	void sleep();

	laser_tf laser_tf_;
	
	//amcl's A
	
public:
	void createTestData(U32 psize);
	void loadTestData(std::string file_nm);
	void show_particle(std::string file_nm);
	void test_endthread();
	void save_laser();
};

#endif//_PF_RESAMPLE_WANGHONGTAO_20151021_

