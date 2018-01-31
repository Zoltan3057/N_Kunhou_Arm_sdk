#ifndef _TRANSFERDEVICE_KYOSHO_20110903_
#define _TRANSFERDEVICE_KYOSHO_20110903_


#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>


//#include "CRC.h"
#include "Manage.hpp"
#include "Object.h"
#include "ByteList.hpp"

//using namespace std;
//using namespace boost;
//using namespace boost::asio;



class cConnPara
{
public:
	static int SERIALPORT;
	static int NETWORK;
	static int TCPCLIENT;
	static int TCPSERVER;
	static int UPD;


	int m_iType;

	void setDeviceName(std::string nm);
	std::string getDeviceName();

	bool b_rs485;
	int  nBaud;
	char  chParity;
	int  nDataBits;
	int  nStopBits;

	int  iAdd;


	int  iDefSample;
	int  iTimeout;

	long  lReflash;

	int m_nNetType;
	//tcp client
	int m_nLocalPort;
	std::string m_nRemoteIP;
	int m_nRemotePort;
	//tcp server
	int n_listen_port;

	cConnPara();
	~cConnPara();

	bool CheckConn(const cConnPara& pConnPara);
	bool CanCreate(const cConnPara& pConnPara);

private:
	std::string sDeviceName; //device name win: com1 linux:ttyUSB0
};

class cStatus{

public:
	enum Status{STATUS_CLOSE,STATUS_OPEN,STATUS_READ};
	void SetOpen(){
		boost::mutex::scoped_lock lock(m_mutex);
		m_Status = STATUS_OPEN;
	};
	void SetClose(){
		boost::mutex::scoped_lock lock(m_mutex);
		m_Status = STATUS_CLOSE;
	}
	void SetRead(){
		boost::mutex::scoped_lock lock(m_mutex);
		m_Status = STATUS_READ;
	}
	Status GetStatus(){
		boost::mutex::scoped_lock lock(m_mutex);
		return m_Status;
	}
	bool IsOpen(){
		boost::mutex::scoped_lock lock(m_mutex);
		if (m_Status == STATUS_OPEN)
		{
			return true;
		}
		else
		{
			return false;
		}  
	}
	bool IsClose(){
		boost::mutex::scoped_lock lock(m_mutex);
		if (m_Status == STATUS_CLOSE)
		{
			return true;
		}
		else
		{
			return false;
		}  
	}
	bool IsRead(){
		boost::mutex::scoped_lock lock(m_mutex);
		if (m_Status == STATUS_READ)
		{
			return true;
		}
		else
		{
			return false;
		}  
	}
private:
	boost::mutex m_mutex;
	Status m_Status;
};



class cTransferDevice;

typedef std::map<int,cTransferDevice*> mTransferDeviceP;

class cTransferDevice : public Object
{

public:
	cStatus  m_Status;

	cTransferDevice(void);
	~cTransferDevice(void);
	
	int      m_iInstanceCount;
	static cTransferDevice* GetInstance( const cConnPara& pConnPara );
	static bool DelInstance(const cConnPara& ConnPara);
	static cTransferDevice* GetInstance(int id);
	static cTransferDevice* GetNextDevice(cTransferDevice* pTransfer = 0);

	bool Init(const cConnPara& ConnPara);             //��ʼ��
	int tID();
	void GetPara(cConnPara& ConnPara);

	virtual bool Open();
	virtual bool Close();
	virtual void SetOpen();

	bool WriteData(/*In*/unsigned char* pData,int Len);
	int  ReadData(/*Out*/unsigned char* pReceiveData,int& iLen,/*In*/int iReclen,int iTimeout);

	void read_callback( const boost::system::error_code& error, size_t bytes_transferred);
	void wait_callback(const boost::system::error_code& error);

protected:

	virtual bool Write();
	virtual void Read();

	bool BeginThread();
	void EndThread();

	boost::asio::io_service m_ios;

	//io_service m_ios2;
	cConnPara m_ConnPara;

	unsigned char  m_szReadTemp[BUF_MAX_SIZE];
	ByteList m_lReadBuffer;

	unsigned char  m_szWriteBuffer[BUF_MAX_SIZE];
	int m_nWriteBufferSize;


	//mutex			m_TimeOutmutex;  //
	bool            m_btime_out;
	static int m_bufSize;

private:

	//boost::mutex m_read_write_Lock;
	//boost::mutex m_ParaLock;
	//boost::mutex m_readlock;
	//boost::condition_variable_any m_condition;

	static mTransferDeviceP m_mTransferDeviceP;

	boost::thread* m_threadRun;
	int ThreadRun();
	bool th_run_;

	static int CreateID();
	static int id_all_;
	int id_;

public:
	

	enum { Type = Object::TRANSFER };
	int type() const
	{ return Type;};

	void Init();
	void Init( int id );

	static cTransferDevice* Create();
	static void Save(std::string path);
	static void Load(std::string path);
	static cTransferDevice* Get(int id);
	static cTransferDevice* GetNext(int id = 0);
protected:
	void Code(std::ofstream& of);
	bool DeCode(std::string data);
	
private:
	static Manage<cTransferDevice> list_;
};


#endif //_TRANSFERDEVICE_KYOSHO_20110903_
