#ifndef _SHARED_DATA_WANGHONGTAO_2015_09_22_
#define _SHARED_DATA_WANGHONGTAO_2015_09_22_


#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>


#include "Comm.h"
#include "con_shared_list.hpp"



template<typename T>
class shared_data
{
public:
	explicit shared_data(std::string share_name):share_name_(share_name){
		shm_ = 0;
		region_ = 0;
		ptr_ = 0;
		bcreate_ = true;
	};
// 	explicit shared_data(msg_queue<T>* ptr):ptr_(ptr){
// 		shm_ = 0;
// 		region_ = 0;
// 
// 		bcreate_ = true;
// 	};
	~shared_data(){

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

				std::cout<<"s size:"<<sizeof(msg_queue<T>)<<std::endl;
				shm_->truncate(sizeof(msg_queue<T>));
				std::cout<<"map and construct share data:"<<share_name_<<std::endl;

				//Map the whole shared memory in this process
				region_ = new boost::interprocess::mapped_region
					(*shm_                       //What to map
					,boost::interprocess::read_write //Map it as read-write
					);

				//Get the address of the mapped region
				void * addr       = region_->get_address();

				//Construct the shared structure in memory
				ptr_ = new (addr) msg_queue<T>();
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
				ptr_ = static_cast<msg_queue<T>*>(addr);
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

	bool put(const T &in){
		if (!ptr_)
		{
			std::cout<<"err check ptr put"<<std::endl;
			return false;
		}

		return ptr_->push_back(in);

	};
	bool get(T &out){
		if (!ptr_)
		{
			std::cout<<"err check ptr get"<<std::endl;
			return false;
		}
		ptr_->pop_front(out);
		return true;
	};
	msg_queue<T>* getPtr(){
		return ptr_;
	};
private:

	boost::interprocess::shared_memory_object* shm_;
	boost::interprocess::mapped_region* region_;

	msg_queue<T>* ptr_;
	
	bool bcreate_;
	std::string share_name_;
};


#endif//_SHARED_DATA_WANGHONGTAO_2015_09_22_
