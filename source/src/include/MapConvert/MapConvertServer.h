#ifndef _MAP_CONVERT_SERVER_
#define _MAP_CONVERT_SERVER_

#include "RobotStruct.h"
#include <string>

class Map_Server;
class MapConvertServer{
public:
	MapConvertServer();
	~MapConvertServer();
public:
	void init(Map_Server* map_server, SGridXY left_corner_grid_point, int use_range_width = 700, int use_range_height = 700);
	bool convertedMapToWorld(int converted_x,  int converted_y, double& world_x, double& world_y);
	bool worldToConvertedMap(double world_x, double world_y, int& converted_x, int& converted_y);

	bool originalMapToWorld(int original_x,  int original_y, double& world_x, double& world_y);
	bool worldToOriginalMap(double world_x, double world_y, int& original_x, int& original_y);

	int convertedMapToConvertedIndex(int converted_x,  int converted_y);
	int originalMapToOrigianlIndex(int original_x,  int original_y);

	int convertedMapToOriginalIndex(int converted_x,  int converted_y);
	int originalMapToConvertedIndex(int original_x,  int original_y);

	bool convertedIndexToConvertedMap(int index,int& converted_x, int& converted_y);
	bool originalIndexToOriginalMap(int index, int&original_x, int& original_y);

	unsigned char* getConveredGlobalMapData();
	signed char getOriginalCost(int original_x,int original_y,std::string layer,std::string target_map);
	signed char getConvertedCost(int converted_x,int converted_y,std::string layer,std::string target_map);

	int getConvertedMapWidth();
	int getConvertedMapHeight();
	int getOriginalMapWidth();
	int getOriginalMapHeight();

	void saveMapForView(unsigned char* data);
	bool convertedMapToOriginalMap(int converted_x,  int converted_y, int& original_x, int& original_y);
	bool originalMapToConvertedMap(int original_x,  int original_y, int& converted_x, int& converted_y);
private:
	Map_Server* map_server_;
	int use_range_width_;
	int use_range_height_;
	int original_map_width_;
	int original_map_height_;
	SGridXY left_corner_grid_point_;
	unsigned char* convertedMapData;
};
#endif