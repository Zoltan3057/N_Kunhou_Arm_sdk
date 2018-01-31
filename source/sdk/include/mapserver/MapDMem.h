#ifndef _MAP_DATA_MEM_KYOSHO_20150630_
#define _MAP_DATA_MEM_KYOSHO_20150630_


#include <boost/thread/mutex.hpp>


#include "MyDefine.h"
#include "MapDBase.h"

class map_data_mem : map_data_base{
	
public:

	map_data_mem();
	~map_data_mem();
	
	void init(std::string shared_name,U32 dsize,std::string para);
	bool expand(U32 dsize);
	bool set_data_ptr(U8* p_data ,U32 dsize );
	
	void init_lock( std::string shared_name );
	void lock();
	void unlock();

private:

	void setPara(std::string name , std::string value);
	boost::mutex m_lock_;
};










#endif//_MAP_DATA_MEM_KYOSHO_20150630_
