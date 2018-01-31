#ifndef _SINGLETON_WANGHONGTAO_2016_11_19_
#define _SINGLETON_WANGHONGTAO_2016_11_19_

#include "MyDefine.h"
#include <list>
#include <boost/thread.hpp>

#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

#include "control_data.h"

class singleton_ctl
{
public:
	
	singleton_ctl();
	~singleton_ctl();

	void get_in_data(Control_data_read &status);


public:

//plc read
public:
	void set_amcl_pos(const SPos &amcl_pos);



//plc write
public:
	void set_diai(const SDIAI &diai);
	void set_function(const U16 &function );
	void set_status(const SRunStatus& sta);  // run stop obstacle
	void set_obstacle(const std::string str);
private:
	void set_sequence();

	
private:
	boost::mutex mu_in_;
	boost::mutex mu_out_;
	Control_data_read in_status;
	Control_data_write out_status;
	
protected:

private:
	//SDOAO doao_;
	void setAO(U32 id,const S32& value);
	void setDO(U32 id,const U8& value);
	SDO do_;
	SAO ao_;
	void create_pub_package();

private:

	typedef enum{
		RUN = 0,
		STOP = 1,
		ENERGENCY = 2,
		ERR = 3
	}Run_Status;

	Run_Status current_status_;
	
	bool low_power_;
	U16 get_current_station_;
	U16 get_target_station_;

	U16 plc_sequence_;
	U32 alive_count_;
	bool plc_alive_;

	//boost::mutex mu_status_;
	SRunStatus planner_status_;

};


#endif//_SINGLETON_WANGHONGTAO_2016_11_19_
