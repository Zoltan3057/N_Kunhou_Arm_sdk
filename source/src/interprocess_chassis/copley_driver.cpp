
#include "math.h"
#include "iostream"
#include "Comm.h"


//#include "SPort.h"
#include "TransferDevice.h"
#include "copley_driver.h"

#define LEN_MAX 1024



copley_driver::copley_driver():pTransDevice_(0),counts_(10000),reduction_ratio_(60),one_wheel_counts_(0)
{
	port_ = "";
	one_wheel_counts_ = reduction_ratio_*counts_;
	first_odo_ = true;

	init_customer_para();
}

copley_driver::~copley_driver()
{


}

void copley_driver::setPara( std::string name ,std::string value )
{

	if (name == "port")
	{
		port_ = value;
	}else if(name == "counts"){
		cComm::ConvertToNum(counts_,value);
		cComm::RangeIt(counts_,(U32)0,(U32)100000);
		std::cout<<"counts_:"<<counts_<<std::endl;
	}else if(name == "reduction"){
		cComm::ConvertToNum(reduction_ratio_,value);
		cComm::RangeIt(reduction_ratio_,(F32)0,(F32)200.0);
		std::cout<<"reduction_ratio_:"<<reduction_ratio_<<std::endl;
	}else{
		std::cout<<"err para:"<<name<<" value:"<<value<<std::endl;
	}

	one_wheel_counts_ = reduction_ratio_*counts_;
	std::cout<<"one_wheel_counts_:"<<one_wheel_counts_<<std::endl;
}

void copley_driver::setRPM( int id,F32 v )
{
	//set left driver speed
	S32 speed = 0 ;  //0.1 counts/s

	if (id == 0)
	{
		left_speed_ = v * reduction_ratio_ * counts_ / 60 ;	//counts / s

		speed = left_speed_ * 10;//0.1 counts/s
	

	}else{

		right_speed_ = v * reduction_ratio_ * counts_ / 60; //counts / s
		
		speed = right_speed_ * 10;//0.1 counts/s

	}

	int sz,ret,value,err;
	U8 buf[32];
	U8 res[32];
	sz=sprintf((char *)buf,"%d s r0x%x %d\r",id,TARGET_SPEED,speed);

	SendData(buf,sz);
	ReadData(res,ret);
	
}
F32 copley_driver::getSpeed( int id )
{
	if (id == 0)
	{
		return c_left_;
	}else{
		return c_right_;
	}
}

F32 copley_driver::getDiffAngle( int id )
{
	
	F32 dangle = 0;

	dangle = GetDiffPos(id);
	dangle /= one_wheel_counts_;

	return dangle;

}

bool copley_driver::open_transfer_device()
{
	close_driver();
	//sport_ = new SerialPort();

	cConnPara conn_para;
	conn_para.m_iType = cConnPara::SERIALPORT ;
	conn_para.setDeviceName(port_);
	conn_para.nBaud = 9600;      //������
	conn_para.chParity = 'N';//У��λ
	conn_para.nDataBits = 8;  //���λ
	conn_para.nStopBits = 1;  //ֹͣλ

	pTransDevice_ = cTransferDevice::GetInstance(conn_para);

	//if (sport_->Init(port_,115200))
	U32 wait = 20;
	if (pTransDevice_ )
	{
		while(wait--){

			if (pTransDevice_->m_Status.IsOpen())
			{
				std::cout<<"copley open 9600 ok"<<std::endl;
				return true;
			}

			SLEEP(100);
		}


	}
	return false;

}

bool copley_driver::init_driver(){

	int sz,ret,value,err;

	U8 buf[32];
	U8 res[32];

	while(1){
		sz=sprintf((char *)buf,"0 s\r");
		//write(hComm, buf, sz);
		SendData(buf,sz);
		while(sz>0){
			sz--;
			SLEEP(1);
		}
		//ret=this->rcvmsg(hComm,&value,&err);
		ReadData(res,ret);
		break;
	}

	sz=sprintf((char *)buf,"0 s r0x90 115200\r");
	//write(hComm, buf, sz);
	SendData(buf,sz);

	while(sz>0){
		sz--;
		SLEEP(1);
	}

	close_driver();


	cConnPara conn_para;
	conn_para.m_iType = cConnPara::SERIALPORT ;
	conn_para.setDeviceName(port_);
	conn_para.nBaud = 115200;      //������
	conn_para.chParity = 'N';//У��λ
	conn_para.nDataBits = 8;  //���λ
	conn_para.nStopBits = 1;  //ֹͣλ

	pTransDevice_ = cTransferDevice::GetInstance(conn_para);

	//if (sport_->Init(port_,115200))
	U32 wait = 50;
	if ( pTransDevice_ )
	{
		while(wait--){

			if (pTransDevice_->m_Status.IsOpen())
			{
				std::cout<<"copley open 115200 ok"<<std::endl;

				int sz,ret,value,err;
				U8 buf[32];
				U8 res[32];
				sz=sprintf((char *)buf,"%d s r0x%x %d\r",0,ACTURAL_POS,0);

				SendData(buf,sz);
				ReadData(res,ret);

				sz=sprintf((char *)buf,"%d s r0x%x %d\r",1,ACTURAL_POS,0);

				SendData(buf,sz);
				ReadData(res,ret);

				last_left_pos_ = left_pos_;
				last_right_pos_ = right_pos_;

				left_dt_.Begin();
				right_dt_.Begin();
				std::cout<<"first left_pos_:"<<left_pos_<<std::endl;
				std::cout<<"first right_pos_:"<<right_pos_<<std::endl;


				std::cout<<"copley init over!!!!!"<<std::endl;
				return true;
			}

			std::cout<<"copley open 115200 wait for"<<std::endl;

			SLEEP(100);
		}


	}
	return false;


}
void copley_driver::close_driver()
{
	if (pTransDevice_)
	{
		cConnPara ConnPara;
		pTransDevice_->GetPara(ConnPara);
		cTransferDevice::DelInstance(ConnPara);
		pTransDevice_ = 0;
	}

}
/////////////////////////////////////////////////////////////




void copley_driver::SendData( U8* s_data,U16 len )
{
	if (pTransDevice_)
	{
		pTransDevice_->WriteData(s_data,len);

	}
}

void copley_driver::ReadData( U8* r_data,int &len )
{
	len = 0;
	if (pTransDevice_)
	{

		for (int i = 0; i < 100 ; ++i)
		{
			int getlen = 0;
			pTransDevice_->ReadData(r_data + len,getlen,1,0);
			len += getlen;
			SLEEP(1);
			if(len && (r_data[len-1] ==0x0d )){
				break;
			}
		}

	}
}

void copley_driver::init_customer_para()
{
	left_speed_ = 0;
	right_speed_ = 0;


	left_pos_ = 0;
	last_left_pos_ = 0;
	deta_left_pos_ = 0;

	left_set_v_ = 0;
	left_current_v_ = 0;
	
	right_pos_= 0;		    //pos
	last_right_pos_ = 0;
	deta_right_pos_ = 0;

	right_set_v_ = 0;
	right_current_v_ = 0;

	c_left_ = 0;
	c_right_ = 0;

}

F32 copley_driver::GetDiffPos( int id )
{

	

	int sz,ret,err;
	S32 value;
	U8 buf[32];
	U8 res[32];
	memset(res,0,32);

	sz=sprintf((char *)buf,"%d g r0x%x\r",int(id),ACTURAL_POS);

	SendData(buf,sz);
	ReadData(res,ret);

	err=sscanf((char *)res,"v %d\r",&value);
// 	std::cout<<"id:"<<int(id)<<" value:"<<value<<std::endl;
// 	std::string str = (char *)res;
// 	std::cout<<"hex:"<<str<<std::endl;

	if (err != 1)
	{
		std::string err_cmd = cComm::ByteToHexString(res,ret);
		std::cout<<"err get pos id:"<<int(id)<<" str:"<<err_cmd<<std::endl;
		return 0;
	}


	if (id == 0)
	{
		left_pos_ = value;

		if (first_odo_)
		{
			last_left_pos_ = left_pos_;
			right_dt_.Begin();
			std::cout<<"first left_pos_:"<<left_pos_<<std::endl;

		}
		

		F64 dt = left_dt_.GetTime();
		dt /= 1000000;
		left_dt_.Begin();

		deta_left_pos_ = (left_pos_ - last_left_pos_)  ;
		//deta_left_pos_ = - deta_left_pos_;
	
		//std::cout<<"now left_pos_:"<<left_pos_<<" diff:"<<deta_left_pos_<<std::endl;
		if(abs(deta_left_pos_) > 100000){
			std::cout<<"err deta_left_pos_:"<<deta_left_pos_<<" left_pos_:"<<left_pos_<<" last_left_pos_:"<<last_left_pos_<<std::endl;
			deta_left_pos_ = 0;
		}
		
		c_left_ = deta_left_pos_;
		c_left_ /= counts_ * reduction_ratio_;
		c_left_ *= 60;
		if ( (fabs(dt) > 1e-6) && (fabs(dt) < 1.0))
		{
			c_left_ /= dt;
		}else{
			c_left_ = 0;
		}
		
	
		last_left_pos_ = left_pos_;
		
		return deta_left_pos_;

	}else{
		
		right_pos_ = value;

 		if (first_odo_)
 		{
 			last_right_pos_ = right_pos_;
 			right_dt_.Begin();
 			std::cout<<"first right_pos_:"<<right_pos_<<std::endl;

 			first_odo_ = false;
 		}

		F64 dt = right_dt_.GetTime();
		dt /= 1000000;
		right_dt_.Begin();

		deta_right_pos_ = (right_pos_ - last_right_pos_);
		deta_right_pos_ = -deta_right_pos_;

		//std::cout<<"now right_pos_:"<<right_pos_<<" diff:"<<deta_right_pos_<<std::endl;
		if(abs(deta_right_pos_) > 100000){
			std::cout<<"err deta_right_pos_:"<<deta_right_pos_<<" right_pos_:"<<right_pos_<<" last_right_pos_:"<<last_right_pos_<<std::endl;
			deta_right_pos_ = 0;
		}

		c_right_ = deta_right_pos_ ;
		c_right_ /= counts_ * reduction_ratio_;
		c_right_ *= 60;
		if ( (fabs(dt) > 1e-6) && (fabs(dt) < 1.0))
		{
			c_right_ /= dt;
		}else{
			c_right_ = 0;
		}
		
			
		last_right_pos_ = right_pos_;

		return deta_right_pos_;
	}
	

}

void copley_driver::setAngle( int id,F32 rad_angle )
{

}

F32 copley_driver::getAngle( int id )
{
	return 0;
}






