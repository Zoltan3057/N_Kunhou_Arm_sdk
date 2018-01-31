#ifndef SURO_PLATFORM_CORE_RPC_LOCAL_IP_H
#define SURO_PLATFORM_CORE_RPC_LOCAL_IP_H

#include <Singleton.hpp>

#include <iostream>

class LocalIp : public Singleton<LocalIp>{

public:
	LocalIp(){
		m_ip = "NULL";
		m_node_name = "_";
	}

	std::string getLocalIp(){
		return m_ip;
	}

	void setLocalIp(std::string ip){
		static bool set = false;
		if(!set){
			m_ip = ip;
			set = true;
		}
		//std::cout<<"&&&&&&&&& ip = "<< m_ip<<std::endl;
        }

public:
	std::string m_node_name;

private:

	std::string m_ip;
};

#define LOCAL_IP LocalIp::GetSingleton()


#endif
