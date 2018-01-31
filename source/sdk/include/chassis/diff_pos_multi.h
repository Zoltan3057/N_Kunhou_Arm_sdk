#ifndef _CHASSIS_DIFF_POS_WANGHONGTAO_2017_3_13_
#define _CHASSIS_DIFF_POS_WANGHONGTAO_2017_3_13_

#include "MyDefine.h"
#include "TimerDiff.h"

class diff_pos_multi{


public:
	diff_pos_multi();
	~diff_pos_multi();

	S16 get_diff_pos(const S16 &pos);

private:



	bool First_pos_;

	S16 last_pos_;
	S16 deta_pos_;


};

class diff_pos_multi_32{


public:
	diff_pos_multi_32();
	~diff_pos_multi_32();

	S32 get_diff_pos(const S32 &pos);

private:



	bool First_pos_;

	S32 last_pos_;
	S32 deta_pos_;


};

class diff_rpm_multi{


public:
	diff_rpm_multi();
	~diff_rpm_multi();

	F32 get_revolutions(const S16 &rpm);

private:

	bool First_rpm_;
	cTimerDiff dt_;
};

class driver_unit{

public:

	driver_unit();
	~driver_unit();

	U8 add_id_;
	diff_pos_multi diff_pos_;
	diff_pos_multi_32 diff_pos_32_;
	diff_rpm_multi diff_rpm_;
	

	S8 status_;
	F32 set_rpm_;

	bool set_ok_;
//
	F32 reduction_;

};

#endif//_CHASSIS_DIFF_POS_WANGHONGTAO_2017_3_13_
