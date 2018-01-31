//#include <boost/circular_buffer.hpp> 

// #include <boost/interprocess/shared_memory_object.hpp>
// #include <boost/interprocess/mapped_region.hpp>

#ifndef _CON_SHARED_LIST_WANGHONGTAO_2015_09_22_
#define _CON_SHARED_LIST_WANGHONGTAO_2015_09_22_


#include <iostream>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp> 
#include <boost/interprocess/sync/interprocess_condition.hpp>


#define BUFFER_SIZE 10

template<typename T>
class msg_queue{

public:
	msg_queue():head_(0),rear_(0),deap_(0){

	};
	~msg_queue(){

	};

	bool empty(){
		if (deap_ == 0)
		{
			return true;
		}else{
			return false;
		}

	};
	bool full(){
		if (deap_ == BUFFER_SIZE)
		{
			return true;
		}else{
			return false;
		}

	};

	bool push_back(const T &in){
		bool bfull = false;
		{
			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mu);

			bfull = full();
			if (bfull)
			{
				//std::cout<<"is full egnore:"<<msg_list_[head_]<<std::endl;
				head_ = (++head_) % BUFFER_SIZE;
				deap_--;
			}

			msg_list_[rear_] = in;
			rear_ = (++rear_) % BUFFER_SIZE;
			deap_++;
		}
		cond_get.notify_one();
		assert(deap_ < (BUFFER_SIZE+1));

		return bfull;
	};
	void pop_front(T &in){
		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mu);

		while (empty())
		{
			//std::cout << "empty waiting..." << std::endl;
			cond_get.wait(lock);
			//std::cout << "empty waiting over ..." << std::endl;
		}

		in = msg_list_[head_];
		//std::cout<<"pop index:"<<head_<<std::endl;

		head_ = (++head_) % BUFFER_SIZE;
		deap_--;

	};

private:
	T msg_list_[BUFFER_SIZE];
	int head_;
	int rear_;
	int deap_;
	boost::interprocess::interprocess_mutex mu;
	boost::interprocess::interprocess_condition cond_get;
};
#endif//_CON_SHARED_LIST_WANGHONGTAO_2015_09_22_