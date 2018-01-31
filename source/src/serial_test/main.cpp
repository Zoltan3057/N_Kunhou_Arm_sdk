
#include "TransferDevice.h"
#include "Comm.h"
#include "Log.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{

	std::string strPort = "ttyS0";
	std::cout<<"please input port: (def ttyS0)";
	std::cin>>strPort;
	std::cout<<"serial port :"<<strPort<<std::endl;

	int ibaud = 9600;
	std::cout<<"please input baud:";
	std::cin>>ibaud;
	std::cout<<"serial port baud:"<<ibaud<<std::endl;



	cConnPara conn_para2;
	conn_para2.m_iType = cConnPara::SERIALPORT;
	conn_para2.setDeviceName(strPort);
	conn_para2.nBaud = ibaud;
	conn_para2.chParity = 'N';
	conn_para2.nDataBits = 8;
	conn_para2.nStopBits = 1;
	conn_para2.b_rs485 = true;

	cTransferDevice* pSerialPort = cTransferDevice::GetInstance(conn_para2);

	if (pSerialPort)
	{
		while(!pSerialPort->m_Status.IsOpen()){
			 SLEEP(100);
			 std::cout<<"port open err!"<<std::endl;
		}
	}

	while(pSerialPort->m_Status.IsOpen()){

		U8 send[26] ;
		U32 slen = 26;
		for (int i = 0 ; i < slen ; ++i)
		{
			send[i] = i ;
		}
		pSerialPort->WriteData(send,slen);
		std::cout<<"send 26:"<<cComm::ByteToHexString(send,slen)<<std::endl;

		U8 rec[100] = {0};
		int rlen = 0;
		pSerialPort->ReadData(rec,rlen,26,1000);

		if (rlen > 0)
		{
			std::cout<<"rec:"<<cComm::ByteToHexString(rec,rlen)<<std::endl;

		}else{
			//SLEEP(500);
			std::cout<<"no data read!"<<std::endl;
			continue;
		}



	}
}
