#ifndef _CHASSIS_GUIBU_DRIVER_WANGHONGTAO_2015_09_17_
#define _CHASSIS_GUIBU_DRIVER_WANGHONGTAO_2015_09_17_


#include "chassis/driver_base.h"

class cTransferDevice;

namespace guibu{


	#define CW 1
	#define CCW -1

	typedef enum{NO,STOP,RUN,ERR,TEST} STATUS;


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



	class guibu_driver : Driver_base
	{
	public:
		guibu_driver();
		~guibu_driver();

	protected:

	private://virtual
		void setPara(std::string name ,std::string value);
		bool open_transfer_device();
		bool init_driver();
		void close_driver();

		void setRPM(int id,F32 v);
		F32 getSpeed(int id);
		F32 getDiffAngle(int id);

		void setAngle(int id,F32 rad_angle);
		F32 getAngle(int id);

	private:
		std::string port_;
		//SerialPort* sport_;
		cTransferDevice* pTransDevice_;
		F32 reduction_ratio_;
		U32 counts_;

		U32 one_wheel_counts_;


		bool first_odo_;

		//my chassis example, customer should not do like this
		void init_customer_para();

		bool SetPID( U8 num,F32 P,F32 I );

		bool SetSpeedLR( F32 left,F32 right );
		F32 GetDiffPos(U8 id );
		void GetSpeedLR();

		void SendData( U8* s_data,U16 len );
		void ReadData( U8* r_data,int &len,int need,int timeout );


		F32 left_speed_;
		F32 right_speed_;

		U8 address_;

		U8 left_status_;
		U8 left_disable_;
		U8 left_emergency_;
		U8 left_over_load_;
		U8 left_pwm_;
		S8 left_direction_;
		S16 left_pos_;
		S16 last_left_pos_;
		S16 deta_left_pos_;

		F32 left_set_v;
		F32 left_current_v;
		F32 left_current;

		U8 right_status_;
		U8 right_disable_;
		U8 right_emergency_;
		U8 right_over_load_;
		U8 right_pwm_;
		S8 right_direction_;
		S16 right_pos_;
		S16 last_right_pos_;
		S16 deta_right_pos_;

		F32 right_set_v;
		F32 right_current_v;
		F32 right_current;
	
		F32 c_left_;
		F32 c_right_;

	
	};

}//end namespace guibu

#endif//_CHASSIS_GUIBU_DRIVER_WANGHONGTAO_2015_09_17_
