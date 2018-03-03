
#include <iostream>

#include "Comm.h"
#include "jsoncpp/json.h"
#include "TransferDevice.h"

#include "webinterface/NavikitInterface.h"

HANDLE   handle = NULL;
cTransferDevice* pTrans = 0;

#define MAX_BUF 409600

std::string str_total = "";
cConnPara conn_para;
//ByteList list_total;
unsigned char uch_total[MAX_BUF];
//int len_total = 0;

void clear_buf(){
//	list_total.Clear();
	memset(uch_total,0,MAX_BUF);
//	len_total = 0;
	str_total = "";
}
std::string check_json_over(){



	int left_size = cComm::PatternCount(str_total,"{");

	int right_size = cComm::PatternCount(str_total,"}");

	if( ( left_size > 0 ) || ( right_size > 0 ))
	{
		std::cout<<"left_size:"<<left_size<<" right_size:"<<right_size<<std::endl;
		std::cout<<"total string:"<<str_total<<std::endl;
	}
	//

	if( (left_size > 0) || (right_size > 0)){
		if(left_size == right_size){
			return str_total;
			
		}else{
			return "";
		}

	}else{
		return "";
	}

}
std::string transfer_data(std::string command){

	std::cout<<"command:"<<command<<std::endl;
	std::string  result;

	int state = NavikitInterfaceCall(&handle,
		                             command,
									 result);


	return result;
}

bool network_port(std::string ip){
	std::vector<std::string> vstr;
	cComm::SplitString(ip,".",vstr);
	if (vstr.size()>3)
	{
		return true;
	}else{
		return false;
	}

}

bool init_trans(std::string str_init_name){
	//std::ifstream iff("appNavikitInterface.ini");
	str_init_name += ".ini";
	std::cout<<"init appNavikitinterface ini:"<<str_init_name<<std::endl;
	std::ifstream iff(str_init_name);
	std::string str;
	//std::string  strIP = "192.168.1.194";
	std::string  strIP = "COM1";
	int iPort(7700);

	if(iff.is_open()){

		std::getline(iff,str);

		if(str.size() > 0){
			strIP = str;
		}

		std::getline(iff,str);
		if(str.size() > 0){
			iPort = atoi(str.c_str());
		}
	}



//	list_total.Init(MAX_BUF);



	if(network_port(strIP)){

		std::cout<<"strIP:"<<strIP<<" iPort:"<<iPort<<std::endl;

		conn_para.m_iType = cConnPara::NETWORK;

		conn_para.m_nNetType = cConnPara::TCPSERVER;
		conn_para.n_listen_port = iPort;//
	//	conn_para.m_nNetType = cConnPara::TCPCLIENT;
	//	conn_para.m_nRemoteIP = strIP;
	//	conn_para.m_nRemotePort = iPort;//



	}else{

		std::cout<<"Serial Port:"<<strIP<<std::endl;

		conn_para.m_iType = cConnPara::SERIALPORT ;
		conn_para.setDeviceName(strIP);
		conn_para.nBaud = 115200;
		conn_para.chParity = 'N';
		conn_para.nDataBits = 8;
		conn_para.nStopBits = 1;
		conn_para.b_rs485 = true;

	}


	pTrans = cTransferDevice::GetInstance(conn_para);

	if ( !pTrans)
	{
		return false;
	}

	return true;

}


bool getTCPData(std::string &res){

	res = "";

	if( !pTrans->m_Status.IsOpen() ){
		std::cout<<"err TCP server!"<<std::endl;
		return false;
	}

	clear_buf();

//	unsigned char* uch_data = new unsigned char[1024];
//	memset(uch_data,0,1024);

	int len_data = 0;

	std::string str_send = "";

	int count = 50;

	while( count > 0 ){

		len_data = 0;

		pTrans->ReadData(uch_total,len_data,100,20);
		if(len_data != 0){
			std::cout<<"rec data:"<<cComm::ByteToHexString(uch_total,len_data)<<std::endl;
		}
		std::string tmp = (char*)uch_total;

		str_total += tmp;

		memset(uch_total,0,MAX_BUF);

		str_send = check_json_over();

		if(str_send.size() > 0){
			std::cout<<"clear buf!!!"<<std::endl;
			clear_buf();
			res = str_send;
			return true;
		}else{
			count--;
		}
		//if(str_total.size()){

		//	std::cout<<"get data count:"<<count<<" str_total:"<<str_total.size()<<std::endl;
			
		//}
		if( !pTrans->m_Status.IsOpen() ){
			std::cout<<"get tcp data err!"<<std::endl;
			SLEEP(500);
			count--;
		}

	}

	clear_buf();
	std::cout<<"time out !!!"<<std::endl;
	return false;

}

void sentTCPData(std::string str_res){
	unsigned char* uch_res = (unsigned char*)str_res.c_str();
	int len_res = str_res.length();

	if(len_res < 1){
		return;
	}

	if( !pTrans->m_Status.IsOpen() ){
		std::cout<<"err TCP server!"<<std::endl;
		return ;
	}

	pTrans->WriteData(uch_res,len_res);
	std::cout<<"str_res:"<<str_res<<std::endl;

}
int main(int argc, char *argv[])
{
	char    *host   = "127.0.0.1";

	std::string init_name = argv[0];

	init_trans(init_name);
	//
	NavikitInterfaceCreate( host, 
		                   &handle);
	if (NULL == handle)
	{
		printf("Create Navikit Interface Handle Failed!\n");
		return 0;
	}

	int i_max_err = 0;
	if( argc > 1 ){
		std::string para = argv[1];
		cComm::ConvertToNum(i_max_err,para);
		std::cout<<"max try err count:"<<i_max_err<<std::endl;
	}

	int i_err_count = 0;
	bool b_run = true;

	while(1){


// 		if(!pTrans->m_Status.IsOpen()){
// 			
// 			if( ( i_max_err != 0 ) && ( i_err_count++ > i_max_err) )
// 			{
// 				std::cout<<"long time no data receive!"<<std::endl;
// 				b_run = false;
// 			}
// 			std::cout<<"recreate transfer"<<std::endl;
// 			SLEEP(500);
// 			continue;
// 		}
		if(  !b_run ){
			std::cout<<"close transer device!"<<std::endl;
			cTransferDevice::DelInstance(conn_para);
			SDelete(pTrans);

			SLEEP(3000);
			std::cout<<"close tranfer device over!"<<std::endl;

			init_trans(init_name);

			b_run = true;
			i_err_count = 0;
		}

		std::string str_send;
		if( !getTCPData(str_send) ){
			
			std::cout<<"no data receive:"<<i_err_count<<std::endl;
			
			if( ( i_max_err != 0 ) && ( i_err_count++ > i_max_err) )
			{
				std::cout<<"long time no data receive!"<<std::endl;
				b_run = false;
			}
			SLEEP(100);
			continue;
		}else{
			i_err_count = 0;
		}
		

		std::string str_res = transfer_data(str_send);
		//std::string str_res = "res ok";

		sentTCPData(str_res);

		std::cout<<"get data:"<<str_res<<std::endl;
	}

	NavikitInterfaceRelease(&handle);

	return 0;
}
