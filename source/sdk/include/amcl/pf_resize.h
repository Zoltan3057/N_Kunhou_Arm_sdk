#ifndef _PF_RESIZE_WANGHONGTAO_20160103_
#define _PF_RESIZE_WANGHONGTAO_20160103_

#include <boost/thread.hpp>
#include <boost/serialization/singleton.hpp>
#include <fstream>
#include <vector>
#include "buffer_con.hpp"
#include "RobotStruct.h"
#include "amcl/hd_particle.h"

class pf_resize{

public:
	pf_resize();
	~pf_resize();

	void init();
	U32 filter(const U32 &psize, const U32& Wavg);

private:
	U32 pWagv_;
	U32 qWagv_;

	F32 para_p_;
	F32 para_q_;
};

typedef boost::serialization::singleton<pf_resize> Singleton_RESIZE;
//#define Singleton_REPLAY::get_mutable_instance() SINGLETON_REPLAY;

#endif//_PF_RESIZE_WANGHONGTAO_20160103_
