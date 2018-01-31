#ifndef _FILTER_TEMPLATE_WANGHONGTAO_20150824_
#define _FILTER_TEMPLATE_WANGHONGTAO_20150824_

#include <vector>
#include <map>
#include "MyDefine.h"
#include "map_def.h"
#include "RobotStruct.h"


// #define FLOAT_MAP F32
// #define INT_MAP U8

class filter_template
{
public:
	filter_template();
	~filter_template();

	void init(F32 sigma,F32 resolution  = 0.05,U8 max_value = MAP_OCCUPY_MAX);
	void cut(int top , int half_width);

	U8 operator[](int index);
	U8 operator()(int index);

	int filter_size();

	U8 max_value_;

protected:
private:
	bool get_width(int &top,int &half_width);

	static const int FILTER_CNT = 255;
	F32 filter[FILTER_CNT];
	U8 ifilter[FILTER_CNT];

	U8 use_filter[FILTER_CNT];
	int size_;

	
};
class Node_2D
{
public:
explicit Node_2D(S16 x,S16 y,S8 v){
		gx = x;
		gy = y;
		value = v;
	};

	~Node_2D(){};

	S16 gx;
	S16 gy;

	S8 value;

protected:
private:
};
class filter2D_template
{
public:
	filter2D_template():data_(0),dsize_(0),max_value(0){};
	~filter2D_template(){};

	void init(F32 sigma,F32 resolution,U8 max);
	void init_notching( F32 sigma,F32 resolution,U8 max);
	//std::map<int,std::vector<Node_2D> > m_list;
	std::vector< std::vector<Node_2D> > m_list;
	U8 max_value;
	


protected:
private:
	
	U8* data_;
	U16 dsize_;
	
};

class Inflation
{
public:

	Inflation();
	~Inflation();

	void init(F32 sigma,F32 resolution  = 0.05,U8 max_value = MAP_OCCUPY_MAX);
	bool cols_filter(unsigned char* des , unsigned char* src ,int rows ,int cols);
	bool rows_filter(unsigned char* des , unsigned char* src ,int rows ,int cols);

	U32 filter_size(){
		return ft_.filter_size();
	};

protected:
private:
	S32 U8_2_S32(U8* p8);
	void S32_2_U8(U8* p8,S32 data);
	filter_template ft_;
	
};

#endif//_FILTER_TEMPLATE_WANGHONGTAO_20150824_
