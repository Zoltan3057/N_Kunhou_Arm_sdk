#ifndef _PF_REPLAY_WANGHONGTAO_20160103_
#define _PF_REPLAY_WANGHONGTAO_20160103_

#include <boost/thread.hpp>
#include <boost/serialization/singleton.hpp>
#include <fstream>
#include <vector>
#include "buffer_con.hpp"
#include "RobotStruct.h"
#include "amcl/hd_particle.h"

class pf_replay{

public:

	typedef enum{NONE,ENTROPY,FILE,PUBLISH} TY;
	
	pf_replay();
	~pf_replay();

	void init();


	void set_wdata(U32 hd_tatoal_weight);
	void set_ddata();
	void push_back();

	void begin(TY type);
	void stop();

	SPos amcl_pos();

private:
	
	bool b_input_;

	TY ty_;

	bool b_run_;
	boost::thread* th_p_;
	void thread_consumer();

// save file
	void save_file(SparticleAll& pall);
	std::ofstream file_;
// publish
	void publish(SparticleAll& pall);
// call entropy
	SAMCL_RES res_;
	void entropy(SparticleAll& pall,double hz);
	S16 chassis_dia_;
	
	SparticleAll pall_;
	SparticlePub pub_;


	//GridMap* amcl_map_;
	//std::list<SparticleAll> data_;
	CSBuffer<SparticleAll,20> data_;
// call estimate pos
	F32 per_;
	SPos res_pos_;
	boost::mutex res_mu_;
	void estimate_pos_pub(SparticleAll& pall);
	void estimate_pos_pub2(SparticleAll& pall);
	void sort(SparticleAW* pa,U32 psize);
};

typedef boost::serialization::singleton<pf_replay> Singleton_REPLAY;
//#define Singleton_REPLAY::get_mutable_instance() SINGLETON_REPLAY;

#endif//_PF_REPLAY_WANGHONGTAO_20160103_
