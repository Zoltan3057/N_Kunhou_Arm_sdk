#ifndef _STRUCT_DRIVER_WANGHONGTAO_2015_09_17_
#define _STRUCT_DRIVER_WANGHONGTAO_2015_09_17_

#include "MyDefine.h"

#define CW 1
#define CCW -1

typedef enum{NO,STOP,RUN,ERRO,TEST} STATUS;


typedef struct{
	F32 p;
	F32 i;
	F32 d;
}MOTOR_PID;


typedef struct{

	U8 status;	
	S8 Direction;	
	U8 PWM;			
	U8 Disable;		

}PC_SET_STATUS;


typedef struct {

	U8 use_left;		
	U8 use_right;		

	U8 disable_left;		
	U8 disable_right;	


	F32 v_left;			
	F32 v_right;		

}Driver_2;

typedef struct{

	U8 status;		
	U8 disable;		
	U8 emergency;	
	U8 over_load;	

	U8 pwm;			
	S8 back1;
	S16 pos;		

	F32 RPM;  		

	F32 C_RPM;		
	F32 current;	

}DO_SPEED;

#endif//_STRUCT_DRIVER_WANGHONGTAO_2015_09_17_
