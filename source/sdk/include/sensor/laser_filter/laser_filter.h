#ifndef _LASER_FILTER_KYOSHO_20160316_
#define _LASER_FILTER_KYOSHO_20160316_

#include "RobotStruct.h"

#include "polygon_ex.h"

class laser_filter
{
public:
	laser_filter();
	~laser_filter();
	
	void init_laser_para(const SLaser &laser,const F32 &laser_dx,const F32 &laser_dy);
//	void set_laser_para( const F32& min_angle,const F32& max_angle,const SLaser& laser);
	void filter_used( SLaser& used_laser);

// 	void set_laser_para(const F32& min_angle,const F32& max_angle,const SLaser_Urg& laser_urg);
// 	void filter_used( SLaser_Urg& used_laser );
	void get_filter(SLaser& filter_laser);

protected:
private:
// 	void set_max_min(const F32& min_angle,const F32& max_angle);
// 	void set_max_min();

	polygon_ex polygon_ex_;
	
	SLaser min_range_;
//	SLaser_Urg min_range_urg_;

// 	F32 start_angle_;
// 	F32 resolution_;

// 	F32 min_fangle_;
// 	F32 max_fangle_;
// 
// 	U32 min_iangle_;
// 	U32 max_iangle_;

};

#endif//_LASER_FILTER_KYOSHO_20160316_
