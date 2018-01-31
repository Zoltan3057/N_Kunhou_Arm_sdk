#ifndef _KEYPRESS_KYOSHO_20151008_
#define _KEYPRESS_KYOSHO_20151008_

#include "buffer_con.hpp"

class keypress
{
public:
	keypress();
	~keypress();

	bool getkey(char &key_code);
	void init();

protected:
	
private:

	bool th_run_;
	void keypress_thread();
	void close();
	SBuffer<char> keypress_list_;
	char last_key_code_;
	int  continue_key_;
};
#endif//_KEYPRESS_KYOSHO_20151008_
