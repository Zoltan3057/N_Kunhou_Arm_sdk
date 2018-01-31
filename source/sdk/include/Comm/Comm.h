#ifndef _COMM_KYOSHO_20110903_
#define _COMM_KYOSHO_20110903_

#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/math/common_factor_rt.hpp>
#include "KHTypes.h"


/**************************************************************************************************
 * class cComm
***************************************************************************************************/
class cComm
{
public:
	cComm(void);
	~cComm(void);

public:
	static void sleep(F32 time);

	static int SplitString(const STRING        &input,
			               const STRING        &delimiter,
			               std::vector<STRING> &results);

	static int PatternCount(const STRING       &str,
							const STRING       &pattern);

	static void StringUpper(STRING &strDes);

    static STRING trim(STRING &str);

	static STRING ByteToHexString(U8   *pData,
			                      S32   iLen);

	static S32 HexStringToByte(U8      **pChar,
			                   S32      &iLen,
			                   STRING    strHex);

	static void Find_files(const STRING        &strdir,
			               const STRING        &filename,
			               std::vector<STRING> &v);

	static STRING GetRunPath();

	static bool FileExist(STRING strPath);

	static STRING Get_FileName(STRING strPath);

	static STRING Get_FileType(STRING strPath);

	static STRING Get_FilePath(STRING strPath);

	static bool load();

	static U32 GetBit(const U8 &data,
			          const U8 &pos );

	static U16 CRC16(U8 *p,U32 len);
	static bool Check_CRC16(U8 *p,U32 len);

public:
	template<typename T>
	static bool RangeIt(T &res,
			            T  min,
			            T  max)
	{
		if (res < min)
		{
			res = min;
			return false;
		}
		if (res > max)
		{
			res = max;
			return false;
		}
		return true;
	};

	template<typename T>
	static bool SaveGetFromBuffer(T& res,char* buffer,T tmin,T tmax)
	{
		T t;
		memcpy(&t,buffer,sizeof(T));
		if (!RangeIt(t,tmin,tmax))
		{
			memset(&tmax,0,sizeof(T));
			return false;
		}else
		{
			res = t;
			return true;
		}
	};

	template<typename T>
	static bool SetBufferOfType(char* buffer,const T& source)
	{
		if (!buffer)
		{
			return false;
		}
		memset(buffer,0,sizeof(T));
		memcpy(buffer,&source,sizeof(T));
		return true;
	};

	template<typename T>
	static bool num_valid(const char* str)
	{
		try
		{
			boost::lexical_cast<T>(str);
			return true;
		}
		catch(boost::bad_lexical_cast)
		{
			return false;
		}
	};

	template <class T> 
	static std::string ConvertToString(T value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	};

	template <class T>
	static T ConvertToNum(T &res,const std::string &str)
	{
		try
		{
			res = boost::lexical_cast<T>(str.c_str());
			return res;
		}
		catch (boost::bad_lexical_cast& e)
		{
			
		}
		return res;
	};

	template<class T>
	static T Max(T t1 ,T t2)
	{
		if (t1 > t2)
		{
			return t1;
		}
		else
		{
			return t2;
		}
	};

	template<class T>
	static T Min(T t1 ,T t2){
		if (t1 < t2)
		{
			return t1;
		}
		else
		{
			return t2;
		}
	};
	template<class T>
	static T Lcm(T t1, T t2)
	{
		return boost::math::lcm(t1,t2);
	};

private:
	static U8 *bit_value_;
};


/**************************************************************************************************
 * class Config
***************************************************************************************************/
#define _USE_INI_PARSER_CFG_  0

#if _USE_INI_PARSER_CFG_

#include "KHIniParser.h"

class Config
{
public:
	static bool loadConfig();
	static bool SaveConfig();

public:
	template<typename Type>
	static bool getConfig(STRING name, Type &out)
	{
		std::map<STRING, STRING>::const_iterator cit  = configs_.find(name) ;
		if (cit != configs_.end())
		{
			STRING str = cit->second;
			cComm::ConvertToNum(out,str);
			return true;
		}
		return false;
	};

	template<typename T>
	static void setConfig(STRING name, T value)
	{
		STRING str = cComm::ConvertToString<T>(value);
		configs_[name] = str;
	};

private:
	static std::map<STRING, STRING> configs_;
	static STRING                   run_path_;
	static INIParser                ini_parser_;
};

#else

#include "KHUtils.hpp"
#include "KHBoostInterproc.h"

class Config
{
public:
	template<typename Type>
	static bool getConfig(STRING name, Type &out)
	{
		bool    is_success = false;
		STRING  param_val;

		//
		try
		{
			BI::managed_shared_memory        cfg_data(BI::open_only, name.c_str());
			BI::named_mutex                  cfg_data_mtx(BI::open_only, "cfg_data_mtx");
			BI::scoped_lock<BI::named_mutex> lock(cfg_data_mtx);

			std::pair<BIString*, std::size_t> pair = cfg_data.find<BIString>(name.c_str());
			if( 0 != pair.first )
			{
				std::stringstream sstr;
				sstr<<(*pair.first);

				param_val = sstr.str();

				KHUtils::String2Type(param_val, out);
				//std::cout<<"****"<<name<<"***"<<param_val<<std::endl;

				is_success = true;
			}
		}
		catch(boost::interprocess::interprocess_exception &ex)
		{
			//std::cout<<"*** No such param! name:***"<<name<<" ***default value:"<<out<<std::endl;
		}

		return is_success;
	};

	static bool getConfig(STRING name, STRING &out)
	{
		bool    is_success = false;

		//
		try
		{
			BI::managed_shared_memory        cfg_data(BI::open_only, name.c_str());
			BI::named_mutex                  cfg_data_mtx(BI::open_only, "cfg_data_mtx");
			BI::scoped_lock<BI::named_mutex> lock(cfg_data_mtx);

			std::pair<BIString*, std::size_t> pair = cfg_data.find<BIString>(name.c_str());
			if( 0 != pair.first )
			{
				std::stringstream sstr;
				sstr<<(*pair.first);

				out = sstr.str();

		
				is_success = true;
			}
		}
		catch(boost::interprocess::interprocess_exception &ex)
		{
			std::cout<<"*** No such param! name:***"<<name<<" default value:"<<out<<std::endl;
		}

		return is_success;
	};
};
#endif



/**************************************************************************************************
 * Config_file
***************************************************************************************************/
class Config_file
{
public:
	bool loadConfig(STRING run_path)
	{
		std::cout<<"load config path:"<<run_path<<std::endl;

		configs_.clear();
		std::fstream stream( run_path.c_str(),
				             (std::ios::in) | (std::ios::out) );

		STRING key;
		STRING value;

		while(stream>>key)
		{
			stream>>value;
			if(key.find("#",0) == std::string::npos)
			{
				configs_[key] = value;
			}else
			{
				std::cout<<"ignore para key:"<<key<<" value:"<<value<<std::endl;
			}
		}

		stream.close();
		return true;
	};


	template<typename T>
	bool getConfig(STRING  name,
			       T      &out)
	{
		std::map<STRING, STRING>::const_iterator cit  = configs_.find(name) ;
		if (cit != configs_.end())
		{
			STRING str = cit->second;
			cComm::ConvertToNum(out,str);
			return true;
		}
		return false;
	};

	template<typename T>
	void setConfig(STRING name, T value)
	{
		STRING str = cComm::ConvertToString<T>(value);
		configs_[name] = str;
	};

private:
	std::map<STRING, STRING> configs_;
};


/**************************************************************************************************
 * Marocs
***************************************************************************************************/
#define SLEEP(time)      cComm::sleep(time)
#define SDelete( ptr )   do { if ( ptr ) { delete ( ptr ); ( ptr ) = 0; } } while(0)


#endif //_COMM_KYOSHO_20110903_
