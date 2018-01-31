#ifndef _WANGHONGTAO_REMOTE_2017_04_05_H_
#define _WANGHONGTAO_REMOTE_2017_04_05_H_

#include <boost/thread.hpp>

class cTransferDevice;

typedef struct _SRemote_data{
	
	U8 key1_;
	U8 key2_;
	U8 key3_;
	U16 new_ad1_;
	U16 new_ad2_;
	S32 sequence_;

}SRemote_data;

class remote_ctl{

public:
  remote_ctl();
  ~remote_ctl();

    void init(const std::string port);
	
	void stop();
	
	bool get_data(SRemote_data &data);

private:

	void run();
	bool open_transfer_device();
	void close_driver();

	cTransferDevice* pTransDevice_;
	std::string port_;

	void thread_loop();
	bool thread_run_;

	typedef enum{HEAD,KEY,AD1_0,AD1_1,AD2_0,AD2_1,CRC0,CRC1} State;
	State sta_;

	U8 key_;
	U16 ad_1;
	U16 ad_2;

	void state_mathine(U8 data);

private:
	void setKey();
	void setAd();

	boost::mutex mu_data_;

	SRemote_data data_;
	S32 last_sequence_;
	U8  err_count_;
	bool first_flag_;
};

#endif//_WANGHONGTAO_REMOTE_2017_04_05_H_
