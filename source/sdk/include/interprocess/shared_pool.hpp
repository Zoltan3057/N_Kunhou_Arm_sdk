#ifndef _SHARED_POOL_WANGHONGTAO_2015_09_24_
#define _SHARED_POOL_WANGHONGTAO_2015_09_24_

#include <fstream>
#include <map>
#include <string>
//#include <boost/interprocess/shared_memory_object.hpp>
//#include <boost/interprocess/mapped_region.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "shared_data.hpp"
#include "shared_range.hpp"
#include "shared_lock.hpp"
#include "shared_pipe.hpp"



#define PIPE_SIZE 40960

class msg_pair
{
public:
	msg_pair(){};
	~msg_pair(){};

	std::string th1;
	std::string th2;
	std::string msg;
	std::string golobal_name;
	void* ptr_;

};
class sh_pair{
public:
	sh_pair(){};
	~sh_pair(){};

	std::string sh_name;
	U32 dsize_;
	shared_range* ptr_;
};

class lock_pair{
public:
	lock_pair(){};
	~lock_pair(){};

	std::string sh_name;
	shared_lock* ptr_;
};

class pipe_pair{
public:
	pipe_pair(){};
	~pipe_pair(){};

	std::string th1;
	std::string th2;
	std::string msg;
	std::string golobal_name;

	shared_pipe<PIPE_SIZE>* ptr_;
};


class shared_pool
{
public:
	explicit shared_pool(){

	};
	~shared_pool(){
		
	};

	static std::string name(){
		return nm_;
	};
	static void init(char *argv[]){
		nm_ = argv[0];
		nm_ = cComm::Get_FileName(nm_);

		std::cout<<"process init:"<<nm_<<std::endl;
		if (nm_ != "interprocess_core")
		{
			load_msg("reg_msg.txt");
			load_sh("reg_sh.txt");
			load_lock("reg_lock.txt");
			load_pipe("reg_pipe.txt");
			def_shutdown();
		}
		
	};
	static bool Check_exist(std::string sh_name){
		boost::interprocess::shared_memory_object* shm = 0;
		try{
			shm  = new boost::interprocess::shared_memory_object
			(boost::interprocess::open_only               //only open
			,sh_name.c_str()           //name
			,boost::interprocess::read_write                //read-write mode
			);
			if (shm)
			{
				return true;
			}
			
		}
		catch(boost::interprocess::interprocess_exception &ex){

			std::cout<<"create shared data err <shared_data> :" << ex.what() << std::endl;
			SDelete(shm);
			return false;
		}
	};
	//register a shared pipe
	static bool Reg_sh_pipe(std::string th_name1,std::string th_name2,std::string msg_name){
		std::string golobal_name;
		if (insert_pipe_pair(th_name1 , th_name2 , msg_name , golobal_name))
		{
			if (check_redefine(golobal_name))
			{
				return false;
			}


			std::cout<<"Pool create shared pipe:"<<golobal_name<<std::endl;
			shared_pipe<PIPE_SIZE>* sp = new shared_pipe<PIPE_SIZE>(golobal_name);
			sp->init(true);
			return true;
		}

		return false;

	};
	//register a shared pipe
	static bool Reg_sh_pipe(std::string sh_name){
		sh_name = sh_name + "_pipe";

		if (check_redefine(sh_name))
		{
			return false;
		}

		std::map<std::string , pipe_pair>::iterator it = m_pipe_pair_.find(sh_name);

		if (it != m_pipe_pair_.end())
		{
			return false;
		}


		pipe_pair sh;
		sh.golobal_name = sh_name;
		sh.th1 = "";
		sh.th2 = "";
		sh.ptr_ = 0;
		m_pipe_pair_[sh_name] = sh; 

		std::cout<<"Pool create shared pipe:"<<sh_name<<std::endl;
		shared_pipe<PIPE_SIZE>* sp = new shared_pipe<PIPE_SIZE>(sh_name);
		sp->init(true);

		return true;
	};
	static bool insert_pipe_pair(std::string th_name1,std::string th_name2,std::string msg_name,std::string &golobal_name){
		std::map<std::string,pipe_pair>::iterator it = m_pipe_pair_.begin();
		for (; it != m_pipe_pair_.end(); ++it)
		{
			pipe_pair &mp(it->second);
			if ((mp.th1 == th_name1) && (mp.th2 == th_name2) && (mp.msg == msg_name)){
				return false;
			}

		}

		//get a golobal name
		golobal_name = get_name(th_name1,th_name2,msg_name);
		//msg set
		pipe_pair pp;
		pp.th1 = th_name1;
		pp.th2 = th_name2;
		pp.msg = msg_name;
		pp.golobal_name = golobal_name;
		pp.ptr_ = 0;
		m_pipe_pair_[golobal_name] = pp;

		return true;
	};
	//register a shared lock
	static bool Reg_sh_lock(std::string sh_name){
		
		if (check_redefine(sh_name))
		{
			return false;
		}

		std::map<std::string , lock_pair>::iterator it = m_lock_pair_.find(sh_name);

		if (it != m_lock_pair_.end())
		{
			return false;
		}


		lock_pair sh;
		sh.sh_name = sh_name;
		sh.ptr_ = 0;
		m_lock_pair_[sh_name] = sh; 
		
		std::cout<<"Pool create shared lock:"<<sh_name<<std::endl;
		shared_lock* sr = new shared_lock(sh_name);
		sr->init(true);

		return true;
	};
	//register a shared range data
	static bool Reg_sh_range(std::string sh_name,U32 dsize){

		if (check_redefine(sh_name))
		{
			return false;
		}
		std::map<std::string , sh_pair>::iterator it = m_sh_pair_.find(sh_name);
		
		if (it != m_sh_pair_.end())
		{
			return false;
		}
		
		
		sh_pair sh;
		sh.sh_name = sh_name;
		sh.dsize_ = dsize;
		sh.ptr_ = 0;
		m_sh_pair_[sh_name] = sh; 

		std::cout<<"Pool create shared range:"<<sh_name<<" size:"<<dsize<<std::endl;
		shared_range* sr = new shared_range(sh_name,dsize);
		sr->init(true);
		
		return true;
	};
	static bool Reg_sh_map(std::string sh_name,U32 dsize){
		if (!Reg_sh_range(sh_name,dsize))
		{
			return false;
		}
		if (!Reg_sh_lock(sh_name + "_lock"))
		{
			return false;
		}
		
		return true;
	};
	static bool Shared_Pipe_Pop(std::string sh_name,std::string &name,U8* data,U32 &len){

		std::map<std::string,pipe_pair>::iterator it = m_pipe_pair_.find(sh_name);
		if (it == m_pipe_pair_.end())
		{
			std::cout<<"err call set ptr ,shared pipe name:"<<sh_name<<std::endl;
			return false;
		}else{

			shared_pipe<PIPE_SIZE>* sh_ptr = 0;

			if (!it->second.ptr_)
			{
				sh_ptr = new shared_pipe<PIPE_SIZE>(sh_name);
				sh_ptr->init();
				it->second.ptr_ = sh_ptr;
			}
			else
			{
				sh_ptr = static_cast<shared_pipe<PIPE_SIZE>*>(it->second.ptr_);
			}

			sh_ptr->pop_front(name,data,len);

		}
		return true;
	};
	static bool Shared_Pipe_Push(std::string sh_name,std::string name,U8* data,U32 len){
		sh_name += "_pipe";
		std::map<std::string,pipe_pair>::iterator it = m_pipe_pair_.find(sh_name);
		if (it == m_pipe_pair_.end())
		{
			std::cout<<"err call set ptr ,shared pipe name:"<<sh_name<<std::endl;
			return false;
		}else{

			shared_pipe<PIPE_SIZE>* sh_ptr = 0;

			if (!it->second.ptr_)
			{
				sh_ptr = new shared_pipe<PIPE_SIZE>(sh_name);
				sh_ptr->init();
				it->second.ptr_ = sh_ptr;
			}
			else
			{
				sh_ptr = static_cast<shared_pipe<PIPE_SIZE>*>(it->second.ptr_);
			}

			sh_ptr->push_back(name,data,len);

		}
		return true;
	};
	static bool Publish_pipe(std::string th_name1,std::string msg_name,std::string name,U8* data,U32 len,bool wait_full = true){
		std::map<std::string,pipe_pair> pub_list;
		pub_ref_pipe(th_name1,msg_name,pub_list);
		if(pub_list.size() < 1){
			std::cout<<"pub pipe err : check core shared pool init thread:"<<th_name1<<" msg:"<<msg_name<<std::endl;
			return false;
		}

		std::map<std::string,pipe_pair>::iterator it = pub_list.begin();
		for ( ; it != pub_list.end() ; ++it)
		{
			pipe_pair &pp(it->second);

			shared_pipe<PIPE_SIZE>* sh_ptr = 0;

			if (!it->second.ptr_)
			{
				sh_ptr = new shared_pipe<PIPE_SIZE>(pp.golobal_name);

				if(sh_ptr->init()){
					set_pipe_ptr(pp.golobal_name,sh_ptr);
				}else{
					std::cout<<"publish pipe err : check core shared pool init with:"<<pp.golobal_name<<std::endl;
					return false;
				}


			}
			else
			{
				sh_ptr = static_cast<shared_pipe<PIPE_SIZE>*>(it->second.ptr_);
			}

			sh_ptr->push_back(name,data,len,wait_full);
		}

		return true;

	};
	static bool Subscribe_Pipe(std::string th_name1,std::string msg_name,boost::function<void (std::string name,U8* data,U32 len)> fnc ){
		std::map<std::string,pipe_pair> sub_list;
		sub_ref_pipe(th_name1,msg_name,sub_list);

		if(sub_list.size() < 1){
			std::cout<<"sub pipe err : check core shared pool init thread:"<<th_name1<<" msg:"<<msg_name<<std::endl;
			return false;
		}

		std::map<std::string,pipe_pair>::iterator it = sub_list.begin();
		for ( ; it != sub_list.end() ; ++it)
		{
			pipe_pair &pp(it->second);

			shared_pipe<PIPE_SIZE>* sh_ptr = 0;

			if (!it->second.ptr_)
			{
				sh_ptr = new shared_pipe<PIPE_SIZE>(pp.golobal_name);
				if(sh_ptr->init()){
					set_pipe_ptr(pp.golobal_name,sh_ptr);
				}else{
					std::cout<<"subscribe pipe err : check core shared pool init with:"<<pp.golobal_name<<std::endl;
					return false;
				}
			}
			else
			{
				sh_ptr = static_cast<shared_pipe<PIPE_SIZE>*>(it->second.ptr_);
			}


			std::cout<<"create pipe sub thread:"<<pp.golobal_name<<std::endl;
			boost::thread th(boost::bind(&shared_pool::sub_pipe_thread,sh_ptr,fnc));
		}

		return true;
	};

	static void sub_pipe_thread(shared_pipe<PIPE_SIZE>* pshd,boost::function<void( std::string name,U8* data,U32 len )> fnc){

		std::cout<<"begin pipe thread"<<std::endl;
		std::string name;
		U8* data = new U8[PIPE_SIZE];
		U32 len = PIPE_SIZE;


		while(1){
			pshd->pop_front(name,data,len);
			fnc(name,data,len);
			//std::cout<<"pipe sub thread:"<<std::endl;
		}
	};
	static bool Shared_Range_Read(std::string sh_name,U8* out,U32 offset,U32 &len){
		std::map<std::string,sh_pair>::iterator it = m_sh_pair_.find(sh_name);
		if (it == m_sh_pair_.end())
		{
			std::cout<<"err call set ptr ,shared range name:"<<sh_name<<std::endl;
			return false;
		}else{

			shared_range* sh_ptr = 0;
			

			if (!it->second.ptr_)
			{
				sh_ptr = new shared_range(sh_name,it->second.dsize_ );
				sh_ptr->init();
				it->second.ptr_ = sh_ptr;
			}
			else
			{
				sh_ptr = static_cast<shared_range*>(it->second.ptr_);
			}
			
			return sh_ptr->read(out,offset,len);

		}
	};

	static bool Shared_Range_Write(std::string sh_name,U8* in,U32 offset,U32 &len){
		std::map<std::string,sh_pair>::iterator it = m_sh_pair_.find(sh_name);
		if (it == m_sh_pair_.end())
		{
			std::cout<<"err call set ptr ,shared range name:"<<sh_name<<std::endl;
			return false;
		}else{

			shared_range* sh_ptr = 0;


			if (!it->second.ptr_)
			{
				sh_ptr = new shared_range(sh_name,it->second.dsize_ );
				sh_ptr->init();
				it->second.ptr_ = sh_ptr;
			}
			else
			{
				sh_ptr = static_cast<shared_range*>(it->second.ptr_);
			}

			return sh_ptr->write(in,offset,len);

		}
	};

	//register a shared data between two thread; 
	//identify by two thread name and message name
	template<typename T>
	static bool Reg_msg(std::string th_name1,std::string th_name2,std::string msg_name,const T &t){
		
		std::string golobal_name;
		if (insert_msg_pair(th_name1 , th_name2 , msg_name , golobal_name))
		{
			if (check_redefine(golobal_name))
			{
				return false;
			}
			
			std::cout<<"Pool create shared data:"<<golobal_name<<std::endl;
			shared_data<T> shd(golobal_name);
			shd.init(true);
			return true;
		}

		return false;
	};
	
	//publish a message to another thread
	template<typename T>
	static bool Publish(std::string th_name1,std::string msg_name,const T &in){
		std::map<std::string,msg_pair> pub_list;
		pub_ref_msg(th_name1,msg_name,pub_list);
		if(pub_list.size() < 1){
			std::cout<<"pub err : check core shared pool init thread:"<<th_name1<<" msg:"<<msg_name<<std::endl;
			return false;
		}

		std::map<std::string,msg_pair>::iterator it = pub_list.begin();
		for ( ; it != pub_list.end() ; ++it)
		{
			msg_pair &mp(it->second);

			shared_data<T>* send_ptr = 0;

			if (!mp.ptr_)
			{
				//shared_data<T> shd(mp.golobal_name);
				send_ptr = new shared_data<T>(mp.golobal_name);
				if(send_ptr->init()){
					set_ptr(mp.golobal_name,send_ptr);
				}else{
					std::cout<<"publish err : check core shared pool init with:"<<mp.golobal_name<<std::endl;
					return false;
				}
			}else{
				send_ptr = static_cast<shared_data<T>*>(mp.ptr_);
			}

			send_ptr->put(in);
		}
		
		return true;
	
	};
	template<typename T>
	static bool Subscribe(std::string th_name1,std::string msg_name,boost::function<void( const T &t)> fnc){

		std::map<std::string,msg_pair> sub_list;
		sub_ref_msg(th_name1,msg_name,sub_list);

		if(sub_list.size() < 1){
			std::cout<<"sub err : check core shared pool init thread:"<<th_name1<<" msg:"<<msg_name<<std::endl;
			return false;
		}

		std::map<std::string,msg_pair>::iterator it = sub_list.begin();
		for ( ; it != sub_list.end() ; ++it)
		{
			msg_pair &mp(it->second);

			shared_data<T>* send_ptr = 0;

			if (!mp.ptr_)
			{
				//shared_data<T> shd(mp.golobal_name);
				send_ptr = new shared_data<T>(mp.golobal_name);
				if(send_ptr->init()){
					set_ptr(mp.golobal_name,send_ptr);
				}else{
					std::cout<<"sub err : check core shared pool init with:"<<mp.golobal_name<<std::endl;
					return false;
				}


			}else{
				send_ptr = static_cast<shared_data<T>*>(mp.ptr_);
			}
			std::cout<<"create sub thread:"<<mp.golobal_name<<std::endl;
			boost::thread th(boost::bind(&shared_pool::sub_thread<T>,send_ptr,fnc));
		}

		return true;
	};
	template<typename T>
	static void sub_thread(shared_data<T>* pshd,boost::function<void( const T &t)> fnc){

		std::cout<<"begin thread"<<std::endl;
		T tmp;
		while(1){
			
			pshd->get(tmp);
			fnc(tmp);
			//std::cout<<"sub thread:"<<std::endl;
		}
	};
	static void save_msg(std::string path){
		std::ofstream fo(path.c_str());
		std::map<std::string,msg_pair>::iterator it = m_msg_pair_.begin();
		for (; it != m_msg_pair_.end(); ++it)
		{
			msg_pair &mp(it->second);
			fo<<mp.golobal_name<<" "<<mp.th1<<" "<<mp.th2<<" "<<mp.msg<<std::endl;
		}
	};
	static void save_sh(std::string path){

		std::ofstream fo(path.c_str());
		std::map<std::string , sh_pair>::iterator it2 = m_sh_pair_.begin();
 		for (; it2 != m_sh_pair_.end() ; ++it2)
 		{
 			sh_pair &mp(it2->second );
 			fo<<mp.sh_name<<" "<<mp.dsize_<<std::endl;
 		}
		
	};
	static void save_lock(std::string path){

		std::ofstream fo(path.c_str());
		std::map<std::string , lock_pair>::iterator it2 = m_lock_pair_.begin();
		for (; it2 != m_lock_pair_.end() ; ++it2)
		{
			lock_pair &mp(it2->second );
			fo<<mp.sh_name<<std::endl;
		}

	};
	static void save_pipe(std::string path){
		std::ofstream fo(path.c_str());
		std::map<std::string , pipe_pair>::iterator it2 = m_pipe_pair_.begin();
		for (; it2 != m_pipe_pair_.end() ; ++it2)
		{
			pipe_pair &mp(it2->second );
			fo<<mp.golobal_name<<" "<<mp.th1<<" "<<mp.th2<<" "<<mp.msg<<std::endl;
		}
	};
	static void load_msg(std::string path){
		std::ifstream fi(path.c_str());
		std::string str;
		std::vector<std::string> vlist;
		while(std::getline(fi,str)){
			cComm::SplitString(str," ",vlist);
			if (vlist.size() > 3)
			{
				msg_pair mp;
				mp.th1 = vlist[1];
				mp.th2 = vlist[2];
				mp.msg = vlist[3];
				mp.golobal_name = vlist[0];
				mp.ptr_ = 0;
				m_msg_pair_[mp.golobal_name] = mp;
			}
			
		}
		
	};

	static void load_sh(std::string path){
		std::ifstream fi(path.c_str());
		std::string str;
		std::vector<std::string> vlist;
		while(std::getline(fi,str)){
			cComm::SplitString(str," ",vlist);
			if (vlist.size() > 1)
			{
				sh_pair sh;
				sh.sh_name = vlist[0];
				cComm::ConvertToNum(sh.dsize_,vlist[1]);
				sh.ptr_    = 0;
		
				m_sh_pair_[sh.sh_name] = sh;
			}

		}

	};

	static void load_lock(std::string path){
		std::ifstream fi(path.c_str());
		std::string str;
		std::vector<std::string> vlist;
		while(std::getline(fi,str)){
			cComm::SplitString(str," ",vlist);
			if (vlist.size() > 0)
			{
				lock_pair sh;
				sh.sh_name = vlist[0];
				sh.ptr_    = 0;

				m_lock_pair_[sh.sh_name] = sh;
			}

		}

	};
	static void load_pipe(std::string path){
		std::ifstream fi(path.c_str());
		std::string str;
		std::vector<std::string> vlist;
		while(std::getline(fi,str)){
			cComm::SplitString(str," ",vlist);
			if (vlist.size() > 3)
			{

				pipe_pair pp;
				pp.th1 = vlist[1];
				pp.th2 = vlist[2];
				pp.msg = vlist[3];
				pp.golobal_name = vlist[0];
				pp.ptr_ = 0;
				m_pipe_pair_[pp.golobal_name] = pp;
			}else if(vlist.size() == 1){
				//std::cout<<"vlist size:"<<vlist.size()<<" string:"<<str<<std::endl;
				pipe_pair pp;
				pp.th1 = "";
				pp.th2 = "";
				pp.msg = "";
				pp.golobal_name = vlist[0];
				pp.ptr_ = 0;
				m_pipe_pair_[pp.golobal_name] = pp;
			}
		}

	};

	static void destructor(){
		std::map<std::string,msg_pair>::iterator it = m_msg_pair_.begin();
		for (; it != m_msg_pair_.end(); )
		{
			msg_pair &mp(it->second);
			SDelete(it->second.ptr_ );
			std::cout<<"desctructor msg:"<<mp.golobal_name<<std::endl;
			m_msg_pair_.erase(it++);

		}
		std::map<std::string , sh_pair>::iterator it2 = m_sh_pair_.begin();
		for (; it2 != m_sh_pair_.end() ; )
		{
			sh_pair &sp( it2->second );
			SDelete(it2->second.ptr_ );
			std::cout<<"desctructor shared range:"<<sp.sh_name<<std::endl;
			m_sh_pair_.erase(it2++);
		}

		std::map<std::string , lock_pair>::iterator it3 = m_lock_pair_.begin();
		for (; it3 != m_lock_pair_.end() ; )
		{
			lock_pair &sp( it3->second );
			SDelete(it3->second.ptr_ );
			std::cout<<"desctructor shared lock:"<<sp.sh_name<<std::endl;
			m_lock_pair_.erase(it3++);
		}

		std::map<std::string , pipe_pair>::iterator it4 = m_pipe_pair_.begin();
		for (; it4 != m_pipe_pair_.end() ; )
		{
			pipe_pair &sp( it4->second );
			SDelete(it4->second.ptr_ );
			std::cout<<"desctructor shared pipe:"<<sp.golobal_name<<std::endl;
			m_pipe_pair_.erase(it4++);
		}
		
	};
	static bool check_redefine(std::string shared_name){
		std::map<std::string , int >::iterator it = m_redefine_.find(shared_name);
		if (it == m_redefine_.end())
		{
			m_redefine_[shared_name] = 1;
			return false;
		}
		std::cout<<"err!!!!!! shared_name redefine:"<<shared_name<<std::endl;
		SLEEP(3);
		exit(0);
		return true;
	};
private:
	//get golobal name to identify 
	static std::string get_name(std::string th_name1,std::string th_name2,std::string msg_name){
		return (th_name1 + "-" + th_name2 + "-" + msg_name);
	};
	//insert name to map for searching
	static bool insert_msg_pair(std::string th_name1,std::string th_name2,std::string msg_name,std::string &golobal_name){


		std::map<std::string,msg_pair>::iterator it = m_msg_pair_.begin();
		for (; it != m_msg_pair_.end(); ++it)
		{
			msg_pair &mp(it->second);
			if ((mp.th1 == th_name1) && (mp.th2 == th_name2) && (mp.msg == msg_name)){
				return false;
			}

		}

		//get a golobal name 
		golobal_name = get_name(th_name1,th_name2,msg_name);
		//msg set
		msg_pair mp;
		mp.th1 = th_name1;
		mp.th2 = th_name2;
		mp.msg = msg_name;
		mp.golobal_name = golobal_name;
		mp.ptr_ = 0;
		m_msg_pair_[golobal_name] = mp;

		return true;
	};
	static void pub_ref_msg(std::string th_name1,std::string msg_name,std::map<std::string,msg_pair> &pub_list){
		std::map<std::string,msg_pair>::iterator it = m_msg_pair_.begin();
		for (; it != m_msg_pair_.end(); ++it)
		{
			msg_pair &mp(it->second);
			if ((mp.th1 == th_name1) && (mp.msg == msg_name))
			{
				pub_list.insert(*it);

			}
			
		}
		
	};
	static void sub_ref_msg(std::string th_name1,std::string msg_name,std::map<std::string,msg_pair> &sub_list){
		std::map<std::string,msg_pair>::iterator it = m_msg_pair_.begin();
		for (; it != m_msg_pair_.end(); ++it)
		{
			msg_pair &mp(it->second);
			if ((mp.th2 == th_name1) && (mp.msg == msg_name))
			{
				sub_list.insert(*it);

			}

		}

	};

	static void pub_ref_pipe(std::string th_name1,std::string msg_name,std::map<std::string,pipe_pair> &pub_list){
		std::map<std::string,pipe_pair>::iterator it = m_pipe_pair_.begin();
		for (; it != m_pipe_pair_.end(); ++it)
		{
			pipe_pair &pp(it->second);
			if ((pp.th1 == th_name1) && (pp.msg == msg_name))
			{
				pub_list.insert(*it);

			}

		}

	};
	static void sub_ref_pipe(std::string th_name1,std::string msg_name,std::map<std::string,pipe_pair> &sub_list){
		std::map<std::string,pipe_pair>::iterator it = m_pipe_pair_.begin();
		for (; it != m_pipe_pair_.end(); ++it)
		{
			pipe_pair &pp(it->second);
			if ((pp.th2 == th_name1) && (pp.msg == msg_name))
			{
				sub_list.insert(*it);

			}

		}

	};
	static bool set_ptr(std::string golobal_name,void* ptr){
		std::map<std::string,msg_pair>::iterator it = m_msg_pair_.find(golobal_name);
		if (it == m_msg_pair_.end())
		{
			std::cout<<"err call set ptr ,shared data name:"<<golobal_name<<std::endl;
			return false;
		}else{
			it->second.ptr_ = ptr;
			return true;
		}

	};
	static bool set_pipe_ptr(std::string golobal_name,shared_pipe<PIPE_SIZE>* ptr){
		std::map<std::string,pipe_pair>::iterator it = m_pipe_pair_.find(golobal_name);
		if (it == m_pipe_pair_.end())
		{
			std::cout<<"err call set ptr ,pipe shared data name:"<<golobal_name<<std::endl;
			return false;
		}else{
			it->second.ptr_ = ptr;
			return true;
		}

	};
// 	static bool set_sh_ptr(std::string sh_name,void* ptr){
// 		
// 	};
	static void def_shutdown(){
		msg_pair mp;
		mp.th1 = "interprocess_core";
		mp.th2 = nm_;
		mp.msg = "shutdown";
		mp.golobal_name = get_name("interprocess_core",nm_,"shutdown");
		mp.ptr_ = 0;
		m_msg_pair_[mp.golobal_name] = mp;
	};
	
	static std::map<std::string,  msg_pair > m_msg_pair_;
	static std::map<std::string , sh_pair > m_sh_pair_;
	static std::map<std::string , lock_pair > m_lock_pair_;
	static std::map<std::string , pipe_pair > m_pipe_pair_;

	static std::map<std::string , int > m_redefine_;

	static std::string nm_;
};


#endif//_SHARED_POOL_WANGHONGTAO_2015_09_24_
