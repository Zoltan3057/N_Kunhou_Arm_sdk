#ifndef _CAN_SOCKET_WANGHONGTAO_2017_06_23_H_
#define _CAN_SOCKET_WANGHONGTAO_2017_06_23_H_

#include <linux/can.h>
#include <signal.h>
#include <aio.h>

#include "buffer_con.hpp"

class can_socket{

public:
	can_socket();
	~can_socket();

	static void init();

	//bool WriteData(struct can_frame frm_rec_);
	//int  ReadData(/*Out*/unsigned char* pReceiveData,int& iLen,/*In*/int iReclen,int iTimeout);
	static bool ReadData(std::vector<can_frame> &frame_list);
	static bool WriteData(can_frame frame);

private:

	static int sock_;

	static struct aiocb m_aiocb_;
	static struct aiocb m_aiocb_wr_;

	//struct can_frame frm_rec_;
	static U8* data_rec_;
	static U8* data_send_;
	static CSBuffer<can_frame,100> can_list_;
	static void read_callback(sigval_t sigval);
};

#endif//_CAN_SOCKET_WANGHONGTAO_2017_06_23_H_
