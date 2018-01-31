#ifndef _CONDITION_BUFFER_KYOSHO_20150909_
#define _CONDITION_BUFFER_KYOSHO_20150909_

#include <boost/thread.hpp>
#include <boost/circular_buffer.hpp> 
#include <list>

//a list buffer ,if full or empty will waiting
//usually used for comsumer productor model
template<typename T>
class LBuffer
{
private:
    boost::mutex mu;
    boost::condition_variable_any cond_put;
    boost::condition_variable_any cond_get;

	std::list<T> buffer_list;
public:
	explicit LBuffer(size_t buffer_size) : full_size(buffer_size)
    {};
	
    size_t put(T in)
    {
		size_t icount = 0;
		{
			boost::mutex::scoped_lock lock(mu);

			 while (buffer_list.size() >= full_size)
			 {
     			std::cout << "full waiting..." << std::endl;
				 cond_put.wait(lock); //
			 }
// 			if (buffer_list.full())
// 			{
// 				std::cout << "circular_buffer full:"<<buffer_list.size()<<std::endl;
// 			}
			
			buffer_list.push_back(in);
			icount = buffer_list.size();
		}
		//std::cout<<"buffer size:"<<buffer_list.size()<<std::endl;
        cond_get.notify_one();
		return icount;
    };
    void get(T &out)
    {
		{
			boost::mutex::scoped_lock lock(mu);

			while (buffer_list.empty())
			{
				//std::cout << "empty waiting..." << std::endl;
				cond_get.wait(lock);
			}
			out = buffer_list.front();
			buffer_list.pop_front();

		}
        cond_put.notify_one();
    };
private:
	int full_size ;
};
// a ring buffer put over head , if empty get will waiting 
template<typename T>
class CBuffer
{
private:
	boost::mutex mu;
	boost::condition_variable_any cond_get;

	boost::circular_buffer<T> buffer_list;
	
public:
	explicit CBuffer(size_t buffer_size) : buffer_list(buffer_size)
	{};
	size_t size(){
		boost::mutex::scoped_lock lock(mu);
		return buffer_list.size();
	};
	void put(T in)
	{
		
		{
			boost::mutex::scoped_lock lock(mu);
			if (buffer_list.full())
			{
				//std::cout << "full !! over head ..." << std::endl;
			}
			
			buffer_list.push_back(in);
			
		}
		//std::cout<<"buffer size:"<<buffer_list.size()<<std::endl;
		cond_get.notify_one();

	};
	void get(T &out)
	{
		{
			boost::mutex::scoped_lock lock(mu);

			while (buffer_list.empty())
			{
				//std::cout << "empty waiting..." << std::endl;
				cond_get.wait(lock);
			}
			out = buffer_list.front();
			buffer_list.pop_front();

		}
	
	};
private:
	//int full_size ;
};
// a predefine size(SIZE) ring buffer  : put over head , if empty get will waiting
template<typename T,int SIZE>
class CSBuffer
{
private:
	boost::mutex mu;
	boost::condition_variable_any cond_get;

	boost::circular_buffer<T> buffer_list;

public:
	explicit CSBuffer() : buffer_list(SIZE)
	{};
	size_t size(){
		boost::mutex::scoped_lock lock(mu);
		return buffer_list.size();
	};
	void put(T in)
	{

		{
			boost::mutex::scoped_lock lock(mu);
			if (buffer_list.full())
			{
				//buffer_list.pop_front();
				//std::cout << "full !! over head ..." << std::endl;
			}

			buffer_list.push_back(in);

		}
		//std::cout<<"buffer size:"<<buffer_list.size()<<std::endl;
		cond_get.notify_one();

	};
	void get(T &out)
	{
		{
			boost::mutex::scoped_lock lock(mu);

			while (buffer_list.empty())
			{
				//std::cout << "empty waiting..." << std::endl;
				cond_get.wait(lock);
			}
			out = buffer_list.front();
			buffer_list.pop_front();

		}

	};
	void clear()
	{
		boost::mutex::scoped_lock lock(mu);
		while(!buffer_list.empty()){
			buffer_list.pop_front();
		}
	};

};

// a ring buffer put get no wait
template<typename T>
class SBuffer{

public:
	explicit SBuffer(size_t buffer_size) : buffer_list(buffer_size){};
	
	std::size_t size(){
		boost::mutex::scoped_lock lock(mu);
		return buffer_list.size();
	};

	void clear(){
		boost::mutex::scoped_lock lock(mu);
		buffer_list.clear();
	};
	bool put(T in){
		boost::mutex::scoped_lock lock(mu);
		if (buffer_list.full())
		{
			buffer_list.push_back(in);
			return false;
		}else{
			buffer_list.push_back(in);
			return true;
		}
		
	};
	bool get(T &out){
		boost::mutex::scoped_lock lock(mu);
		if (buffer_list.empty())
		{
			return false;
		}
		out = buffer_list.front();
		buffer_list.pop_front();
		return true;
	};
private:
	boost::mutex mu;

	boost::circular_buffer<T> buffer_list;
};
// no use boost:a ring buffer put over head , if empty get will waiting 
#define BUFFER_SIZE 10
template<typename T>
class OCBuffer{

public:
	OCBuffer():head_(0),rear_(0),deap_(0){

	};
	~OCBuffer(){

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

	void push_back(const T &in){
		{
			boost::mutex::scoped_lock lock(mu);
			if (full())
			{
				std::cout<<"is full egnore:"<<msg_list_[head_]<<std::endl;
				head_ = (++head_) % BUFFER_SIZE;
				deap_--;
			}

			msg_list_[rear_] = in;
			rear_ = (++rear_) % BUFFER_SIZE;
			deap_++;
		}
		cond_get.notify_one();
		assert(deap_ < (BUFFER_SIZE+1));
	};
	void pop_front(T &in){
		boost::mutex::scoped_lock lock(mu);

		while (empty())
		{
			//std::cout << "empty waiting..." << std::endl;
			cond_get.wait(lock);
		}

		in = msg_list_[head_];
		std::cout<<"pop index:"<<head_<<" value:"<<msg_list_[head_]<<std::endl;

		head_ = (++head_) % BUFFER_SIZE;
		deap_--;

	};

private:
	T msg_list_[BUFFER_SIZE];
	int head_;
	int rear_;
	int deap_;
	boost::mutex mu;
	boost::condition_variable_any cond_get;
};

template<typename T,int SIZE>
class filter{


private:

	T g_average[SIZE];
	int average_index;

public:
	filter():average_index(0){
		memset(g_average,0,sizeof(T)*SIZE);
	};
	~filter(){};

	T Average(T data){
		int index;
		double tmp = 0;
		g_average[average_index] = data;

		for (index = 0; index < SIZE; ++index)
		{
			tmp += g_average[index];
		}
		tmp /= SIZE;

		average_index++;
		average_index %= SIZE;

		return tmp;
	};
};

template<typename T>
class THSafe{

public:

	THSafe(){};
	~THSafe(){};

	THSafe(const T& in){
		boost::mutex::scoped_lock lock(mu_);
		t_ = in;
	};

	void operator = ( const T &in ){
		boost::mutex::scoped_lock lock(mu_);
		t_ = in;
	};

	T operator () (void){
		boost::mutex::scoped_lock lock(mu_);
		T out = t_;
		return out;
	};

private:

	T t_;
	boost::mutex mu_;
};
// LBuffer<int> data_list_(10);
// void thread_consumer(){
// 	int i = 0;
// 
// 	while(1){
// 		if (i%5 == 0)
// 		{	
// 			for (int ic = 0; ic < 10 ; ++ic)
// 			{
// 				int si = data_list_.put(i);
// 				std::cout<<"th1 buf size:"<<si<<std::endl;
// 			}
// 
// 		}else{
// 			int si = data_list_.put(i);
// 			std::cout<<"th1 buf size:"<<si<<std::endl;
// 		}
// 		SLEEP(500);
// 		++i;
// 
// 	}
// }
// void thread_producter(){
// 	while(1){
// 		int i = 0;
// 		data_list_.get(i);
// 		std::cout<<"get:"<<i<<std::endl;
// 	}
// }
// int main(int argc, char* argv[]){
// 	boost::thread th1(boost::bind(thread_consumer));
// 	boost::thread th2(boost::bind(thread_producter));
// 
// 	while(1){
// 		for (int ic = 0; ic < 10 ; ++ic)
// 		{
// 			int si = data_list_.put(-99);
// 			std::cout<<"buf size:"<<si<<std::endl;
// 		}
// 		SLEEP(1000);
// 	}
// CBuffer<int> data_list_(10);
// void thread_consumer(){
// 	int i = 0;
// 
// 	while(1){
// 
// 		data_list_.put(i);
// 		std::cout<<"put:"<<i<<std::endl;
// 		SLEEP(100);
// 		++i;
// 
// 	}
// }
// void thread_producter(){
// 	while(1){
// 		int i = 0;
// 		data_list_.get(i);
// 		std::cout<<"     get:"<<i<<std::endl;
// 		SLEEP(200);
// 	}
// }
// int main(int argc, char* argv[]){
// 	boost::thread th1(boost::bind(thread_consumer));
// 	boost::thread th2(boost::bind(thread_producter));
// 
// 	while(1){
// 
// 		SLEEP(1000);
// 	}
// }

#endif//_CONDITION_BUFFER_KYOSHO_20150909_

