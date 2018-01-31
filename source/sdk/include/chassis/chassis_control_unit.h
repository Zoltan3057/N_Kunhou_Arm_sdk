#ifndef _CHASSIS_CONTROL_UNIT_
#define _CHASSIS_CONTROL_UNIT_

#include "MyDefine.h"


class Control_unit
{
public:
	Control_unit();
	~Control_unit();

	//position to base link
	F32 lx;
	F32 ly;

protected:

private:

};



class diff_pos
{
public:
	diff_pos();
	~diff_pos();

	F32 speed_;				//set_speed
	F32 current_speed_;
	S32 pos_;
	S32 last_pos_;
	S32 deta_pos_;


protected:

private:
};


#endif











