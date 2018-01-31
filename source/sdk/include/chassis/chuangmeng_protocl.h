#ifndef _CHASSIS_CHUANGMENG_PROTOCL_WANGHONGTAO_2017_3_11_
#define _CHASSIS_CHUANGMENG_PROTOCL_WANGHONGTAO_2017_3_11_


#include "chassis/driver_base.h"

class cTransferDevice;

typedef struct{
	bool st_;
	bool ccw_;
	bool enable_;
	bool back_;
}SSPEED_BIT;

class chuangmeng_protocl
{
public:
	chuangmeng_protocl();
	~chuangmeng_protocl();

	//get cmd from v_set_rpm
	void setRPM(std::map<int,F32> &m_set_rpm);
	void getSpeedCmd(U8* data_cmd , U32& len);
	void getEnCwCmd(U8* data_cmd , U32& len);

	void getGet_status(U8* data_cmd , U32& len , const U8 &id);
	void getClear_fault(U8* data_cmd , U32& len , const U8 &id);

	void getPos(U8* data_cmd , U32& len ,const U8 &id);
	void getRPM( U8* data_cmd , U32& len ,const U8 &id );
	bool prase_pos(S16 &pos, U8* data_cmd , S32& len ,const U8 &id );
	bool prase_rpm(S16 &rpm, U8* data_cmd , S32& len ,const U8 &id );
	bool getDriection( int &dir, const U8 &id );

private:

	void create_st_cw_bit(std::map<int,F32> &m_set_rpm);
	std::map<int,SSPEED_BIT> m_speed_bit_;


	U8 ch_speed[17];  //00 10 3F 08 00 02 04 01 F4 00 FA 00 FA 00 FA 7A 20
	U32 speed_len_;

	U8 ch_odom[8];     //01 03 46 12 00 01 31 47
	U8 ch_rpm[8];      //01 03 00 02 00 01 25 CA 

	U8 ch_get_status[8];    //01 03 00 03 00 01 74 0A
	U8 ch_clear_fault[8];   //01 06 0A 00 00 01 4B D2

private:
	void init();
};



#endif//_CHASSIS_CHUANGMENG_PROTOCL_WANGHONGTAO_2017_3_11_
