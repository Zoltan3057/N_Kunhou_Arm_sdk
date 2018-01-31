#ifndef _ANGLE_CALIBRATION_WANGHONGTAO_2016_11_21_
#define _ANGLE_CALIBRATION_WANGHONGTAO_2016_11_21_

#include <map>

#include "MyDefine.h"
#include <boost/thread.hpp>

#include "TimerDiff.h"
#include "RobotStruct.h"
#include "buffer_con.hpp"

class angle_calibration
{
public:
	angle_calibration();
	~angle_calibration();

	void init();
	void setZero(const S32 &ad_value);
	void setAngleMax(const S32 &ad_value);
	void setAngleMin(const S32 &ad_value);

	F32 getAngle(const S32 &ad_value);
	void setK(const F32 &k1,const F32 &k2);

	F32 getK1();
	F32 getK2();
	S32  getZero();

protected:

private:

	void cal_k();

	S32 ad_zero_;
	S32 ad_max_;
	S32 ad_min_;

	F32 k1_;
	F32 k2_;

};

class angle_manage
{
public:

	angle_manage();
	~angle_manage();

	F32 getAngle(const S32 &id,const S32 &ad_value);

	void setZero(const S32 &id,const S32 &ad_value);
	void setAngleMax(const S32 &id,const S32 &ad_value);
	void setAngleMin(const S32 &id,const S32 &ad_value);

	void setK(const S32 &id,const F32 &k1,const F32 &k2);

	void saveAngle();
	void loadAngle();

private:

	angle_calibration* getAngle_instance(const S32 &id);
	void destroy_angle_inc();
	

	std::map<U32, angle_calibration* > angle_list_;

};


#endif//_ANGLE_CALIBRATION_WANGHONGTAO_2016_11_21_
