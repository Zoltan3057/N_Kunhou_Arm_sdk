#ifndef _MY_TF_KYOSHO_20150906_
#define _MY_TF_KYOSHO_20150906_

#include <map>
#include <string>

#include <boost/serialization/singleton.hpp>
#include "robot/Geometry.h"
#include "MyDefine.h"
#include "RobotStruct.h"


class tf_2d
{
public:
	tf_2d(){};
	~tf_2d(){};

	std::string parent_;	//where layer_diff_'s original 
	std::string child_;    //

	SPos pos_;	//base_laer in base link

};

#define BASE_LASER "base_laser"
#define BASE_LINK  "base_link"
#define BASE_ODOM  "base_odom"

class tf_2d_server
{
public:

	tf_2d_server();
	~tf_2d_server();

	void set_base_laser_in_base_link(const F32& x,  const F32& y , const F32& th);
	void tf_base_laser_to_base_link(VecPosition& p);
	void tf_base_link_to_base_laser(VecPosition& p);

protected:
private:
	typedef std::map<std::string,tf_2d> M_tf_2d;
	M_tf_2d tf_2d_list_;
	
	bool get_tf( std::string parent,std::string child,SPos& pos );
	bool create_tf(std::string parent,std::string child,const SPos& pos);
};

typedef boost::serialization::singleton<tf_2d_server> Singleton_TF;

#define TF_SERVER Singleton_TF::get_mutable_instance()

class my_tf
{
public:
	my_tf();
	~my_tf();

	void setLaserInBaselink(F32 x ,F32 y ,F32 th);

	void getDLaserInBaselink(F32 &x, F32 &y,F32 amcl_rotation);
	void getDLaserInBaseOdom(F32 &x,F32 &y,F32 ox,F32 oy,F32 oth);
	
	void getBaselinkInLaser( VecPosition &npos );
	//set laser angle by begin angle and resolution
	void setLaserPara(const SLaser &lpara);
	void getLaserXY(SLaserXY &laserxy,const SLaser &lpara,F32 amcl_rotation);

private://para




protected:
private:

	SPos pos_;	//base_laer in base link

	F32 angle_list_[LASER_COUNT];

	F32 cos_list_[LASER_COUNT];
	F32 sin_list_[LASER_COUNT];
};

class layer_tf
{
public:
	layer_tf();
	~layer_tf();

	std::string parent_;	//where layer_diff_'s original 
	std::string child_;    //

	S64 diff_x_;
	S64 diff_y_;

};

class map_tf_server
{

public:

	map_tf_server();
	~map_tf_server();

	bool create_tf(std::string parent,std::string child,S64 diff_x,S64 diff_y);

	void get_tf(std::string parent,std::string child,S64 &diff_x,S64 &diff_y);


private:
	
	typedef std::map<std::string,layer_tf> LayerMap;
	LayerMap layer_list_;


};



#endif//_MY_TF_KYOSHO_20150906_
