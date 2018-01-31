#ifndef _PLC_SIEMENS_WANGHONGTAO_2016_11_19_
#define _PLC_SIEMENS_WANGHONGTAO_2016_11_19_

#include "plc_base.h"

class Plc_siemens : PLC_base
{
public:

	Plc_siemens();
	~Plc_siemens();

private://virtual
	//
	void setPara(std::string name , std::string value);

	bool set_PLC();
	bool get_PLC();

	bool get_PLC_rtu();

	bool get_PLC_tcp();

private:
	U32 do_get_io_;
	U32 do_set_io_;
	bool get_PLC_rtu_io();
	bool set_PLC_io();
};

#endif//_PLC_SIEMENS_WANGHONGTAO_2016_11_19_
