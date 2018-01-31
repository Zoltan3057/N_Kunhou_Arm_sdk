
#ifndef _CON_PIPE_LIST_WANGHONGTAO_2015_09_22_
#define _CON_PIPE_LIST_WANGHONGTAO_2015_09_22_


#include <iostream>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp> 
#include <boost/interprocess/sync/interprocess_condition.hpp>


//#define PIPE_SIZE 8192

typedef struct _Smsg_pipe
{
	char ch_name_[100];
	U32 offset_;
	U32 len_;

}Smsg_pipe;

template<U32 PIPE_SIZE>
class msg_pipe{

public:

	msg_pipe():head_(0),rear_(0),offset_(0),total_size_(0){
		memset(msg_data_,0,sizeof(U8)*PIPE_SIZE);
		memset(msg_pipe_,0,sizeof(Smsg_pipe)*PIPE_SIZE);
	};
	~msg_pipe(){

	};

	bool empty(){
		if (total_size_ == 0)
		{
			return true;
		}else{
			return false;
		}

	};
	bool full(U32 in){
		if ( (total_size_ + in) <= PIPE_SIZE)
		{
			return false;
		}else{
			return true;
		}

	};
	
	void push_back(std::string name,U8* data,U32 len,bool wait_full){
		if (len > PIPE_SIZE)
		{
			std::cout<<"max pipe size is "<<PIPE_SIZE<<" now:"<<len<<std::endl;
			return;
		}
		
		U32 name_len = name.length();
		
		if ((name_len < 1) || ( name_len > 99) || (!data) || (!len) )
		{
			//std::cout<<"err push name:"<<name<<" data:"<<int(data)<<" len:"<<len<<std::endl;
			return;
		}
		
		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mu);

		while ( full(len))
		{
			if(wait_full){
				std::cout<<"list full begin wait!"<<std::endl;
				cond_put.wait(lock);
			}else{
				//std::cout<<"list full pop_one!"<<std::endl;
				pop_one();
			}
		}

		Smsg_pipe &pipe(msg_pipe_[rear_]);
		name.copy(pipe.ch_name_,name_len,0);
		pipe.offset_ = offset_;
		pipe.len_ = len;

		
		if ( (offset_ + pipe.len_ )>= PIPE_SIZE)
		{
			//seperate two path
			memcpy(msg_data_ + offset_,data,PIPE_SIZE - pipe.offset_);
			memcpy(msg_data_,data + (PIPE_SIZE - pipe.offset_),pipe.len_ - (PIPE_SIZE - pipe.offset_));
			//std::cout<<"seperate push:"<<pipe.offset_<<" len:"<<pipe.len_<<std::endl;
			
		}else{
			memcpy(msg_data_ + offset_,data,len);
			//std::cout<<"      push:"<<pipe.offset_<<" len:"<<pipe.len_<<std::endl;
		}
		
		offset_ += pipe.len_;
		offset_ = offset_ % PIPE_SIZE;

		total_size_ += pipe.len_;
		
		rear_ = (++rear_) % PIPE_SIZE;
		
		cond_get.notify_one();

	};
	void pop_front(std::string &name,U8* data,U32 &len){
		
		if(!data){
			std::cout<<" msg_pipe pop_front:data prt err!"<<std::endl;
			return;
		}
		boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mu);

		while (empty())
		{
			//std::cout << "empty waiting..." << std::endl;
			cond_get.wait(lock);
			//std::cout << "empty waiting over ..." << std::endl;
		}
		memset(data,0,len);

		Smsg_pipe &pipe(msg_pipe_[head_]);
		name = pipe.ch_name_;
		
		if ((pipe.offset_ + pipe.len_) >=PIPE_SIZE)
		{
			len = PIPE_SIZE - pipe.offset_ ;
			memcpy(data,msg_data_ + pipe.offset_,len);
			memcpy(data + len,msg_data_,pipe.len_ - len);
			memset(msg_data_ + pipe.offset_,0xff,len);
			memset(msg_data_,0xff,pipe.len_ - len);
			len = pipe.len_;

			//std::cout<<"seperate pop:"<<pipe.len_<<std::endl;

		}else{
			memcpy(data,msg_data_ + pipe.offset_,pipe.len_);
			memset(msg_data_ + pipe.offset_,0xff,pipe.len_);
			len = pipe.len_;
			//std::cout<<"pop:"<<pipe.offset_<<" len:"<<pipe.len_<<std::endl;
		}	
		memset(&pipe,0,sizeof(Smsg_pipe));
		total_size_ -= len;
		//std::cout<<"total_size_:"<<total_size_<<std::endl;
		head_ = (++head_) % PIPE_SIZE;

		
		cond_put.notify_one();
	};

private:
	
	char msg_data_[PIPE_SIZE];
	Smsg_pipe msg_pipe_[PIPE_SIZE];
	
	U32 head_;
	U32 rear_;
	U32 offset_;
	U32 total_size_;

	boost::interprocess::interprocess_mutex mu;
	boost::interprocess::interprocess_condition cond_get;
	boost::interprocess::interprocess_condition cond_put;

	void pop_one(){


		Smsg_pipe &pipe(msg_pipe_[head_]);

		int len = 0;
		if ((pipe.offset_ + pipe.len_) >=PIPE_SIZE)
		{
			len = PIPE_SIZE - pipe.offset_ ;
//			memcpy(data,msg_data_ + pipe.offset_,len);
//			memcpy(data + len,msg_data_,pipe.len_ - len);
			memset(msg_data_ + pipe.offset_,0xff,len);
			memset(msg_data_,0xff,pipe.len_ - len);
			len = pipe.len_;

			//std::cout<<"seperate pop:"<<pipe.len_<<std::endl;

		}else{
			//memcpy(data,msg_data_ + pipe.offset_,pipe.len_);
			memset(msg_data_ + pipe.offset_,0xff,pipe.len_);
			len = pipe.len_;
			//std::cout<<"pop:"<<pipe.offset_<<" len:"<<pipe.len_<<std::endl;
		}
		memset(&pipe,0,sizeof(Smsg_pipe));
		total_size_ -= len;
		//std::cout<<"total_size_:"<<total_size_<<std::endl;
		head_ = (++head_) % PIPE_SIZE;
	};
};
#endif//_CON_PIPE_LIST_WANGHONGTAO_2015_09_22_
