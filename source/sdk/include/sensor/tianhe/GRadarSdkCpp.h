#ifndef _GRADARSDK_H_
#define _GRADARSDK_H_

#if defined(WIN32)||defined(WINNT)

#include <winsock2.h>
#include <Windows.h>
#include <process.h>

#ifndef STRUCTPACKED
#define STRUCTPACKED
#endif
#pragma comment(lib,"ws2_32.lib")

#else

#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h> 

#ifndef STRUCTPACKED
#define STRUCTPACKED __attribute__((__packed__))
#endif

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
using namespace std;


#define TIP_CODE_UNDEF		0	
#define TIP_CODE_HB			1	//Heartbeat
#define TIP_CODE_HBACK		2	//Heartbeat ack

#define TIP_CODE_REALTIME_LADARDATA				111	//Ladar-specified dataʵʱ�״����� data_1:����������data_2:������ʼ�Ƕȣ�data_3��������ֹ�Ƕȣ�data_4���Ƕȷֱ���*100

#define TIP_CODE_LOGIN							3050	//��½
#define TIP_CODE_LOGIN_BACK						3051	//��½�ظ�

#define TIP_CODE_DLL2SERVER_OK					3534	//�����״�ɹ�
#define TIP_CODE_DLL2SERVER_ERR					3535	//�����״�ʧ��

#define TIP_CODE_GET_SYS_STATE					3730	//��ȡϵͳ����״̬��GMDT->PCDT��GTOOLS->PCDT,PCDT�յ���������ͱ��ؼ�¼��ϵͳ״̬�ظ���GTOOLS����GBC��PCDTϵͳ�������·��������GSTR,TGCCI��VLS����ѯĿǰϵͳ״̬
#define TIP_CODE_GET_SYS_STATE_BACK				3731	//��ȡϵͳ����״̬�ظ���GTOOLS<-PCDT, GMDT<-PCDT��

#define TIP_CODE_SET_RADAR_TIME					6028 //�����״�ʱ�䣬resparm��u_param��time
#define TIP_CODE_SET_RADAR_TIME_BACK			6029 //�����״�ʱ��ظ�

#define TIP_CODE_GET_RADAR_INFO					7000 //��ȡ�״���Ϣ
#define TIP_CODE_GET_RADAR_INFO_BACK			7001 //��ȡ�״���Ϣ�ظ���date_1:���ذ汾��data_2:Ӳ�������汾��data_3:��������汾��data��������˾����+�豸�ͺ�+��������
#define TIP_CODE_GET_ALARM_AREA					7101 //��ȡ��������
#define TIP_CODE_GET_ALARM_AREA_BACK			7102 //��ȡ��������ظ���dataΪ������Ϣ
#define TIP_CODE_SET_ALARM_AREA					7103 //���ñ�������dataΪ������Ϣ
#define TIP_CODE_SET_ALARM_AREA_BACK			7104 //���ñ�������ظ���data_1���ɹ���1��ʧ�ܣ�0
#define TIP_CODE_RADAR_OCCLUDE					7105 //�״��ڵ�������80%�ĵ�С��1��Ϊ�ڵ�
#define TIP_CODE_RADAR_DISOCCLUDE				7106 //�״��ڵ�����
#define TIP_CODE_RADAR_DATA_ERR					7107 //�״������쳣��80%�ĵ�Ϊ�뾶
#define TIP_CODE_RADAR_DATA_OK					7108 //�״���������
#define TIP_CODE_RADAR_WORK_ERR					7109 //�״������쳣���״�û����Ȧ�Ĳ�������
#define TIP_CODE_RADAR_WORK_OK					7110 //�״﹤������
#define TIP_CODE_RADAR_ALARM					7111 //�״ﱨ��
#define TIP_CODE_RADAR_DISALARM					7112 //�״�����
#define TIP_CODE_RADAR_ALARM_OBJ				7113 //�״ﱨ��Ŀ������,resparam:data_1:Ŀ�������data��Ŀ������
#define TIP_CODE_RADAR_GET_IO_STATE				7117 //��ȡ�״�IO������Ϣ
#define TIP_CODE_RADAR_GET_IO_STATE_BACK		7118 //�״�IO������Ϣ��resparam:data_1��YK1;data_2��YK2;data_3��YK3;data_4��YK4; 0�رգ�1��
#define TIP_CODE_RADAR_CONTROL_IO				7119 //�״�IO�������resparam:data_1��YK1����YK4;data_2��0�رգ�1��
#define TIP_CODE_RADAR_CONTROL_IO_BACK			7120 //�״�IO��������ظ���resparam:data_1��YK1����YK4;data_2��0�رգ�1�򿪣�data_3:0ʧ�ܣ�1�ɹ�


//Retry times
#define TIP_RETRY_ZORE		 0x33	//retyt zore
#define TIP_RETRY_FOREVER	 (TIP_RETRY_ZORE-1) //For tips whihc should be tried sending-out forever.
#define TIP_RETRY_DATAL1	 (TIP_RETRY_ZORE+3)	//For data & heart-beat tips.
#define TIP_RETRY_DATAL2	 (TIP_RETRY_ZORE+5)	//For important data tips.
#define TIP_RETRY_EVTL2		 (TIP_RETRY_ZORE+3)	//For events which can be lost.
#define TIP_RETRY_ACTL2		 (TIP_RETRY_ZORE+3)	//For actions which can be missed.
#define TIP_RETRY_TICK		 (TIP_RETRY_ZORE+2)	//For tick.

//Priority
#define TIP_PRI_MAX				98		//MAX
#define TIP_PRI_MANUAL			91		//Emergency
#define TIP_PRI_EMRC			90		//Emergency
#define TIP_PRI_ERR_L1			80		//Software & cretical-equipment error
#define TIP_PRI_ERR_L2			81		//Secondary equipment error
#define TIP_PRI_ACT_L1			70		//Critical action commond
#define TIP_PRI_ACT_L2			71		//Normal commond
#define TIP_PRI_EVT_L1			60		//Important event
#define TIP_PRI_EVT_L2			61		//Normal event
#define TIP_PRI_DATA_L1			50		//Important data
#define TIP_PRI_DATA_L2			51		//Normal data
#define TIP_PRI_TICK			15		//Tick & tick-ack

//TIP mode
#define TIP_MODE_FIFO	1	//First-in-first-process. For tips whose created order MUST be maintained.
#define TIP_MODE_LIFO	2	//First-in-last-process. For data tips whose newest-state is prefered.

#define TIP_VERSION		4

typedef struct
{
	unsigned char			head_ad;		//head 0xAD
	unsigned char			head_49;		//head 0x0x49
	unsigned short			code;			//TIP code
	unsigned char			packtype;		//packtype,downstream(0x0:all, 0x1:up, 0x2:down); upstream(0x80:all,0x81:up,0x82:down);
	unsigned char			sotype;			//source object type
	unsigned char			sid;			//source id
	unsigned char			dotype;			//dest object type
	unsigned char			did;			//dest id
	unsigned char			rn;				//Maximum retried number
	unsigned char			pri;			//Prioriy
	unsigned char			mode;			//FIFO/LIFO
	unsigned char			version;		//version
	unsigned char			runmode;		//system run mode
	unsigned short			check;			//checksum
	unsigned int			abtLen;			//length of TIP	
	unsigned int			tid;			//ID of T. B:TIP_TID_B
	unsigned int			ctm;			//Created time
	int	  					cid;			//Connection ID. For transferring. 0 means posted by PostTIP. non-zero means by physical channel.
	short					data[4];		//
}STRUCTPACKED TIP_HEAD;

#define RADAR_LOGIN_USER "gjdt1d"
#define RADAR_LOGIN_PWD "gc343j46"

typedef struct
{
	char user[64];
	char pwd[64];
}STRUCTPACKED LOGIN_PARAM;

void _Sleep(int n);
bool TIP_MAKE(unsigned short code, unsigned char sid, TIP_HEAD* _tip, char* _data=NULL, int _data_len=0);
bool TIP_COPY(TIP_HEAD* _stip, TIP_HEAD*&_dtip);
TIP_HEAD* GetTipDefType(unsigned short code);
unsigned short TipCheckSum(TIP_HEAD * _tip);

void InitSocket();
void ExitSocket();

#if defined(WIN32)||defined(WINNT)

void Lock_Init(CRITICAL_SECTION &cs);
void Lock_Destroy(CRITICAL_SECTION &cs);
void Lock_Lock(CRITICAL_SECTION &cs);
void Lock_Unlock(CRITICAL_SECTION &cs);

void __cdecl ThreadRadarCltProc(void* lpParameter);
void __cdecl ThreadRadarTmProc(void* lpParameter);
typedef void (CALLBACK *RadarMsgCallBack)(unsigned int no, int dlen, void* data, int _paddr);

#else
void Lock_Init(pthread_mutex_t &m);
void Lock_Destroy(pthread_mutex_t &m);
void Lock_Lock(pthread_mutex_t &m);
void Lock_Unlock(pthread_mutex_t &m);

void* ThreadRadarCltProc(void* lpParameter);
void* ThreadRadarTmProc(void* lpParameter);
typedef void(*RadarMsgCallBack)(unsigned int no, int dlen, void* data, int _paddr);
#endif


typedef map<unsigned int, TIP_HEAD*>TIP_MAP;

class Mdata
{
public:
	Mdata(unsigned int _max=1000);
	~Mdata();
	bool PutTip(TIP_HEAD*&_tip);
	bool GetTip(TIP_HEAD*&_tip);
private:
#if defined(WIN32)||defined(WINNT)
	CRITICAL_SECTION m;
#else
	pthread_mutex_t m;
#endif
	unsigned int max;
	unsigned int no;
	TIP_MAP tip_map;
};
//
#define CRADARCLT_RECV_BUF_LEN 102400
#define CRADARCLT_SEND_BUF_LEN 1024
class CRadarClt
{
public:
	CRadarClt(int _id, const char* _ip, int _port, Mdata* _m_pdata);
	~CRadarClt();

	bool StartClt();
	bool RunProcClt();
	int SendData(const char* _data, int _data_len);
	int SendCodeToRadar(unsigned short _code, const char* _data=NULL, int _data_len=0);
	bool ContorlRadarIo(int _io, int _state);
	bool SetRadarTime(unsigned int _tm);
private:
	int ReadPackData(const char *buff, int buflen, char*& pack, int &plen);
	int FindAD49(unsigned char *buff, int BuffLen);
	int	 SocketRecv(int handle, char* data, int len);
	int  SocketSend(int handle, const char* data, int len);
	bool SocketClose(int &handle);
	int  ConnectServer(const char* ip, int port);
	int CheckRecvData(const char* _data, int _data_len);
	bool SendCodeToTm(unsigned short _code, const char* _data = NULL, int _data_len = 0);
	bool Login();
	bool SendHb();
	bool CheckHb();
private:
	int id;
	char ip[16];
	int port;
	int m_sock;
	int recv_len;
	unsigned int ntm;
	unsigned int last_send_hb_tm;
	unsigned int last_recv_hbback_tm;
	char recv_buf[CRADARCLT_RECV_BUF_LEN];
	char send_buf[CRADARCLT_SEND_BUF_LEN];
	Mdata *m_pMdata;
};


typedef map<int, CRadarClt*>ID_RADAR;

class CRadarTm
{
public:
	CRadarTm();
	~CRadarTm();

	bool StartTm();
	bool RunProcTm();
	
	bool SetCallBack(RadarMsgCallBack callback, int _paddr);
	bool CreatRadarConnect(int _id, const char* _ip, int _port);
	int SendDataToRadar(int _id, const char* _data, int _data_len);
	bool AskFactoryInfo(int _id);
	bool GetAlarmArea(int _id);
	bool SetAlarmArea(int _id, const char* _data, int _data_len);
	bool GetIoState(int _id);
	bool ControlIo(int _id, int _io, int _state);
	bool SetRadarTime(int _id, unsigned int _tm);
protected:
	Mdata m_data;

private:


private:
#if defined(WIN32)||defined(WINNT)
	CRITICAL_SECTION m;
#else
	pthread_mutex_t m;
#endif
	unsigned int usr_data;
	ID_RADAR id_radar;
	RadarMsgCallBack msgcallback;
};

#endif


