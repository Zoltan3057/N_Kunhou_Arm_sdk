#ifndef _LASER_SAFE_KYOSHO_20161128_
#define _LASER_SAFE_KYOSHO_20161128_

#include "RobotStruct.h"

#include "polygon.h"

class laser_safe
{
public:
	laser_safe();
	~laser_safe();
	
	void set_laser_para( const F32& min_angle,const F32& max_angle,const SLaser& laser);
	void filter_used( SLaser& used_laser);

// 	void set_laser_para(const F32& min_angle,const F32& max_angle,const SLaser_Urg& laser_urg);
// 	void filter_used( SLaser_Urg& used_laser );

protected:
private:
	void set_max_min(const F32& min_angle,const F32& max_angle);
	void set_max_min();



	polygon polygon_;
	
	SLaser min_range_;
//	SLaser_Urg min_range_urg_;

	F32 start_angle_;
	F32 resolution_;

	F32 min_fangle_;
	F32 max_fangle_;

	U32 min_iangle_;
	U32 max_iangle_;

};

#endif//_LASER_SAFE_KYOSHO_20161128_
