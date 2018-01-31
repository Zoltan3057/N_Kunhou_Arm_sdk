#include "math.h"
#include <boost/thread.hpp>
#include "Comm.h"
#include "TransferDevice.h"

#include "plc/plc_base.h"



PLC_base::PLC_base()
{
	init();
}

PLC_base::~PLC_base()
{
	
}

void PLC_base::init()
{
#ifdef _WIN32
	port_ = "COM1";
#else
	port_ = "ttyUSB0";
#endif
	pTransDevice_ = 0;

	memset( &di_ai_ , 0 , sizeof(SDIAI));

	modbus_tcp_head_.t1_ = 0;
	modbus_tcp_head_.t2_ = 0;
	modbus_tcp_head_.p1_ = 0;
	modbus_tcp_head_.p2_ = 0;
	modbus_tcp_head_.len0_ = 0;
	modbus_tcp_head_.len1_ = 0;
	modbus_tcp_head_.add_ = 2;
	modbus_tcp_head_.unused_ = 0;
}

bool PLC_base::init( std::string para  )
{
	//init para
	std::vector<std::string> vpara;
	cComm::SplitString(para,";",vpara);

	std::vector<std::string>::iterator it = vpara.begin();
	for (; it != vpara.end() ; ++it)
	{
		std::string &para_pair = *it;
		if (para_pair.length())
		{
			std::vector<std::string> vpara2;
			cComm::SplitString(para_pair,":",vpara2);
			if (vpara2.size() > 1)
			{
				std::cout<<"para 1:"<<vpara2[0]<<" para2:"<<vpara2[1]<<std::endl;
				setPara(vpara2[0],vpara2[1]);
			
			}
		}
	}

	return true;
}



void PLC_base::set_DO( const U32 &id,const S8 &value )
{

}
void PLC_base::set_AO( const U32 &id,const S32 &value )
{
	if (id < MAX_AO)
	{
		do_ao_.ao_[id] = value ;
	}
}
void PLC_base::get_DI( const U32 &id,S8 &value )
{

}
void PLC_base::get_AI( const U32 &id,S32 &value )
{
	value = -1;

// 	if (!get_PLC_AI())
// 	{
// 		std::cout<<"PLC Base get AI communication err!"<<std::endl;
// 		return;
// 	}

	if (id < MAX_AI)
	{
		value = di_ai_.ai_[id] ;
	}

}

bool PLC_base::network_port(std::string ip){
	std::vector<std::string> vstr;
	cComm::SplitString(ip,".",vstr);
	if (vstr.size()>3)
	{
		return true;
	}else{
		return false;
	}

}

bool PLC_base::open_transfer_device()
{
	cConnPara conn_para;
	if (network_port(port_))
	{
		conn_para.m_iType = cConnPara::NETWORK;
		conn_para.m_nNetType = cConnPara::TCPCLIENT;
		conn_para.m_nRemoteIP = port_;
		conn_para.m_nRemotePort = 502;//
	}else{
		conn_para.m_iType = cConnPara::SERIALPORT ;
		conn_para.setDeviceName(port_);
		conn_para.nBaud = 115200;
		conn_para.chParity = 'N';
		conn_para.nDataBits = 8;
		conn_para.nStopBits = 1;
	}
	

	pTransDevice_ = cTransferDevice::GetInstance(conn_para);
	U8 wait = 10;
	//if (sport_->Init(port_,115200))
	while(wait--)
	{
		if(pTransDevice_ && pTransDevice_->m_Status.IsOpen()){
			return true;
		}
		SLEEP(500);
	}
	return false;
}

void PLC_base::close_transfer_device()
{
	if (pTransDevice_)
	{
		pTransDevice_->Close();
		SDelete(pTransDevice_);
	}
}

void PLC_base::SendData( U8* s_data,U16 len )
{
	if (pTransDevice_)
	{
		//std::string str = cComm::ByteToHexString(s_data,len);
		//std::cout<<"send str:"<<str<<std::endl;
		pTransDevice_->WriteData(s_data,len);
	}
}

void PLC_base::ReadData( U8* r_data,int &len,int need,int timeout )
{
	if (pTransDevice_)
	{
		pTransDevice_->ReadData(r_data,len,need,timeout);
	}
}

void PLC_base::set_DI( const U32 &id,const S8 &value )
{
	if (id < MAX_DI)
	{
		di_ai_.di_[id] = value;
	}
}

void PLC_base::set_AI( const U32 &id,const S32 &value )
{
	if (id < MAX_AI)
	{
		di_ai_.ai_[id] = value;
	}
}

void PLC_base::get_DO( const U32 &id,S8 &value )
{
	if (id < MAX_DO)
	{
		value = do_ao_.do_[id] ;
	}
}

void PLC_base::get_AO( const U32 &id,S32 &value )
{
	if (id < MAX_DO)
	{
		value = do_ao_.ao_[id] ;
	}
}

void PLC_base::get_DI_AI( SDIAI &value )
{
	boost::mutex::scoped_lock lock(mu_read_write_);
	value = di_ai_;
}


