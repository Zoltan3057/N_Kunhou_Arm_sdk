#ifndef _WANGHONGTAO_PID_2014_12_19_H_
#define _WANGHONGTAO_PID_2014_12_19_H_

#include "KHTypes.h"

class Pid{
public:

  Pid(){};
  ~Pid(){};

  void init(const F32 &kp,const F32 &ki,const F32 &kd,const F32 &vmax = 3.0,const F32 &vmin = -3.0 );
  void setP(const F32 &kp);
  void setI(const F32 &ki);
  void setD(const F32 &kd);
  void reset();

  F32 cal_pid(const F32 &setV,const F32 &actV);

private:

	F32 err_;
	F32 err_last_;
	F32 Kp_,Ki_,Kd_;
	F32 out_;
	F32 integral_;
	F32 vmax_;
	F32 vmin_;

};

#endif//_WANGHONGTAO_PID_2014_12_19_H_
