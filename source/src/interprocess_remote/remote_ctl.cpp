#include "math.h"
#include <boost/thread.hpp>
#include "MyDefine.h"
#include "Comm.h"

#include "TransferDevice.h"

#include "remote_ctl.h"

#define HEAD_STR 0xff
#define CRC0_STR 0xa5
#define CRC1_STR 0x5a

#define KEY1 (0)
#define KEY2 (1)
#define KEY3 (2)

remote_ctl::remote_ctl()
{
	port_ = "COM1";
	sta_ = HEAD;
	last_sequence_ = 0;
	memset(&data_,0,sizeof(SRemote_data));
	first_flag_ = true;
}

remote_ctl::~remote_ctl()
{
	stop();
}
void remote_ctl::init(const std::string port)
{
	port_ = port;
	open_transfer_device();
	boost::thread th(boost::bind(&remote_ctl::thread_loop,this));
}
bool remote_ctl::open_transfer_device()
{
	close_driver();
	//sport_ = new SerialPort();

	cConnPara conn_para;
	conn_para.m_iType = cConnPara::SERIALPORT ;
	conn_para.setDeviceName(port_);
	conn_para.nBaud = 9600;
	conn_para.chParity = 'N';
	conn_para.nDataBits = 8;
	conn_para.nStopBits = 1;

	pTransDevice_ = cTransferDevice::GetInstance(conn_para);

	//if (sport_->Init(port_,115200))
	U32 wait = 200;
	if (pTransDevice_ )
	{
		while(wait--){

			if (pTransDevice_->m_Status.IsOpen())
			{
				std::cout<<"remote open 9600 ok"<<std::endl;
				return true;
			}
			
			SLEEP(100);
		}


	}
	return false;

}
void remote_ctl::close_driver()
{
	if (pTransDevice_)
	{
		cConnPara ConnPara;
		pTransDevice_->GetPara(ConnPara);
		cTransferDevice::DelInstance(ConnPara);
		pTransDevice_ = 0;
	}

}

void remote_ctl::stop()
{
	thread_run_ = false;
	SLEEP(1000);
	std::cout<<"stop remote loop"<<std::endl;
}
void remote_ctl::thread_loop(){

	

	thread_run_ = true;

	U8 data[102400];
	int len = 8;
	while(thread_run_){
		len = 0;
		pTransDevice_->ReadData(data,len,8,1000);
		for (int i = 0; i < len ; ++i)
		{
			state_mathine(data[i]);
		}
		
		
	}

}

void remote_ctl::state_mathine( U8 data )
{
	
	switch (sta_)
	{
	case HEAD:
		if( HEAD_STR == data ){
			sta_ = KEY;
		}else{
			sta_ = HEAD;
		}
		break;
	case KEY:
		if ( ~(0x07) & data)
		{
			sta_ = HEAD;
		}else{
			key_ = data;	
			sta_ = AD1_0;
		}
		break;

	case AD1_0:

		ad_1 = data;	
		sta_ = AD1_1;

		break;
	case AD1_1:

		ad_1 |= data<<8;	
		sta_ = AD2_0;

		break;
	case AD2_0:

		ad_2 = data;	
		sta_ = AD2_1;

		break;

	case AD2_1:

		ad_2 |= data<<8;	
		sta_ = CRC0;

		break;
	case CRC0:
		if ( CRC0_STR == data)
		{
			sta_ = CRC1;
		}else{
			sta_ = HEAD;
		}
		break;

	case CRC1:
		if ( CRC1_STR == data)
		{
			boost::mutex::scoped_lock lock(mu_data_);
			setKey();
			setAd();
			sta_ = HEAD;
		}else{
			sta_ = HEAD;
		}
		break;

	}
	

}

void remote_ctl::setKey()
{
	data_.key1_ = (key_ & (1<<KEY1));
	data_.key2_ = (key_ & (1<<KEY2));
	data_.key3_ = (key_ & (1<<KEY3));
	//std::cout<<"key1_:"<<int(data_.key1_)<<" key2_:"<<int(data_.key2_)<<" key3_:"<<int(data_.key3_)<<std::endl;
}

void remote_ctl::setAd()
{
	data_.new_ad1_ = ad_1;
	data_.new_ad2_ = ad_2;

	data_.sequence_++;
	//std::cout<<"new_ad1_:"<<int(data_.new_ad1_)<<" new_ad2_:"<<data_.new_ad2_<<std::endl;
}

bool remote_ctl::get_data(SRemote_data &data)
{
	
	
	boost::mutex::scoped_lock lock(mu_data_);
	data = data_;
	if ( (last_sequence_ != 0) && (last_sequence_ == data_.sequence_) )
	{
		if (err_count_ < 200)
		{
			err_count_++;
		}
		
		
	}else{
		
		err_count_ = 0;
	}
	
	if(last_sequence_ != 0){
		first_flag_ = false;
	}


	last_sequence_ = data_.sequence_;

	if (first_flag_)
	{
		
		return false;
	}
	
	if ( err_count_ > 30)
	{
		return false;
	}
	
	

	return true;
}





