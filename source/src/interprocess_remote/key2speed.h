#ifndef _WANGHONGTAO_KEY2SPEED_2017_04_05_H_
#define _WANGHONGTAO_KEY2SPEED_2017_04_05_H_

#include "buffer_con.hpp"
#include "remote_ctl.h"

typedef struct _SRange{
	U16 ad1_min_;
	U16 ad1_max_;
	U16 ad2_min_;
	U16 ad2_max_;

	U16 ad1_zero_;
	U16 ad2_zero_;

	S32 ad1_min_range_;
	S32 ad1_max_range_;

	S32 ad2_min_range_;
	S32 ad2_max_range_;

}SRange;

class key2speed{

public:
	key2speed();
	~key2speed();
	
	void set_data(const SRemote_data &data);
	void get_speed(F32 &vx,F32 &vw);
	void setMaxMin(const F32 &vx_max,const F32 &vx_min,const F32 &radius);
	bool init( const SRemote_data &data );
private:
	int sta_zero_;
	int sta_range_;

	//int initialized_;
	filter<U16,10> ad1_filter_;
	filter<U16,10> ad2_filter_;
	

	SRange rangle_;

private:
	F32 ad1_pre_;
	F32 ad2_pre_;
	U8 key1_;
	U8 key2_;
	U8 key3_;
private:
	F32 vx_max_;
	F32 vx_min_;
	//F32 radius_;
	F32 vw_max_;
	F32 radius_max_;
private:
	F32 vx_;
	F32 vw_;
};

#endif//_WANGHONGTAO_KEY2SPEED_2017_04_05_H_
