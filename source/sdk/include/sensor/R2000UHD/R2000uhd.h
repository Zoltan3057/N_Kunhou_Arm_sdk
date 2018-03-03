/*
 * R2000uhd.cpp
 *
 *  Created on: Jan 19, 2018
 *      Author: dl
 */

#ifndef R2000UHD_H_
#define R2000UHD_H_

#include <string>

#include "MyDefine.h"
#include "R2000UHD/DataConvert.h"
#include "R2000UHD/SendCommand.h"


class cTransferDevice;

typedef struct _scanData1 {

//	U32 h_distance[720];
	F32 h_distance[720];
	U32 h_echo[720];
    double h_angle[720];

} scanData1;


typedef struct _headData{
    U8 magic[2];
    U16 packet_type;
    U32 packet_size;
    U16 header_size;
    U16 scan_number;
    U16 packet_number;
    U64 timestamp_raw;
    U64 timestamp_sync;
    U32 status_flags;
    U32 scan_frequency;
    U16 num_points_scan;
    U16 num_points_packet;
    U16 first_index;
    S32 first_angle;
    S32 angular_increment;
    double angular_increment_real;
    U32 output_status;
    U32 field_status;
}headData;



class R2000uhd {
public:
	R2000uhd();
	virtual ~R2000uhd();


	void connect(std::string host, int port = 2111);


	void disconnect();


	bool isConnected();

	void Laser_Init(const std::string ip);

	void startMeas(const std::string ip);
	void Feed_Watchdog();

	void stopMeas();

	bool getData(scanData1& data);
	//add by wht for get float dist
	void getLdata(float* ldata,scanData1& data);
	bool isReadStatus();

	headData m_headData;

private:
	bool connected;
	bool debug;
	int sockDesc;

	DataConvert m_DataConvert;
	//scanData m_scanData;
//	cTransferDevice* pTransDevice;
	SendCommand m_sendmand;
	//U8 buffer[2048];
//	U8* buffer1;
	S16 m_count;
	S16 m_feedCount,m_ReadCount;
	bool IsScanRead;
	bool IsReadDate;
	std::string  laser_ip_;

};

#endif    /* R2000UHD_H_ */

