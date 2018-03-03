/*
 * SendCommand.h
 *
 *  Created on: Jan 19, 2018
 *      Author: dl
 */

#ifndef SENDCOMMAND_H_
#define SENDCOMMAND_H_

#include <string>
#include "R2000UHD/CHttpClient.h"
#include "KHTypes.h"
#include "R2000UHD/DataConvert.h"

class SendCommand {
public:
	SendCommand();
	virtual ~SendCommand();

	STRING SendComm(STRING command);
	STRING Request_Handle_TCP(STRING ip, STRING packet_type, STRING timeout, STRING start_angle);
	STRING Release_Handle(STRING ip);
	STRING Start_Scanoutput(STRING ip);
	STRING Stop_Scanoutput(STRING ip);
	STRING Set_Parameter(STRING ip, STRING parameter);
	STRING Get_Parameter_Info(STRING ip);
	STRING set_scanoutput_config(STRING ip,STRING parameter);
	STRING Feed_Watchdog(STRING ip);

public:
	CHttpClient m_httpclient;
	DataConvert m_DataConvert;
	S16 Int16_Sucess;
	S16 Int16_ErrorCode;
	STRING Handle;
	STRING Port;


};

#endif /* SENDCOMMAND_H_ */
