#ifndef _MAP_DEF_WANGHONGTAO_20151105_
#define _MAP_DEF_WANGHONGTAO_20151105_


#define AMCL_MAP "amcl_map"
#define GLOBAL_INF_MAP		 "global_inf_map"
//#define GLOBAL_COST_MAP	 "global_cost_map"
#define LASER_INF_MAP		 "laser_inf_map"
//#define LOCAL_COST_MAP		 "local_cost_map"
#define LOCAL_PATH_MAP		 "local_path_map"
#define GOAL_MAP			 "goal_map"
#define PROHIBIT_MAP		 "prohibit_map"
#define PLANNER_MAP			 "planner_map"

#define ODOM_LAYER			 "odom_layer"
#define WORLD_LAYER			 "world_layer"


#define GLOBAL_MAP_WIDTH 8192
#define GLOBAL_MAP_HEIGHT 8192

#define LOCAL_MAP_WIDTH 256
#define LOCAL_MAP_HEIGHT 256

#define GLOBAL_MAP_SIZE (GLOBAL_MAP_WIDTH*GLOBAL_MAP_HEIGHT)
#define LOCAL_MAP_SIZE  (LOCAL_MAP_WIDTH*LOCAL_MAP_HEIGHT)

//bind define should change both
#define MAP_RESOLUTION 0.05
#define F32_2_S16 20
//bind define should change both

#define GLOBAL_ORI_X (-MAP_RESOLUTION*GLOBAL_MAP_WIDTH/2)
#define GLOBAL_ORI_Y (-MAP_RESOLUTION*GLOBAL_MAP_HEIGHT/2)

#define LOCAL_ORI_X (-MAP_RESOLUTION*LOCAL_MAP_WIDTH/2)
#define LOCAL_ORI_Y (-MAP_RESOLUTION*LOCAL_MAP_HEIGHT/2)

#define MAP_OCCUPY_MAX 127
#define MAP_EMPTY		 0
#define MAP_UNKNOWN		-1
#define MAP_ERR			-128

#endif//_MAP_DEF_WANGHONGTAO_20151105_
