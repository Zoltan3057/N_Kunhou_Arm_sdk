#ifndef _PLC_BASE_WANGHONGTAO_2016_11_19_
#define _PLC_BASE_WANGHONGTAO_2016_11_19_


#include "MyDefine.h"
#include <boost/thread.hpp>

#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

class cTransferDevice;

typedef struct{
	U8 t1_;
	U8 t2_;
	U8 p1_;
	U8 p2_;
	U8 len0_;
	U8 len1_;
	U8 add_;
	U8 unused_;
}S_modbus_tcp_head;

class PLC_base
{
public:

	PLC_base();
	~PLC_base();

	bool init( std::string para );
	
	
	bool open_transfer_device();
	void close_transfer_device();

	void get_DI(const U32 &id,S8 &value);
	void get_AI(const U32 &id,S32 &value);

	void get_DI_AI(SDIAI &value);

	void set_DO(const U32 &id,const S8 &value);
	void set_AO(const U32 &id,const S32 &value);


	virtual bool set_PLC() = 0;
	virtual bool get_PLC() = 0;

protected:

	void init();

	virtual void setPara(std::string name , std::string value) = 0;

private:
	
	//
	
	
	

protected://

	S_modbus_tcp_head modbus_tcp_head_;

	bool network_port(std::string ip);

	virtual void SendData( U8* s_data,U16 len );
	virtual void ReadData( U8* r_data,int &len,int need,int timeout );

	std::string port_;
	cTransferDevice* pTransDevice_;

	boost::mutex mu_read_write_;
	
	SDIAI di_ai_;
	SDOAO do_ao_;

	void set_DI(const U32 &id,const S8 &value);
	void set_AI(const U32 &id,const S32 &value);

	void get_DO(const U32 &id,S8 &value);
	void get_AO(const U32 &id,S32 &value);

};

#endif//_PLC_BASE_WANGHONGTAO_2016_11_19_
