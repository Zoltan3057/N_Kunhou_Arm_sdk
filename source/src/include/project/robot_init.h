
#ifndef _ROBOT_INIT_H_
#define _ROBOT_INIT_H_

#include <vector>
#include "RobotStruct.h"
#include "control_data.h"
#include "project/node_init_pos.h"

class robot_init{

public:

	robot_init(){};
	~robot_init(){};

	bool init();
	void init_pos(const Control_data_read &status);

private:
	void pubilsh_init_pos(const F32 &ex,const F32 &ey,const F32 &ea);
	void publish_clear_current_sta_id();

	Manage<Node_init_pos> Node_init_pos_list_;
};


#endif // _ROBOT_INIT_H_
