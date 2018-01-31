#ifndef _MAP_DATA_BASE_KYOSHO_20150630_
#define _MAP_DATA_BASE_KYOSHO_20150630_

#include "MyDefine.h"
#include "Comm.h"

class shared_lock;

class map_data_base{
	
public:

	typedef enum{MEM,SHARED,MMAP} M_TYPE;

	
	map_data_base();
	virtual ~map_data_base();
	
	virtual void init(std::string shared_name,U32 dsize,std::string para) = 0;
	virtual bool expand(U32 dsize)=0;
	virtual bool set_data_ptr(void* p_data ,U32 dsize );

	void get_data_ptr( U8* &p_data,U32 &dsize );
	void get_data(U8* out,U32 &dsize);
	void clear(S8 value = 0);
	
	U32 get_dsize();

	M_TYPE get_type();
	 
	virtual void init_lock(std::string shared_name);
	virtual void lock();
	virtual void unlock();

protected:
	
	U8* data_;
	U32 dsize_;
	M_TYPE type_;
	std::string shared_name_;
	std::string lock_name_;
	shared_lock* share_lock_;

	void init_para( std::string para );

	virtual void setPara(std::string name , std::string value) = 0;
};


#endif//_MAP_DATA_BASE_KYOSHO_20150630_
