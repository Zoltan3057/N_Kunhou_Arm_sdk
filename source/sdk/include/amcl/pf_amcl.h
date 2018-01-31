#ifndef _PF_AMCL_WANGHONGTAO_20151205_
#define _PF_AMCL_WANGHONGTAO_20151205_

#include <string>

#include "RobotStruct.h"
#include "buffer_con.hpp"

#include "amcl/pf_odom.h"
#include "amcl/pf_laser.h"
#include "amcl/pf_amcl.h"
#include "amcl/pf_resample.h"

//#include "amcl/hd_particle.h"
#include "linux_driver/pf_driver.h"

class pf_amcl{

public:

	pf_amcl();
	~pf_amcl();
	
	void init();

	void destructor();
	//---------------------test-------------------
	void test_update_psize(const U32& psize);
	void test_weight_engine_over(const U32 &w);
	//void test_Discrete();
	void test_update_init_over(const U32 &w);
	void test_IRQ_resample_AW();
	void test_irq_w();
	void test_irq_o();
	void test_UI(const SPos& pos ,const U32& psize);

	void test_update_laser(const SLaser &new_laser);
	void test_update_odom(const SOdomSpeed &odom);

	void test_resample(std::string file_nm);
	void test_discrete(SPos pos,U32 sl,U32 count);

	void test_endthread();

private:

	//init call back , when interprocess call function
	void init_call_back();
	//bind function
	void bind_function();
	//init hd_driver_irq_thread();
	void init_IRQ_thread();


private:
	void load_para();
	F32 para1_;
	F32 para2_;
	F32 para3_;
	F32 para4_;

	F32 dr_;
	F32 dth_;

	pf_odom odom_;
	pf_laser laser_;
	pf_resample resample_;

	boost::thread* p_th_;
	boost::thread* o_th_;
	pf_driver driver_;

	bool th_run_;
	void p_irq_run();
	void o_irq_run();

	void show_particle();
};

#endif//_PF_AMCL_WANGHONGTAO_20151205_

