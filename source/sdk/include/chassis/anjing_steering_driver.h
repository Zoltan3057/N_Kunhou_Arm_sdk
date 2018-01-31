#ifndef _ANJIN_Ed_H_
#define _ANJIN_Ed_H_

#include "TimerDiff.h"

#include "chassis/driver_base.h"

#include "linux_driver/gpio.h"
class cTransferDevice;

	class anjing_steering_driver : Driver_base
	{
	public:
		anjing_steering_driver();
		~anjing_steering_driver();



	protected:

	private://virtual
		void setPara(std::string name ,std::string value);

		bool init_driver();
		void close_driver();

		void setRPM(int id,F32 v);
		void setAngle(int id,F32 rad_angle);
		F32 GetDiffPos(U8 id );
		F32 getAngle( int id );
		bool open_transfer_device();

		F32 getSpeed(int id);
		F32 getDiffAngle(int id);



	private:
		std::string port_;
		//SerialPort* sport_;
		cTransferDevice* pTransDevice_;
		F32 reduction_ratio_;
		U32 counts_;

		U32 one_wheel_counts_;


		bool first_odo_;
		bool first_do;

		//my chassis example, customer should not do like this
		void init_customer_para();


		void GetSpeedLR();

		void SendData( U8* s_data,U16 len );
		void ReadData(  U8* r_data,int &len,const int &need  );


		F32 wheel_speed_;
		S32 wheel_speed_1;
		S32 wheel_speed_2;
		S32 wheel_speed_3;
		S32 wheel_speed_4;
		F32 steer_angle_;
		F32 steer_angle_1;
		F32 steer_angle_2;
		F32 steer_angle_3;
		F32 steer_angle_4;
		U32 IO_data_;
		F32 IO_data_1;
		F32 IO_data_2;
		F32 IO_data_3;
		F32 IO_data_4;

		S32 wheel_pos_;
		S32 last_wheel_pos_;
		S32 deta_wheel_pos_;

		F32 feedback_angle_;

		U8 send_data[24];
		U8 ch_read[40];
		S32 line;

		cTimerDiff dt_;

		F32 c_wheel_;

		gpio g_gpio_;


	};

//

#endif
