#ifndef _SHARED_PIPE_WANGHONGTAO_2015_09_22_
#define _SHARED_PIPE_WANGHONGTAO_2015_09_22_


#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>


#include "Comm.h"

#include "con_pipe_list.hpp"

template<U32 PIPE_SIZE>
class shared_pipe
{
public:
	explicit shared_pipe(std::string share_name):share_name_(share_name){
		shm_ = 0;
		region_ = 0;
		ptr_ = 0;
		bcreate_ = true;
	};

	~shared_pipe(){

		SDelete(region_);
		SDelete(shm_);
		
	};

	bool init(bool bcreate = false){
		SDelete(ptr_);
		SDelete(region_);
		SDelete(shm_);
		bcreate_ = bcreate;
	
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

				std::cout<<"pipe  size:"<<sizeof(msg_pipe<PIPE_SIZE>)<<std::endl;
				shm_->truncate(sizeof(msg_pipe<PIPE_SIZE>));
				std::cout<<"pipe map and construct share data:"<<share_name_<<std::endl;

				//Map the whole shared memory in this process
				region_ = new boost::interprocess::mapped_region
					(*shm_                       //What to map
					,boost::interprocess::read_write //Map it as read-write
					);

				//Get the address of the mapped region
				void * addr       = region_->get_address();

				//Construct the shared structure in memory
				ptr_ = new (addr) msg_pipe<PIPE_SIZE>();
			}
			else{

				shm_  = new boost::interprocess::shared_memory_object
					(boost::interprocess::open_only               //only create
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
				ptr_ = static_cast<msg_pipe<PIPE_SIZE>*>(addr);
				std::cout<<"get share data ptr:"<<share_name_<<std::endl;
			}



		}
		catch(boost::interprocess::interprocess_exception &ex){

			std::cout<<"create shared data err <shared_data> :" << ex.what() << std::endl;

			SDelete(ptr_);
			SDelete(region_);
			SDelete(shm_);
			return false;
		}
		return true;
	};
	void push_back(std::string name,U8* data,U32 len,bool wait_full = true){
		if (ptr_)
		{
			ptr_->push_back(name,data,len,wait_full);
		}
		
	};
	void pop_front(std::string &name,U8* data,U32 &len){
		if (ptr_)
		{
			ptr_->pop_front(name,data,len);
		}
		
	};
private:

	boost::interprocess::shared_memory_object* shm_;
	boost::interprocess::mapped_region* region_;

	msg_pipe<PIPE_SIZE>* ptr_;


	bool bcreate_;
	std::string share_name_;
};


#endif//_SHARED_PIPE_WANGHONGTAO_2015_09_22_
