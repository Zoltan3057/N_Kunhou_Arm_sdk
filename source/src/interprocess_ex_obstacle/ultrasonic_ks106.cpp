#include "math.h"
#include <boost/thread.hpp>
#include "Comm.h"
#include "robot/Geometry.h"
#include "TransferDevice.h"

#include "ultrasonic_ks106.h"

ultrasonic_ks106::ultrasonic_ks106(){

}

ultrasonic_ks106::~ultrasonic_ks106(){

}
void ultrasonic_ks106::init(const std::string port){


	port_ = port;
	open_transfer_device();

	SKs106 ks;
	ks.add =  0x30;
	ks.range = 0;
	ks.dis = 0;
	m_channel_[0] = ks;

	ks.add =  0x38;
	ks.range = 0;
	ks.dis = 0;
	m_channel_[1] = ks;

	ks.add =  0x40;
	ks.range = 0;
	ks.dis = 0;
	m_channel_[2] = ks;

	ks.add =  0x48;
	ks.range = 0;
	ks.dis = 0;
	m_channel_[3] = ks;

	//read init data
	U8 rec[100] = {0};
	int rlen = 0;
	if(pTransDevice_ && pTransDevice_->m_Status.IsOpen()){
		pTransDevice_->ReadData(rec,rlen,47,1000);
	}

}
bool ultrasonic_ks106::open_transfer_device()
{

	cConnPara conn_para;
	conn_para.m_iType = cConnPara::SERIALPORT ;
	conn_para.setDeviceName(port_);
	conn_para.nBaud = 9600;
	conn_para.chParity = 'N';
	conn_para.nDataBits = 8;
	conn_para.nStopBits = 1;
	conn_para.b_rs485 = true;



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
void ultrasonic_ks106::close_device()
{
	if (pTransDevice_)
	{
		pTransDevice_->Close();
		SDelete(pTransDevice_);
	}
}
bool ultrasonic_ks106::get_data(){


	F32 err_range = 0.5;
	bool b_err = false;

	std::map<int,SKs106>::iterator it = m_channel_.begin();
	for( ; it != m_channel_.end() ; ++it ){
		if( !get_channel_data(it->first) ){
			std::cout<<"err channel:"<<it->first<<std::endl;
		}else{
			if( it->second.dis < err_range){
				b_err = true;
				std::cout<<"err channel cloest:"<<it->first<<std::endl;
			}
		}
	}

	return b_err;
}
bool ultrasonic_ks106::get_channel_data(int ichannel){

	U8 send[26] = {0};
	send[0] = 0xe8;
	send[1] = 0x02;
	send[2] = 0x30;
	//send

	std::map<int,SKs106>::iterator it = m_channel_.find(ichannel);
	if(it != m_channel_.end() ){
		SKs106 &ks(it->second);
		send[2] = ks.add;
		if( pTransDevice_->m_Status.IsOpen() ){
			pTransDevice_->WriteData(send,3);

			U8 rec[100] = {0};
			int rlen = 0;
			pTransDevice_->ReadData(rec,rlen,1,1000);
			if( rlen > 1 ){

				U8* p = (U8*)&(ks.range);
				*p = rec[1];
				*(p + 1) = rec[0];
				ks.dis = (F32)ks.range / 1000;
				return true;
			}
		}
	}
	return false;
}

void ultrasonic_ks106::set_channel_fieldangle( int angle )
{

	//0xe8 0x02 0x9c 
	//0xe8 0x02 0x95 
	//0xe8 0x02 0x98 
	//0xe8 0x02 0x7d
	//SLEEP(2000);
}
