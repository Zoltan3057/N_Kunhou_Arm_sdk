#ifndef __K_LOCAL_PLANER_INTERFACE_HPP__
#define __K_LOCAL_PLANER_INTERFACE_HPP__

#include <queue>
#include <list>
#include <boost/function.hpp>

#include <Pluma/Pluma.hpp>
#include "RobotStruct.h"



typedef std::deque<std::pair<SSpeed,double> > TimestampSpeed;

class KLocalPlanerInterface
{
public:
	KLocalPlanerInterface():is_init_(false){}
	virtual bool makePlan( SPos& amcl_pos,  SOdomSpeed& current_odo, int& cost, TimestampSpeed& vel_cmd) = 0;
	virtual bool init(Map_Server* map_server,boost::function< void (std::list< SGridXY >& path ) > fnc) = 0;
	virtual std::string getName() = 0;

protected:
	bool is_init_;
};

PLUMA_PROVIDER_HEADER(KLocalPlanerInterface);
#endif
