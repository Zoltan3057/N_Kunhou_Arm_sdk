#ifndef _CLOSET_FILTER_WANGHONGTAO_20161230_
#define _CLOSET_FILTER_WANGHONGTAO_20161230_

#include <string>
#include <vector>
#include <list>
#include <fstream>

#include <boost/thread.hpp>


#include "RobotStruct.h"


class cloest_filter{
	
public:
	cloest_filter();
	~cloest_filter();

	bool make_cloest(std::list<SPos> &path_list);
	

private:
	
	int begin_index_;
	int end_index_;

	bool b_run_;

	bool check_available(F32 x,F32 y);
	F32 get_p1_p2(VecPosition &p1,VecPosition &p2,VecPosition &diff,const std::vector<SPos> &path_list);
	bool get_cloest_new(std::list<SPos> &new_list,const VecPosition &p1,const VecPosition &p2, VecPosition &diff);

	std::list<SPos> res_list_;

};


#endif//_CLOSET_FILTER_WANGHONGTAO_20161230_
