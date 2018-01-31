#ifndef _CHASSIS_VISION_DRIVER_WANGHONGTAO_2016_10_28_
#define _CHASSIS_VISION_DRIVER_WANGHONGTAO_2016_10_28_

#include "TimerDiff.h"

#include "chassis/driver_base.h"

class cTransferDevice;

namespace vision{


	typedef struct{

		U8  head0_;
		U8  head1_;
		U8  cmd_;
		U8  add_;
		U8  len_;

		S32 left_wheel_counts_;
		S32 right_wheel_counts_;

		S32 back_;

		U8  crc0_;
		U8  crc1_;

	}UPLOAD;


	typedef struct{
		U8  head0_;
		U8  head1_;
		U8  cmd_;
		U8  add_;
		U8  len_;

		S32 left_wheel_rpm_;
		S32 right_wheel_rpm_;

		S32 back_;

		U8  crc0_;
		U8  crc1_;

	}DOWNLOAD;

	class vision_driver : Driver_base
	{
	public:
		vision_driver();
		~vision_driver();

	protected:

	private://virtual
		void setPara(std::string name ,std::string value);
		bool open_transfer_device();
		bool init_driver();
		void close_driver();

		void setRPM(int id,F32 v);
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

		//my chassis example, customer should not do like this
		void init_customer_para();

	
		U16 CRC16(U8 *p,U32 len);
		bool SetSpeedLR( F32 left,F32 right );
		F32 GetDiffPos(U8 id );
		void GetSpeedLR();

		void SendData( U8* s_data,U16 len );
		void ReadData( U8* r_data,int &len,int need,int timeout );


		F32 left_speed_;
		F32 right_speed_;

		S32 left_pos_;
		S32 last_left_pos_;
		S32 deta_left_pos_;

	
		S32 right_pos_;
		S32 last_right_pos_;
		S32 deta_right_pos_;

		cTimerDiff dt_;
	
		F32 c_left_;
		F32 c_right_;

private:
		
	DOWNLOAD download_cmd_;
	UPLOAD        upload_cmd_;
	
	};

}//end namespace guibu

#endif//_CHASSIS_VISION_DRIVER_WANGHONGTAO_2016_10_28_
