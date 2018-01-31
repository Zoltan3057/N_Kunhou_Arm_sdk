#ifndef _MMEM_WANGHONGTAO_20151021_
#define _MMEM_WANGHONGTAO_20151021_

#include "KHTypes.h"

class mmem{


public:
	mmem();
	~mmem();

	bool init_mapping(U32 readl_address,U32 msize);

	bool getVirtualdata(U32 &data);

	bool setData(U32 data);
	bool setData(void* add,U32 len);

	bool getData(U32 &data);
	U32 getData(U8* &data);

	bool getVirtualdata(void* &data, U32 &msize);

private:



	U32 getData(void* data,U32 msize);

	int memfd_;

	void* virtual_address_;

	int msize_;

};


#endif//_MMEM_WANGHONGTAO_20151021_
