#ifndef _HD_PARTICLE_WANGHONGTAO_20151021_
#define _HD_PARTICLE_WANGHONGTAO_20151021_

#include <boost/serialization/singleton.hpp>
#include <string>
#include <map>

#include "MyDefine.h"
#include "RobotStruct.h"
#include "amcl/hd_reg.h"
#include "linux_driver/mmem.h"


#define MAX_PARTICLE 8192
#define	MIN_PARTICLE 1024

#define HD_TEST 0

typedef struct _SparticleAW
{

	S16 x;
	S16 y;
	S16 th;
    S16 aw;

}SparticleAW;

typedef struct _SparticleAW64
{

	S16 x;
	S16 y;
	S16 th;
    S64 aw;

}SparticleAW64;


typedef struct _SparticleAll
{
	U32 pwsize_;
	U32 pdsize_;
	U32 hd_total_weight_;
	SparticleAW data_w_[8192];
	SparticleAW data_d_[8192];

}SparticleAll;

typedef struct _SHLaser
{
	S16 range;
	S16 angle;

}SHLaser;

typedef struct _SLBRAM
{
	SHLaser hlaser_[FPGA_LASER_BIN_CNT];

}SLBRAM;

typedef struct _SDISCRETE
{
	S32 th1;
	S32 r1;
	S32 th2;
	U32 fh1;
	U32 fr1;
	U32 fh2;

}SDISCRETE;


class hd_particle{

public:

	hd_particle();
	~hd_particle();

	void set_log(bool b);

	void save(std::string file_nm,const U32& psize);
	void save_normal(std::map<int,SparticleAW> p_normalize , F32 x,F32 y,F32 a);

	void init_mmap(const U32& max_psize);
	void init_laser(F32 x,F32 y,U32 lsize);
	void set_laser_size(U32 lsize);
	void get_laser_diff(S16 &sx,S16 &sy);

	bool set_particle_size(const U32 &psize);
	bool set_odom_particle_size(const U32 &psize);

	void set_amcl_pos(const F32& x,const F32& y,const F32& th);
	S32 get_amcl_th();

	void en_trigger_AW();
	void en_trigger_Dis();

	void trigger_AW();
	void trigger_Dis();
	void sel_motion(U32 omni);

	void trigger_LaserTF();
	void clear_AW();
	void clear_Dis();

	void set_total_W(U32 total);
	U32 get_total_W();

	void set_discrete(SDISCRETE discrete);

//	void set_motion_type(U32 motion_type);
//	U32 get_motion_type();

	U32 get_serial_number();

	SparticleAW* p_data_;
	U32 p_data_size_;
	U32 max_psize_;
	U32 total_weight_;

	SLBRAM* laser_data_;

	U32 psize();

private:
	
	U32 psize_;

	//particle
	static const U32 ADD_PATICLE_DATA    	= PARTICLE_ADD;	//4*psize 4*64k
	
	static const U32 ADD_ODOM_BASE		 	= 0x43c00000;
	static const U32 ADD_PARTICLE_BASE	 	= 0x43c10000;	//


	static const U32 ADD_LASER_DATA		 	= 0x40000000;
	

//	static const U32 ADD_LASER_HIT	    	= 0x14000000;
	bool log_file_;


	mmem mp_data_;


	mmem mp_base_;
	U32 p_trigger_;
	mmem mo_base_;
	U32 o_trigger_;

	

	mmem mlaser_;

	boost::mutex mu_log_;
public:
	SOdom_engine* add_43c0_odom_;
	SWeight_engine* add_43c1_weight_;

};

typedef boost::serialization::singleton<hd_particle> Singleton_HD;

#endif//_HD_PARTICLE_WANGHONGTAO_20151021_

