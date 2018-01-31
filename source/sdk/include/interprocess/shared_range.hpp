#ifndef _SHARED_RANGE_WANGHONGTAO_2015_10_27_
#define _SHARED_RANGE_WANGHONGTAO_2015_10_27_

#include <string>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "mu_shared_range.hpp"


class shared_range
{
public:
	explicit shared_range(std::string share_name,U32 dsize):share_name_(share_name),dsize_(dsize){
		shm_ = 0;
		region_ = 0;
// 		shm_data_ = 0;
// 		region_data_ = 0;
		ptr_ = 0;
		ptr_data_ = 0;
		bcreate_ = true;
	};
// 	explicit shared_range(msg_range* ptr):ptr_(ptr),dsize_(ptr_->size()){
// 		shm_ = 0;
// 		region_ = 0;
// 
// 		bcreate_ = true;
// 	};
	~shared_range(){

		SDelete(region_);
		SDelete(shm_);

// 		SDelete(region_data_);
// 		SDelete(shm_data_);
		
	};
	
	bool init(bool bcreate = false){
		SDelete(ptr_);
		SDelete(region_);
		SDelete(shm_);
// 		SDelete(region_data_);
// 		SDelete(shm_data_);
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

				U32 total_size = sizeof(msg_range);
				std::cout<<"msg_shared range size:"<<total_size<<std::endl;
				total_size += dsize_;

				shm_->truncate(total_size);
				std::cout<<"map and construct share data:"<<share_name_<<" size:"<<total_size<<std::endl;

				//Map the whole shared memory in this process
				region_ = new boost::interprocess::mapped_region
					(*shm_                       //What to map
					,boost::interprocess::read_write //Map it as read-write
					);

				//Get the address of the mapped region
				void * addr       = region_->get_address();

				//Construct the shared structure in memory
				ptr_ = new (addr) msg_range();
				ptr_->init(dsize_);
				ptr_data_ = (U8*)addr + sizeof(msg_range);

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
				ptr_ = static_cast<msg_range*>(addr);
				//ptr_->init(dsize_);

				ptr_data_ = (U8*)addr + sizeof(msg_range);

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

	bool read(U8* out,U32 offset,U32 &len){
		if (!ptr_ && !ptr_data_)
		{
			std::cout<<"err check ptr put"<<std::endl;
			return false;
		}

		return ptr_->read(out,offset,len,ptr_data_);

	};
	bool write(U8* in,U32 offset,U32 &len){
		if (!ptr_ && !ptr_data_)
		{
			std::cout<<"err check ptr get"<<std::endl;
			return false;
		}
		return ptr_->write(in,offset,len,ptr_data_);

	};
	msg_range* getPtr(){
		return ptr_;
	};
	U8* getDataPtr(){
		return ptr_data_;
	};
private:

	boost::interprocess::shared_memory_object* shm_;
	boost::interprocess::mapped_region* region_;


// 	boost::interprocess::shared_memory_object* shm_data_;
// 	boost::interprocess::mapped_region* region_data_;

	msg_range* ptr_;
	U8* ptr_data_;
	U32 dsize_;
	
	bool bcreate_;
	std::string share_name_;



};


#endif//_SHARED_RANGE_WANGHONGTAO_2015_10_27_
