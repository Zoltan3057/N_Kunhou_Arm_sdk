#ifndef _MU_SHARED_RANGE_WANGHONGTAO_20151027_
#define _MU_SHARED_RANGE_WANGHONGTAO_20151027_

#include "string.h"
#include <iostream>

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "MyDefine.h"
#include "Comm.h"


//#include <boost/thread.hpp>

class msg_range{

public:
	explicit msg_range():dsize_(0){

	};
	~msg_range(){
		
	};
	void init(U32 dsize){

		dsize_ = dsize;
		
	};
	U32 size(){
		return dsize_;
	};
	bool read(U8* out,U32 offset,U32 &len,U8* data_){
		if (!data_)
		{
			return false;
		}
		
		memset(out,0,len);
		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mu);
		//boost::mutex::scoped_lock lock(mu);
		if (offset < dsize_ )
		{
			if((offset + len) > dsize_){
				len = dsize_ - offset;
			}
			memcpy(out,data_ + offset,len);
			return true;
		}
		return false;
	};

	bool write(U8* in,U32 offset,U32 &len,U8* data_){
		if (!data_)
		{
			return false;
		}
		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mu);
		//boost::mutex::scoped_lock lock(mu);
		if (offset < dsize_ )
		{
			if((offset + len) > dsize_){
				len = dsize_ - offset;
			}
			memcpy(data_+ offset,in,len);
			SLEEP(2000);
			return true;
		}
		return false;
	};


private:

	U32 dsize_;

	boost::interprocess::interprocess_mutex mu;
	//boost::mutex mu;
};
#endif//_MU_SHARED_RANGE_WANGHONGTAO_20151027_
