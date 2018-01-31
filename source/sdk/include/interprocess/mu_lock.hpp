#ifndef _MU_LOCK_WANGHONGTAO_20151027_
#define _MU_LOCK_WANGHONGTAO_20151027_

#include "string.h"
#include <iostream>

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>



//#include <boost/thread.hpp>

class mu_lock{

public:
	mu_lock(){

	};
	~mu_lock(){
		
	};

	void lock(){
		mu.lock();
	};

	void unlock(){
		mu.unlock();
	};

private:

	boost::interprocess::interprocess_mutex mu;
	//boost::mutex mu;
};
#endif//_MU_LOCK_WANGHONGTAO_20151027_
