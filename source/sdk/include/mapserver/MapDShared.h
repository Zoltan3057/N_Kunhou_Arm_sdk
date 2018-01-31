#ifndef _MAP_DATA_SHARED_KYOSHO_20150630_
#define _MAP_DATA_SHARED_KYOSHO_20150630_


#include "interprocess/shared_range.hpp"

#include "MyDefine.h"
#include "MapDBase.h"


class map_data_shared : map_data_base{
	
public:

	map_data_shared();
	~map_data_shared();
	
	void init(std::string shared_name,U32 dsize,std::string para);
	bool expand(U32 dsize);
	bool set_data_ptr(U8* p_data ,U32 dsize );

private:

	void setPara(std::string name , std::string value);
	
	shared_range* sr_;

};




#endif//_MAP_DATA_SHARED_KYOSHO_20150630_
