#ifndef _BYTE_LIST_WANGHONGTAO_2014_08_30_
#define _BYTE_LIST_WANGHONGTAO_2014_08_30_

#include <boost/thread.hpp>

#define BUF_MAX_SIZE 102400

class ByteList{
protected:
	boost::mutex m_mutex;
	unsigned char* m_pBuffer;
	int			   m_iOffset;
	int			   m_iMax;
public:
	ByteList(): m_pBuffer(NULL),m_iMax(0),m_iOffset(0){};
	~ByteList(){
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
	};
	int Size(){
		boost::mutex::scoped_lock lock(m_mutex);
		return m_iOffset;
	};
	int Write(unsigned char* pWrite,int len){
		boost::mutex::scoped_lock lock(m_mutex);
		if ( len > (m_iMax - m_iOffset) )
		{
			len = m_iMax - m_iOffset;
		}

		if (m_pBuffer == NULL)
		{
			//LOGS_ERROR("TransferDevice")<<"write bufferP is null :"<<len;
			return 0;
		}
		assert(m_pBuffer!=NULL);
		memset(m_pBuffer + m_iOffset,0,len);
		memcpy(m_pBuffer + m_iOffset,pWrite,len);

		m_iOffset+=len;

		return len;
	};
	int Read(unsigned char* pRead,int& len){
		boost::mutex::scoped_lock lock(m_mutex);
		if (m_iOffset>0)
		{
			assert(m_pBuffer!=NULL);
			memset(pRead,0,m_iOffset);
			memcpy(pRead,m_pBuffer,m_iOffset);
			len = m_iOffset;

			if (m_pBuffer != NULL)
			{
				memset(m_pBuffer,0,m_iMax);
			}

			m_iOffset = 0;

			return m_iOffset;
		}
		else
		{
			return 0;
		}
	};
	void Init(int iLen){

		if (m_pBuffer != NULL)
		{
			delete m_pBuffer;
			m_pBuffer = NULL;
		}

		if (iLen>0 && iLen<=BUF_MAX_SIZE)
		{
			boost::mutex::scoped_lock lock(m_mutex);
			m_pBuffer = new unsigned char[iLen];
			m_iMax = iLen;
		}

	};
	void Clear(){
		boost::mutex::scoped_lock lock(m_mutex);
		if (m_pBuffer != NULL)
		{
			memset(m_pBuffer,0,m_iMax);
		}

		m_iOffset = 0;
	};
	bool IsPull(){
		boost::mutex::scoped_lock lock(m_mutex);
		if (m_iOffset == m_iMax )
		{
			return true;
		}
		else
		{
			return false;
		}

	};
};
#endif//_BYTE_LIST_WANGHONGTAO_2014_08_30_
