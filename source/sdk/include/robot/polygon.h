#ifndef _POLYGON_KYOSHO_20160316_
#define _POLYGON_KYOSHO_20160316_

#include "RobotStruct.h"

#define POLYGON_COUNT		1081
#define POLYGON_ANGLE_START -135
#define POLYGON_ANGLE_DIFF  0.25

class polygon
{
public:
	polygon();
	~polygon();
	
	void init(F32 fangle = 45 ,F32 fdis = 0.45);
	void init_width_dis(F32 fwidth = 0.3 ,F32 fdis = 2.6);
	void get_sick_range(SLaser& min_range,F32 dx,F32 dy);
//	void get_urg_range(SLaser_Urg& min_range,F32 dx);

protected:
private:
	void clean();
	
// 	F32 angle_;
// 	F32 dis_;
	
	F32 angle_list_[POLYGON_COUNT];
	F32 range_list_[POLYGON_COUNT];


};


#endif//_LASER_FILTER_KYOSHO_20160316_
