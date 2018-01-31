#ifndef _MAP_PROCESS_BAR_WANGHONGTAO_20150824_
#define _MAP_PROCESS_BAR_WANGHONGTAO_20150824_


#include <boost/serialization/singleton.hpp>

#include "MyDefine.h"

class map_process_bar{

public:
	map_process_bar();
	~map_process_bar();

	void set_total(S32 itotal);
	void set_count(S32 icount);
	void set_count();

private:
	S32 icount_;
	S32 itotal_;
	std::string json_path_ ;
};

typedef boost::serialization::singleton<map_process_bar> Singleton_MAP_PRO_BAR;

#endif//_MAP_PROCESS_BAR_WANGHONGTAO_20150824_
