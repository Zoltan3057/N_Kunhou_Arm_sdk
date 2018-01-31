
#ifndef _FPGA_TF_KYOSHO_20151206_
#define _FPGA_TF_KYOSHO_20151206_

#include "MyDefine.h"
#include "mapserver/map_def.h"

#define M_F32S16(f) (S16)((f)*F32_2_S16)
#define A_F32S16(f) (S16)((f)*8192)

#define S32_F32S16(f) (S32)((f)*65536)
#define U32_F32S16(f) (U32)((f)*65536)

#define U24_F32U24(f) (U32)((f)*5120) //*20*256
#define F32U81920(f) (U32)((f)*81920) //*20*4096
#define F32S81920(f) (S32)((f)*81920) //*20*4096

#define M_S16F32(f) (F32)(MAP_RESOLUTION*(f))
#define A_S16F32(f) (F32)(((F32)(f))/8192)

#define AR_S16F32(f) (F32)(((F32)(f))/142.977) //th / 8192 / 3.1415927 *180

#define A_angle  (25735)  //3.1415927*9182 = 25735.9273984
#define A_angle2 (51472)  //3.1415927*2*9182 51471.8547968

S16 A_Normalize(S16 rad);




//#define TEST_WEIGHT
//#define TEST_DISCRETE

#endif//_FPGA_TF_KYOSHO_20151206_
