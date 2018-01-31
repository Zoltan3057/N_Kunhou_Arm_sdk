
#include <iostream>

#include "Comm.h"
#include "jsoncpp/json.h"
#include "TransferDevice.h"
#include "Geometry.h"
#include "webinterface/NavikitInterface.h"

HANDLE   handle = NULL;
cTransferDevice* pTCP = 0;

#define MAX_BUF 4096

ByteList list_total;
unsigned char uch_total[MAX_BUF];
int len_total = 0;

void clear_buf(){
	list_total.Clear();
	memset(uch_total,0,MAX_BUF);
	len_total = 0;
}
std::string check_json_over(){

	list_total.Read(uch_total,len_total);
	std::string str_res = (char*)uch_total;

	int left_size = cComm::PatternCount(str_res,"{");
	int right_size = cComm::PatternCount(str_res,"}");

	std::cout<<"left_size:"<<left_size<<" right_size:"<<right_size<<std::endl;

	if(left_size == right_size){
		clear_buf();
		return str_res;
	}else{
		return "";
	}

}

std::string getTCPData(){

	if( !pTCP->m_Status.IsOpen() ){
		std::cout<<"err TCP server!"<<std::endl;
		return "";
	}

	clear_buf();

	unsigned char* uch_data = new unsigned char[1024];
	memset(uch_data,0,1024);

	int len_data = 0;


	std::string str_send = "";

	int count = 200;

	while(count){

		pTCP->ReadData(uch_data,len_data,100,20);

		list_total.Write(uch_data,len_data);

		str_send = check_json_over();

		if(str_send.size() > 0){
			return str_send;
			break;
		}
//		if(len_total){
//			count--;
//		}
		count--;
		std::cout<<"get data count:"<<count<<" len_total:"<<len_total<<std::endl;
	}

	clear_buf();
	std::cout<<"time out !!!"<<std::endl;
	return "";

}

void sentTCPData(std::string str_res){
	unsigned char* uch_res = (unsigned char*)str_res.c_str();
	int len_res = str_res.length();

	if(len_res < 1){
		return;
	}

	if( !pTCP->m_Status.IsOpen() ){
		std::cout<<"err TCP server!"<<std::endl;
		return ;
	}

	pTCP->WriteData(uch_res,len_res);
	std::cout<<"str_res:"<<str_res<<std::endl;

}
std::string transfer_data(std::string command){


	std::string  result;

	sentTCPData(command);
	result = getTCPData();

	return result;
}

std::string str_setRobotPos(float current_pos_x,float current_pos_y,float target_pos_x,float target_pos_y){

	Json::Value value_pos;

	value_pos["robot_x"] = current_pos_x;
	value_pos["robot_y"] = current_pos_y;
	value_pos["end_x"] = target_pos_x;
	value_pos["end_y"] = target_pos_y;

	Json::Value value_tpye;
	value_tpye["name"] = "setRobotPos";
	value_tpye["data"] = value_pos;

	Json::Value value_function;
	value_function["CallBackFunction"] = value_tpye;

	Json::Value value_action;
	value_action["Action"] = value_function;

	Json::StyledWriter writer;
	std::string out = writer.write(value_action);

	std::cout<<out<<std::endl;


	return out;

}

std::string setRobotPos(float current_pos_x,float current_pos_y,float target_pos_x,float target_pos_y){

	std::string send = str_setRobotPos(current_pos_x,current_pos_y,target_pos_x,target_pos_y);

	std::string res = transfer_data(send);

	return res;

}

std::string str_getRobotPos(){

	Json::Value value_tpye;
	value_tpye["name"] = "getRobotPos";


	Json::Value value_function;
	value_function["CallBackFunction"] = value_tpye;

	Json::Value value_action;
	value_action["Action"] = value_function;

	Json::StyledWriter writer;
	std::string out = writer.write(value_action);

	std::cout<<out<<std::endl;
	return out;
}

std::string getRobotPos(float &pos_x,float &pos_y,float &pos_th){

	std::string send = str_getRobotPos();
	std::string res = transfer_data(send);

	Json::Reader js_reader;
	Json::Value  js_in_root;


	if ( js_reader.parse(res.c_str(), js_in_root) )
	{
		if(!js_in_root["data"].isNull()){

			Json::Value  js_data = js_in_root["data"];

			if(!js_data["laser_dx"].isNull()){
				float laser_dx = js_data["laser_dx"].asFloat();
			}
			if(!js_data["x"].isNull()){
				pos_x = js_data["x"].asFloat();
			}
			if(!js_data["y"].isNull()){
				pos_y = js_data["y"].asFloat();
			}
			if(!js_data["th"].isNull()){
				pos_th = js_data["th"].asFloat();
			}
		}
	}

	std::cout<<"get robot pos res:"<<res<<std::endl;

	return res;
}
bool init_trans(){

	std::ifstream iff("appNavikitInterface.ini");

	std::string str;
	std::string  strIP = "192.168.1.194";
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

	std::cout<<"strIP:"<<strIP<<" iPort:"<<iPort<<std::endl;

	list_total.Init(MAX_BUF);

	cConnPara conn_para;
	conn_para.m_iType = cConnPara::NETWORK;
//	conn_para.m_nNetType = cConnPara::TCPSERVER;
//	conn_para.n_listen_port = iPort;//
	conn_para.m_nNetType = cConnPara::TCPCLIENT;
	conn_para.m_nRemoteIP = strIP;
	conn_para.m_nRemotePort = iPort;//


	pTCP = cTransferDevice::GetInstance(conn_para);

	if ( !pTCP)
	{
		return false;
	}

	int begin_count = 100;

	while(begin_count--){

		if( pTCP->m_Status.IsOpen() ){
			std::cout<<"tranferdevice open ok!"<<std::endl;
			return true;
			break;
		}
		SLEEP(500);
		std::cout<<"wait for tranferdevice open !"<<std::endl;
	}

	return false;

}
void init_pos(F32 robot_x, F32 robot_y , F32 robot_th){
	double x1, y1, x2, y2;

	x1 = robot_x;
	y1 = robot_y;

	VecPosition vp(x1,y1);
	VecPosition vt(1.0,robot_th,POLAR);
	vt = vp + vt;
	x2 = vt.getX();
	y2 = vt.getY();

	setRobotPos(x1,y1,x2,y2);
}

int main(int argc, char *argv[])
{
	char    *host   = "127.0.0.1";

	init_trans();

	//
	NavikitInterfaceCreate( host, 
		                   &handle);
	if (NULL == handle)
	{
		printf("Create Navikit Interface Handle Failed!\n");
		return 0;
	}

	F32 fx = 0;
	F32 fy = 0;
	F32 fa = 0;

	std::vector<std::string> vstr;
	std::string str_init_pos = "1.0;-2.1;90";
	Config::getConfig("init_pos_xya",str_init_pos);
	cComm::SplitString(str_init_pos,";",vstr);
	if( vstr.size() > 2 ){
		cComm::ConvertToNum( fx, vstr[0] );
		cComm::ConvertToNum( fy, vstr[1] );
		cComm::ConvertToNum( fa, vstr[2] );
	}

	//
	SLEEP(1000);
	init_pos(fx,fy,fa);
	SLEEP(1000);

	while(1){

		float pos_x = 0;
		float pos_y = 0;
		float pos_th = 0;

		getRobotPos(pos_x,pos_y,pos_th);
		std::cout<<"pos_x:"<<pos_x<<" pos_y:"<<pos_y<<" pos_th:"<<pos_th<<std::endl;
		SLEEP(500);
		break;

	}
	//
	NavikitInterfaceRelease(&handle);
	std::cout<<"init robot pos over!!! x:"<<fx<<" y:"<<fy<<" th:"<<fa<<std::endl;
	return 0;
}
