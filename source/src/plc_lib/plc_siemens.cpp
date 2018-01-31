#include "math.h"

#include "Comm.h"
#include "plc/plc_siemens.h"

#define LEN_MAX 1024
#define LINDE 1
#define UP_SEQ_AI 0
#define UP_ANGLE_AI 1
#define UP_CODER_AI 2
#define UP_ID_AI 3
#define UP_CUR_STATION_AI 4
#define UP_CUR_STATE_AI 5

#define FORK_L_BIT 3
#define FORK_R_BIT 3
#define CARGO_BIT 4
#define SET_CURRENT_STA_ID_BIT 7

#define UP_FORK_L_DI 1
#define UP_FORK_R_DI 2
#define UP_CARGO_DI 0
#define UP_CURRENT_STA_DI 3

//#define DOWN_POS_X_AO 4
//#define DOWN_POS_Y_AO 5
//#define DOWN_IN_OUT_AO 6
#define DOWN_BACK1_AO 7

#define DOWN_SEQ_AI 0
#define DOWN_ANGLE_AI 4
#define DOWN_SPEED_AI 5
#define DOWN_FUNCTION_AI 1

#define DOWN_POS_X_AI 2
#define DOWN_POS_Y_AI 3
#define DOWN_STATUS_AI 6


#define DOWN_CLEAR_CURRENT_STA_DO 0

#define GET_IO_MOD 8
#define SET_IO_MOD 8

Plc_siemens::Plc_siemens()
{
	do_get_io_ = 0;
	do_set_io_ = 0;
}

Plc_siemens::~Plc_siemens()
{
	close_transfer_device();
}

void Plc_siemens::setPara( std::string name , std::string value )
{
	if (name == "port")
	{
		port_ = value;
		std::cout<<"port_:"<<value<<std::endl;
	}
}

#if LINDE
bool Plc_siemens::set_PLC()
{
	//02 03 00 0A 00 05 A5 F8 
	//02 03 0A 00 00 00 00 00 00 00 00 00 00 21 75 

	boost::mutex::scoped_lock lock(mu_read_write_);

	unsigned char ch_send[LEN_MAX] = {0};
	//02 10 00 0F 00 06 0C 00 01 00 02 00 00 00 03 00 04 00 05 D3 39
	//02 10 00 0F 00 06 70 3B
	ch_send[0] = 0x02;
	ch_send[1] = 0x10;
	ch_send[2] = 0x00;
	ch_send[3] = 0x0f;
	ch_send[4] = 0x00;
	ch_send[5] = 0x07;
	ch_send[6] = 0x0E;

	S32 plc_seq = 0;
	get_AO(DOWN_SEQ_AI,plc_seq);

	ch_send[7] = ( plc_seq & 0xff00) >> 8;
	ch_send[8] = ( plc_seq & 0x00ff) >> 0;
	
	
	S32 target_angle = 0;
	get_AO(DOWN_ANGLE_AI,target_angle);

	ch_send[9] = ( target_angle & 0x0000ff00) >> 8;
	ch_send[10] = ( target_angle & 0x000000ff) >> 0;

	S32 pos_x = 0;
	get_AO(DOWN_POS_X_AI,pos_x);
	//pos_x = 30000;
	ch_send[11] = ( pos_x & 0x0000ff00) >> 8;
	ch_send[12] = ( pos_x & 0x000000ff) >> 0;

	S32 pos_y = 0;
	get_AO(DOWN_POS_Y_AI,pos_y);
	//pos_y = -30001;
	ch_send[13] = ( pos_y & 0x0000ff00) >> 8;
	ch_send[14] = ( pos_y & 0x000000ff) >> 0;

	S32 target_speed = 0;
	get_AO(DOWN_SPEED_AI,target_speed);

	ch_send[15] = ( target_speed & 0x0000ff00) >> 8;
	ch_send[16] = ( target_speed & 0x000000ff) >> 0;

	S32 function_id = 0;
	get_AO(DOWN_FUNCTION_AI,function_id);

	ch_send[17] = ( function_id & 0x0000ff00) >> 8;
	ch_send[18] = ( function_id & 0x000000ff) >> 0;
	

	S32 robot_status = 0;
	get_AO(DOWN_STATUS_AI,robot_status);
	ch_send[19] = ( robot_status & 0x0000ff00) >> 8;
	ch_send[20] = ( robot_status & 0x000000ff) >> 0;


	U16 crc = cComm::CRC16( ch_send , 21 );
	ch_send[21] = (crc & 0x00ff)>>0;
	ch_send[22] = (crc & 0xff00)>>8;

	SendData(ch_send,23);

	unsigned char ch_read[100];
	int len = 0;
	ReadData(ch_read,len,8,1000);

	if ( ( len == 8 ) && ( 0x02 == ch_read[0] )  && ( cComm::Check_CRC16(ch_read,len) ))
	{
		SLEEP(1);

 		if (0 == do_set_io_)
 		{
 			set_PLC_io();
 		}
 		do_set_io_++;
		do_set_io_ = do_set_io_ % SET_IO_MOD;

		return true;
	}
	std::cout<<"siemens PLC write AO reg communication err! send len:"<<len<<" str:"<<cComm::ByteToHexString(ch_send,23)<<std::endl;
	std::cout<<"siemens PLC write AO reg communication err! read len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
	return false;
}
bool Plc_siemens::set_PLC_io()
{

	unsigned char ch_send[LEN_MAX] = {0};
	//02 10 00 16 00 04 08 00 01 00 02 00 03 00 04 D3 39
	//02 10 00 0F 00 06 70 3B
	ch_send[0] = 0x02;
	ch_send[1] = 0x10;
	ch_send[2] = 0x00;
	ch_send[3] = 0x15;
	ch_send[4] = 0x00;
	ch_send[5] = 0x04;
	ch_send[6] = 0x08;

	ch_send[7] = 0x00;
	ch_send[8] = 0x00;

	ch_send[9] = 0x00;
	ch_send[10] = 0x00;

	ch_send[11] = 0x00;
	ch_send[12] = 0x00;

	S8 vdo = 0;
	get_DO(DOWN_CLEAR_CURRENT_STA_DO,vdo);

	ch_send[13] = vdo;
	ch_send[14] = 0x00;


	U16 crc = cComm::CRC16( ch_send , 15 );
	ch_send[15] = (crc & 0x00ff)>>0;
	ch_send[16] = (crc & 0xff00)>>8;

	SendData(ch_send,17);

	unsigned char ch_read[100];
	int len = 0;
	ReadData(ch_read,len,8,1000);

	if ( ( len == 8 ) && ( 0x02 == ch_read[0] )  && ( cComm::Check_CRC16(ch_read,len) ))
	{
		SLEEP(1);
		return true;
	}
	std::cout<<"siemens PLC write DO reg communication err! send len:"<<len<<" str:"<<cComm::ByteToHexString(ch_send,17)<<std::endl;
	std::cout<<"siemens PLC write DO reg communication err! read len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
	return false;
}
#else
bool Plc_siemens::set_PLC()
{
	//02 03 00 0A 00 05 A5 F8 
	//02 03 0A 00 00 00 00 00 00 00 00 00 00 21 75 

	boost::mutex::scoped_lock lock(mu_read_write_);

	unsigned char ch_send[LEN_MAX] = {0};
	//02 10 00 0A 00 05 0A 01 02 03 04 05 06 07 08 09 0A 38 C6 
	//02 10 00 0A 00 05 20 3B 
	ch_send[0] = 0x02;
	ch_send[1] = 0x10;
	ch_send[2] = 0x00;
	ch_send[3] = 0x0A;
	ch_send[4] = 0x00;
	ch_send[5] = 0x05;
	ch_send[6] = 0x0A;

	S32 plc_seq = 0;
	get_AO(0,plc_seq);

	ch_send[7] = ( plc_seq & 0xff00) >> 8;
	ch_send[8] = ( plc_seq & 0x00ff) >> 0;

	S32 function_id = 0;
	get_AO(1,function_id);

	ch_send[9] = ( function_id & 0x0000ff00) >> 8;
	ch_send[10] = ( function_id & 0x000000ff) >> 0;

	U8 do_0_7 = 0;
	int i = 0;
	for ( i = 0 ; i < 8 ; ++i )
	{
		S8 value = 0;
		get_DO(i,value);
		if (value != 0)
		{
			do_0_7 |= (1<<i);
		}
	}
	ch_send[11] = do_0_7;

	U8 do_8_15 = 0;
	for ( i = 8 ; i < 16 ; ++i )
	{
		S8 value = 0;
		get_DO(i,value);
		if (value != 0)
		{
			do_8_15 |= (1<<i);
		}
	}
	ch_send[12] = do_8_15;

	S32 current_station = 0;
	get_AO(2,current_station);

	ch_send[13] = ( current_station & 0x0000ff00) >> 8;
	ch_send[14] = ( current_station & 0x000000ff) >> 0;

	S32 target_station = 0;
	get_AO(3,target_station);

	ch_send[15] = ( target_station & 0x0000ff00) >> 8;
	ch_send[16] = ( target_station & 0x000000ff) >> 0;

	S32 target_angle = 0;
	get_AO(4,target_angle);

	ch_send[17] = ( target_angle & 0x0000ff00) >> 8;
	ch_send[18] = ( target_angle & 0x000000ff) >> 0;

	S32 target_speed = 0;
	get_AO(5,target_speed);

	ch_send[19] = ( target_speed & 0x0000ff00) >> 8;
	ch_send[20] = ( target_speed & 0x000000ff) >> 0;

	U16 crc = cComm::CRC16( ch_send , 21 );
	ch_send[21] = (crc & 0x00ff)>>0;
	ch_send[22] = (crc & 0xff00)>>8;

	SendData(ch_send,23);

	unsigned char ch_read[100];
	int len = 0;
	ReadData(ch_read,len,8,1000);

	if ( ( len == 8 ) && ( 0x02 == ch_read[0] )  && ( cComm::Check_CRC16(ch_read,len) ))
	{
		SLEEP(1);
		return true;
	}
	std::cout<<"siemens PLC write AO reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
	return false;
}
#endif
bool Plc_siemens::get_PLC(){
	if(network_port(port_)){
		return get_PLC_tcp();
	}else{
		return get_PLC_rtu();
	}
}
#if LINDE
bool Plc_siemens::get_PLC_rtu()
{
	boost::mutex::scoped_lock lock(mu_read_write_);
	unsigned char ch_send[LEN_MAX] = {0};
	//send    02 03 00 00 00 06 C5 FB 
	//receive 02 03 0C 00 00 00 BC 06 00 00 00 00 00 00 00 30 9C 
	ch_send[0] = 0x02;
	ch_send[1] = 0x03;
	ch_send[2] = 0x00;
	ch_send[3] = 0x00;
	ch_send[4] = 0x00;
	ch_send[5] = 0x04;
	ch_send[6] = 0xa5;
	ch_send[7] = 0x5a;
	U16 crc = cComm::CRC16( ch_send , 6 );
	ch_send[6] = (crc & 0x00ff)>>0;
	ch_send[7] = (crc & 0xff00)>>8;

	SendData(ch_send,8);

	unsigned char ch_read[100];
	int len = 0;
	ReadData(ch_read,len,13,1000);

	if ( ( len == 13 ) && ( 0x02 == ch_read[0] )  && ( cComm::Check_CRC16(ch_read,len) ))
	{

		S32 value = 0;
		value = ch_read[3];
		value = value<<8;
		value += ch_read[4];
		set_AI( UP_SEQ_AI , value);
		//std::cout<<"AI 0:"<<value<<std::endl;

		value = ch_read[5];
		value = value<<8;
		value += ch_read[6];
		set_AI( UP_ANGLE_AI , value);
		//std::cout<<"AI 1:"<<value<<std::endl;

		S32 tmp = 0;
		tmp = ch_read[7];
		tmp = tmp<<24;
		value = tmp;
		tmp = ch_read[8];
		tmp = tmp<<16;
		value += tmp;
		tmp = ch_read[9];
		tmp = tmp<<8;
		value += tmp;
		value += ch_read[10];
		set_AI( UP_CODER_AI , value);
			
		//std::cout<<"AI 2:"<<value<<std::endl;


		SLEEP(1);

		if ( 0 == do_get_io_)
		{
			get_PLC_rtu_io();
		}
		do_get_io_++;
		do_get_io_ = do_get_io_ % GET_IO_MOD;
		//std::cout<<"siemens PLC get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
		return true;
	}
	std::cout<<"siemens PLC rtu get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
	return false;
}
bool Plc_siemens::get_PLC_rtu_io()
{

	unsigned char ch_send[LEN_MAX] = {0};
	//send    02 03 00 00 00 06 C5 FB 
	//receive 02 03 0C 00 00 00 BC 06 00 00 00 00 00 00 00 30 9C 
	ch_send[0] = 0x02;
	ch_send[1] = 0x03;
	ch_send[2] = 0x00;
	ch_send[3] = 0x04;
	ch_send[4] = 0x00;
	ch_send[5] = 0x07;
	ch_send[6] = 0xa5;
	ch_send[7] = 0x5a;
	U16 crc = cComm::CRC16( ch_send , 6 );
	ch_send[6] = (crc & 0x00ff)>>0;
	ch_send[7] = (crc & 0xff00)>>8;

	SendData(ch_send,8);

	unsigned char ch_read[100];
	int len = 0;
	ReadData(ch_read,len,19,1000);

	if ( ( len == 19 ) && ( 0x02 == ch_read[0] )  && ( cComm::Check_CRC16(ch_read,len) ))
	{

		S32 value = 0;
		value = ch_read[3];
		value = value<<8;
		value += ch_read[4];
		set_AI( UP_ID_AI , value);
		//std::cout<<"UP_ID_AI :"<<value<<std::endl;

		value = ch_read[5];
		value = value<<8;
		value += ch_read[6];
		set_AI( UP_CUR_STATION_AI , value);
		//std::cout<<"UP_CUR_STATION_AI 1:"<<value<<std::endl;

		value = ch_read[7];
		value = value<<8;
		value += ch_read[8];
		set_AI( UP_CUR_STATE_AI , value);
		//std::cout<<"UP_CUR_STATE_AI 1:"<<value<<std::endl;

		ch_read[9];
		ch_read[10];

		ch_read[11];
		ch_read[12];

		ch_read[13];
		ch_read[14];

		U8 bit0_7 = ch_read[15];
		U8 bit8_15 = ch_read[16];


		//std::cout<<"bit0_7:"<<int(bit0_7)<<std::endl;
		//std::cout<<"bit8_15:"<<int(bit8_15)<<std::endl;

		if( bit0_7 & ( 1<<FORK_L_BIT ) ){
			set_DI(UP_FORK_L_DI,1);
			//std::cout<<"UP_FORK_L_DI 1"<<std::endl;
		}else{
			set_DI(UP_FORK_L_DI,0);
			//std::cout<<"UP_FORK_L_DI 0"<<std::endl;
		}



		if( bit0_7 & ( 1<<FORK_R_BIT ) ){
			set_DI(UP_FORK_R_DI,1);
			//std::cout<<"UP_FORK_R_DI 1"<<std::endl;
		}else{
			set_DI(UP_FORK_R_DI,0);
			//std::cout<<"UP_FORK_R_DI 0"<<std::endl;
		}
		
		if( bit0_7 & ( 1<<CARGO_BIT ) ){
			set_DI(UP_CARGO_DI,1);
			//std::cout<<"UP_CARGO_DI 1"<<std::endl;
		}else{
			set_DI(UP_CARGO_DI,0);
			//std::cout<<"UP_CARGO_DI 0"<<std::endl;
		}
		if( bit0_7 & ( 1<<SET_CURRENT_STA_ID_BIT) ){
			set_DI(UP_CURRENT_STA_DI,1);
		}else{
			set_DI(UP_CURRENT_STA_DI,0);
		}
		
		SLEEP(1);
		//std::cout<<"siemens PLC get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
		return true;
	}
	std::cout<<"siemens PLC rtu get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
	return false;
}
#else
bool Plc_siemens::get_PLC_rtu()
{
	boost::mutex::scoped_lock lock(mu_read_write_);
	unsigned char ch_send[LEN_MAX] = {0};
	//send    02 03 00 00 00 06 C5 FB 
	//receive 02 03 0C 00 00 00 BC 06 00 00 00 00 00 00 00 30 9C 
	ch_send[0] = 0x02;
	ch_send[1] = 0x03;
	ch_send[2] = 0x00;
	ch_send[3] = 0x00;
	ch_send[4] = 0x00;
	ch_send[5] = 0x06;
	ch_send[6] = 0xa5;
	ch_send[7] = 0x5a;
	U16 crc = cComm::CRC16( ch_send , 6 );
	ch_send[6] = (crc & 0x00ff)>>0;
	ch_send[7] = (crc & 0xff00)>>8;

	SendData(ch_send,8);

	unsigned char ch_read[100];
	int len = 0;
	ReadData(ch_read,len,17,1000);

	if ( ( len == 17 ) && ( 0x02 == ch_read[0] )  && ( cComm::Check_CRC16(ch_read,len) ))
	{

		S32 value = 0;
		value = ch_read[3];
		value = value<<8;
		value += ch_read[4];
		set_AI( 0 , value);

		value = ch_read[5];
		value = value<<8;
		value += ch_read[6];
		set_AI( 1 , value);


		value = ch_read[11];
		value = value<<8;
		value += ch_read[12];
		set_AI( 2 , value);

		value = ch_read[13];
		value = value<<8;
		value += ch_read[14];
		set_AI( 3 , value);

		int di_index = 0;

		U8 bvalue = ch_read[7];
		for ( int i = 0 ; i < 8 ; ++i)
		{
			if ( bvalue & (1<<i) )
			{
				set_DI(di_index++,0xff);
			}else{
				set_DI(di_index++,0x00);
			}
		}

		bvalue = ch_read[8];
		for ( int i = 0 ; i < 8 ; ++i)
		{
			if ( bvalue & (1<<i) )
			{
				set_DI(di_index++,0xff);
			}else{
				set_DI(di_index++,0x00);
			}
		}
		SLEEP(1);
		//std::cout<<"siemens PLC get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
		return true;
	}
	std::cout<<"siemens PLC rtu get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
	return false;
}
#endif

bool Plc_siemens::get_PLC_tcp()
{
	boost::mutex::scoped_lock lock(mu_read_write_);
	unsigned char ch_send[LEN_MAX] = {0};
	//send    00 00 00 00 00 06 02 03 00 00 
	//receive 02 03 0C 00 00 00 BC 06 00 00 00 00 00 00 00 30 9C 

	memcpy(ch_send,(U8*)&modbus_tcp_head_,7);
	//len
	ch_send[4] = 0;
	ch_send[5] = 0;
	//cmd
	ch_send[7] = 3;
	//add
	ch_send[8] = 0;
	ch_send[9] = 0;

	SendData(ch_send,10);

	unsigned char ch_read[100];
	int len = 0;
	ReadData(ch_read,len,17,1000);

	if ( ( len == 17 ) && ( 0x02 == ch_read[0] ) )
	{

		S32 value = 0;
		value = ch_read[3];
		value = value<<8;
		value += ch_read[4];
		set_AI( 0 , value);

		value = ch_read[5];
		value = value<<8;
		value += ch_read[6];
		set_AI( 1 , value);


		value = ch_read[11];
		value = value<<8;
		value += ch_read[12];
		set_AI( 2 , value);

		value = ch_read[13];
		value = value<<8;
		value += ch_read[14];
		set_AI( 3 , value);

		int di_index = 0;

		U8 bvalue = ch_read[7];
		for ( int i = 0 ; i < 8 ; ++i)
		{
			if ( bvalue & (1<<i) )
			{
				set_DI(di_index++,0xff);
			}else{
				set_DI(di_index++,0x00);
			}
		}

		bvalue = ch_read[8];
		for ( int i = 0 ; i < 8 ; ++i)
		{
			if ( bvalue & (1<<i) )
			{
				set_DI(di_index++,0xff);
			}else{
				set_DI(di_index++,0x00);
			}
		}
		SLEEP(1);
		//std::cout<<"siemens PLC get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
		return true;
	}
	std::cout<<"siemens PLC tcp get AI reg communication err! len:"<<len<<" str:"<<cComm::ByteToHexString(ch_read,len)<<std::endl;
	return false;
}


