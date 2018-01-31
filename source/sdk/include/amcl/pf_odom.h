#ifndef _PF_ODOM_WANGHONGTAO_20151021_
#define _PF_ODOM_WANGHONGTAO_20151021_


#include <string>
#include <boost/thread.hpp>

//#include "Rand.hpp"
#include "RobotStruct.h"


#include "amcl/hd_particle.h"

typedef struct _SDIFF
{
	F32 th1;
	F32 r1;
	F32 th2;

	F32 fh1;
	F32 fr1;
	F32 fh2;

}SDIFF;



class pf_odom{
	
public:
	pf_odom();
	~pf_odom();

	bool init();
	void destructor();
	//---------------------input-------------------
	//call back when interprocess_chassis (20hz)
	void update_odom(const SOdomSpeed &odom);
	//call when resample over > reset new particle size
	void update_psize(const U32 &psize);
	//move motion para
	void set_para(F32 a1,F32 a2,F32 a3,F32 a4);

	void set_para_diff(F32 dr,F32 dth);

	void set_serial_number(const SSerialNum &serial_num);

private:
	//---------------------output-------------------
	void set_diff_DDR();
	void set_odom_psize(const U32 &psize);
	void trigger_discrete();
	
private:


	//cal odom diff with last_odom_;
	bool first_odom_;
	SOdomSpeed last_odom_;
	SPos diff_odom_;
	boost::mutex mu_odom_;
	SOdomSpeed new_odom_;
	bool get_diff();

	SDIFF diff_;
	SDISCRETE discrete_;

//	MyRand rth_;
//	MyRand rtrans_;

	F32 alpha1_;
	F32 alpha2_;
	F32 alpha3_;
	F32 alpha4_;

	F32 dr_;
	F32 dth_;

	F32 fh1_;
	F32 fr_;
	F32 fh2_;

	bool debug_log_;
	std::string model_;

	void cal_fi_diff();
	void cal_fi_diff_omni();
	void cal_zero_diff();

	void transform();

	boost::mutex mu_p_;
	bool particle_ok_ ;
	bool particle_ok(bool diff_ok);

	void log();

	bool check_motion();

private:// thread
	boost::thread* odom_th_;
	bool b_run_;
	void init_thread();
	void end_thread();
	void th_run();

private:

	SSerialNum serial_num_;
};

#endif//_PF_ODOM_WANGHONGTAO_20151021_

