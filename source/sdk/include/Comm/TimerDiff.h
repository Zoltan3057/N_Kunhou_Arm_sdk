#ifndef _TIMERDIFF_KYOSHO_20110903_
#define _TIMERDIFF_KYOSHO_20110903_


//#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

class cTimerDiff
{

private:
	boost::posix_time::ptime beginTime;
	boost::posix_time::ptime endTime;

	void ms_begin();
	long long ms_end();
	boost::posix_time::ptime ms_beginTime;
	boost::posix_time::ptime ms_endTime;

public:
	cTimerDiff(void);
	~cTimerDiff(void);

	void Begin();
	long long End();

	long long GetTime() const;

	void ms_loop(int ms);


	void PrintString();
public:
	static std::string get_now();
	static long long total_ms();
};

#endif // _TIMERDIFF_KYOSHO_20110903_
