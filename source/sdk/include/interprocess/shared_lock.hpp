#ifndef _SHARED_LOCK_WANGHONGTAO_20151027_
#define _SHARED_LOCK_WANGHONGTAO_20151027_

#include "string.h"
#include <iostream>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "MyDefine.h"
#include "Comm.h"
#include "mu_lock.hpp"



//#include <boost/thread.hpp>

class shared_lock{

public:
	explicit shared_lock(std::string share_name):share_name_(share_name){
		shm_ = 0;
		region_ = 0;
		mu_lock_ = 0;
	};
	~shared_lock(){
		SDelete(region_);
		SDelete(shm_);
	};

	void lock(){
		if (mu_lock_)
		{
			mu_lock_->lock();
		}
		
		
	};

	void unlock(){
		if (mu_lock_)
		{
			mu_lock_->unlock();
		}

	};
	
	bool init(bool bcreate = false){
		SDelete(mu_lock_);
		SDelete(region_);
		SDelete(shm_);
		// 		SDelete(region_data_);
		// 		SDelete(shm_data_);

		try{
			if(bcreate)
			{
				std::cout<<"remove share data:"<<share_name_<<std::endl;
				boost::interprocess::shared_memory_object::remove(share_name_.c_str());


				//Create a shared memory object.
				shm_  = new boost::interprocess::shared_memory_object
					(boost::interprocess::create_only               //only create
					,share_name_.c_str()           //name
					,boost::interprocess::read_write                //read-write mode
					);
				std::cout<<"Create share data:"<<share_name_<<std::endl;
				//Set size

				U32 total_size = sizeof(mu_lock);
				std::cout<<"msg_shared range size:"<<total_size<<std::endl;

				shm_->truncate(total_size);
				std::cout<<"map and construct share data:"<<share_name_<<std::endl;

				//Map the whole shared memory in this process
				region_ = new boost::interprocess::mapped_region
					(*shm_                       //What to map
					,boost::interprocess::read_write //Map it as read-write
					);

				//Get the address of the mapped region
				void * addr       = region_->get_address();

				//Construct the shared structure in memory
				mu_lock_ = new (addr) mu_lock();

				std::cout<<"get share data ptr:"<<share_name_<<std::endl;
			}
			else{

				shm_  = new boost::interprocess::shared_memory_object
					(boost::interprocess::open_only               //only open
					,share_name_.c_str()           //name
					,boost::interprocess::read_write                //read-write mode
					);
				std::cout<<"get share data:"<<share_name_<<std::endl;
				region_ = new boost::interprocess::mapped_region
					(*shm_                       //What to map
					,boost::interprocess::read_write //Map it as read-write
					);

				//Get the address of the mapped region  
				void * addr       = region_->get_address();  

				//Obtain a pointer to the shared structure  
				mu_lock_ = static_cast<mu_lock*>(addr);
				//ptr_->init(dsize_);

				std::cout<<"get share data ptr:"<<share_name_<<std::endl;
			}



		}
		catch(boost::interprocess::interprocess_exception &ex){

			std::cout<<"create shared data err <shared_data> :"<<share_name_<<" ex:" << ex.what() << std::endl;

			SDelete(mu_lock_);
			SDelete(region_);
			SDelete(shm_);
			return false;
		}
		return true;
	};
private:

	boost::interprocess::shared_memory_object* shm_;
	boost::interprocess::mapped_region* region_;

	std::string share_name_;

	mu_lock* mu_lock_;
};
#endif//_SHARED_LOCK_WANGHONGTAO_20151027_
