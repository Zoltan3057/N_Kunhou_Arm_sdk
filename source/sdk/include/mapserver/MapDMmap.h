#ifndef _MAP_DATA_MMAP_KYOSHO_20150630_
#define _MAP_DATA_MMAP_KYOSHO_20150630_

#include "MyDefine.h"
#include "MapDBase.h"


class map_data_mmap : map_data_base{
	
public:

	map_data_mmap();
	~map_data_mmap();
	
	void init(std::string shared_name,U32 dsize,std::string para);
	bool expand(U32 dsize);
	bool set_data_ptr(U8* p_data ,U32 dsize );

private:
	void init_mmap();
	void des_mmap();
	void setPara(std::string name , std::string value);
	
	U32 add_;
	int memfd_;

};




#endif//_MAP_DATA_MMAP_KYOSHO_20150630_
