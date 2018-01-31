#ifndef _HD_REG_WANGHONGTAO_20151216_
#define _HD_REG_WANGHONGTAO_20151216_

#include "MyDefine.h"

#define ZYNQ_7020 1

typedef struct _SWeight_engine
{
	//laser in base link
	S32 BASE_LINK_X			;//0;	//
	S32 BASE_LINK_Y    		;//1;	//

	U32 PF0_XBASEODOM		;//2
	U32 PF0_YBASEODOM		;//3
	U32 PF0_TBASEODOM		;//4

	U32 RLASER_COUNT    		;//5;
	U32 PARTICLE_TRIGGER 		;//6;	//

	U32 back7				;
	U32 back8				;
	U32 back9				;

	U32 PF1_XBASEODOM		;//10
	U32 PF1_YBASEODOM		;//11
	U32 PF1_TBASEODOM		;//12

	U32 back13				;
	U32 back14				;
	U32 back15				;
	
	U32 PATICLE_SIZE    		;//16;   //64k particle 32k particle pairs

	U32 PATICLE_TOTAL_W 		;//17;	 //4 byte
	S32 RES_AMCL_POS_X    	;//18;
	S32 RES_AMCL_POS_Y    	;//19;
	S32 RES_AMCL_POS_TH    	;//20;

	U32 LASER_TF_TRIGGER	;//21;

}SWeight_engine;

typedef struct _SOdom_engine
{
	U32 HALF_FILTER_SIZE	;//0
	U32 INF_MAP_TRIGGER		;//1
	U32 back2				;//2
	U32 back3				;//3
	U32 back4				;//4
	U32 back5				;//5


	S32 ODOM_DISCRETE_TH1	 	;//6;
	U32 ODOM_DISCRETE_R1	 	;//7;
	S32 ODOM_DISCRETE_TH2 		;//8;
	U32 ODOM_DISCRETE_FH1	 	;//9;
	U32 ODOM_DISCRETE_FR	 	;//10;
	U32 ODOM_DISCRETE_FH2 		;//11;

	U32 ODOM_PATICLE_SIZE  		;//12;
	U32 back13;
	U32 back14;
	U32 ODOM_TRIGGER   			;//15;
	U32 SERIAL_NUMBER           ;//16
	//U32 MOTION_TYPE             ;//17
}SOdom_engine;

#ifdef ZYNQ_7020
//#define AMCL_MAP_ADD 0x30000000
//#define PARTICLE_ADD 0x38000000
//#define LASERHIT_ADD 0x3D000000

#define AMCL_MAP_ADD 0x3c000000
#define PARTICLE_ADD 0x38000000

#define LASERHIT_ADD 0x39000000
#define SAFELASERHIT_ADD_1 0x3A000000
#define SAFELASERHIT_ADD_2 0x3B000000

#else
#define AMCL_MAP_ADD 0x1C000000
#define PARTICLE_ADD 0x18000000
#define LASERHIT_ADD 0x14000000
#endif
#endif//_HD_REG_WANGHONGTAO_20151216_

