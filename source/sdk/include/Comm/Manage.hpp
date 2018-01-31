#ifndef AGV_SIMULATER_MANAGE_20121102_
#define AGV_SIMULATER_MANAGE_20121102_

#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "MyDefine.h"

template <typename T>
class Manage{
public:
	Manage();
	~Manage();

	static T* Create();
	static T* Create(const U32& id);
	static void Save(std::string path);
	static void Load(std::string path);


	static void AddtoList(T* t);
	static T* Get(U32 id);
	static void Del(T* t);
	static void DeleteAll();
	static T* GetNext(U32 id);
	static T* GetFirst();
	static std::size_t size(){
		return t_list_.size();
	};

private:
	static std::map<U32,T*> t_list_;
};
template <typename T>
std::map<U32,T*> Manage<T>::t_list_;

template <typename T>
Manage<T>::~Manage()
{

}
template <typename T>
Manage<T>::Manage()
{

}
template <typename T>
void Manage<T>::DeleteAll()
{
	typename std::map<U32,T*>::iterator it = t_list_.begin();
 	for (;it != t_list_.end();++it)
 	{
		delete it->second;
	}
	t_list_.clear();
}
template <typename T>
void Manage<T>::Del( T* t )
{

	typename std::map<U32,T*>::iterator it = t_list_.begin();
	for (;it != t_list_.end();++it)
	{
		if (it->second == t)
		{
			
			t_list_.erase(it);
			break;
		}

	}
	delete t;
}
template <typename T>
T* Manage<T>::Get(U32 id)
{
	typename std::map<U32,T*>::iterator it = t_list_.find(id);
	if(it != t_list_.end())
	{
		return it->second;
	}
	return NULL;
}
template <typename T>
T* Manage<T>::GetFirst(){

	typename std::map<U32,T*>::iterator it = t_list_.begin();
	if(it != t_list_.end())
	{
		return it->second; 
	}
	return NULL;
}
template <typename T>
T* Manage<T>::GetNext(U32 id){
	

	typename std::map<U32,T*>::iterator it = t_list_.find(id);
	if(it != t_list_.end())
	{
		it++;
		if(it != t_list_.end()){
			return it->second;
		}else{
			return 0;
		}
	}
	return 0;
}

template <typename T>
void Manage<T>::AddtoList( T* t )
{
	if (t_list_.count(t->ID()) == 0)
	{
		t_list_.insert(typename std::map<U32,T*>::value_type(t->ID(),t));
	}
}
template <typename T>
void Manage<T>::Load( std::string path )
{
	DeleteAll();

	std::ifstream f(path.c_str());
	std::vector<std::string> vstr;
	while (f.good())
	{
		std::string str;
		std::getline(f,str);
		if (str.length()==0)
		{
			continue;
		}
		T* t = new T();
		
		if (!t->DeCode(str))
		{
			SDelete(t) ;
		}
		if (t)
		{
			AddtoList(t);
		}

	}
	f.close();
}
template <typename T>
void Manage<T>::Save( std::string path )
{
	std::ofstream of(path.c_str(),std::ios::trunc | std::ios::binary);

	typename std::map<U32,T*>::iterator it = t_list_.begin();
	for (;it != t_list_.end();++it)
	{
		T* t = it->second;
		t->Code(of);
	}
	of.close();
}
template <typename T>
T* Manage<T>::Create()
{
	T* t = new T();
	t->Init();
	AddtoList(t);
	return t;
}
template <typename T>
T* Manage<T>::Create(const U32& id)
{
	T* t = new T();
	t->Init(id);
	AddtoList(t);
	return t;
}




#endif //AGV_SIMULATER_MANAGE_20121102_
