#ifndef _HD_MAP_WANGHONGTAO_20150824_
#define _HD_MAP_WANGHONGTAO_20150824_


#include <boost/serialization/singleton.hpp>

#include "MyDefine.h"
#include "amcl/hd_reg.h"

#include "linux_driver/mmem.h"

//#define HD_INFATION

class GridMap;

typedef struct _SMAP_INFLATION
{
	U32 half_inflation_size_;    // <=96
	U32 inflation_trigger_;		 // 0 bit

}SMAP_INFLATION;

class HD_Map
{
public:

	HD_Map();
	~HD_Map();

	bool init_mmap(U32 add);

	void trigger();

	void set_filter(U8* data,U32 len);

	GridMap* get_map();

	static const U32 ADD_INFLATION_0		 	= 0x43C20000;
	static const U32 ADD_INFLATION_1		 	= 0x43C40000;
	static const U32 ADD_INFLATION_2		 	= 0x43C60000;

	static const U32 ADD_INF_COEF_0		 		= 0x40002000;
	static const U32 ADD_INF_COEF_1		 		= 0x40006000;
	static const U32 ADD_INF_COEF_2		 		= 0x40004000;

	static const U32 ADD_INF_MAP_0		 		= 0x40020000;
	static const U32 ADD_INF_MAP_1		 		= 0x40060000;
	static const U32 ADD_INF_MAP_2		 		= 0x40040000;

protected:

private:

	


	mmem m_inf_base_;

	mmem m_inf_coef_;

	SMAP_INFLATION* inflation_;				//

	U8* inflation_coef_;						//

	GridMap* map_;
};

typedef boost::serialization::singleton<HD_Map> Singleton_MAP_HD;

#endif//_HD_MAP_WANGHONGTAO_20150824_
