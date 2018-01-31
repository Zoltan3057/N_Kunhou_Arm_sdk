#ifndef _REF_FEATURE_LOCALIZATION_BASE_WANGHONGTAO_2017_02_07_
#define _REF_FEATURE_LOCALIZATION_BASE_WANGHONGTAO_2017_02_07_


#include "MyDefine.h"
#include <boost/thread.hpp>

#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"



class ref_feature_loc_base
{
public:

	ref_feature_loc_base();
	~ref_feature_loc_base();

	bool init( std::string para );



protected:

	void init();

	virtual void setPara(std::string name , std::string value) = 0;

private:
	
	//
	
	
	

};

#endif//_REF_FEATURE_LOCALIZATION_BASE_WANGHONGTAO_2017_02_07_
