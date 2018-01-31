#ifndef __LOG4CPP_ARM_H__
#define __LOG4CPP_ARM_H__

#define LOG4CPP_HAVE_INT64_T 
#define LOG4CPP_HAVE_INT64_T 
//typedef __int64 int64_t;

//#include <stdio.h>
#include "log4cpp/Portability.hh"
#ifdef LOG4CPP_HAVE_UNISTD_H
#include <unistd.h>
#endif
//#include <iostream>
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#ifdef LOG4CPP_HAVE_SYSLOG
#include "log4cpp/SyslogAppender.hh"
#endif
#include "log4cpp/Layout.hh"
//#include "log4cpp/BasicLayout.hh"
#include "log4cpp/SimpleLayout.hh"		//by laogu
#include "log4cpp/Priority.hh"
#include "log4cpp/NDC.hh"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>


#include <map>

#include <log4cpp/PatternLayout.hh>			//for PatternLayout
#include <log4cpp/BasicConfigurator.hh>		//for BasicConfigurator
#include "LogAppender.hpp"
#include "OsStreamAppender.hpp"

using namespace std;

using namespace log4cpp;

#ifndef _ARM_SIG_
#define _ARM_SIG_
#endif

class ARM_Log_Type {
public:
	ARM_Log_Type(log4cpp::Category& cat) :
		m_cat(cat) {
		m_cat.setPriority(Priority::INFO);
	};

	~ARM_Log_Type() {

	};

	log4cpp::Category& getCategory() {
		return m_cat;
	};

	log4cpp::Category& m_cat;
};

class ARM_Core_log: public Singleton<ARM_Core_log> {

private:
	map<string, boost::shared_ptr<ARM_Log_Type> > m_logs;

	boost::mutex m_mutex;

	log4cpp::PatternLayout* m_layout;

	log4cpp::PatternLayout* m_layout2;

	log4cpp::ARMOstreamAppender* m_syslogAppender;

	log4cpp::ARMLogAppender* m_string_appender;

public:
	ARM_Core_log() {

		m_layout = new log4cpp::PatternLayout();
		//m_logs.find(name)->second->m_
		m_layout->setConversionPattern("%d [%p] %c:%m %n");

		m_layout2 = new log4cpp::PatternLayout();
		//m_logs.find(name)->second->m_
		m_layout2->setConversionPattern("%d [%p] %c:%m %n");

		//m_syslogAppender = new log4cpp::OstreamAppender("syslogdummy",
		//		&std::cout);

		m_syslogAppender = new log4cpp::ARMOstreamAppender("syslogdummy",
				&std::cout);

		m_syslogAppender->setLayout(m_layout);

		m_string_appender = new log4cpp::ARMLogAppender("StringQueueAppender");
		m_string_appender->setLayout(m_layout2);

		log4cpp::Category& root = log4cpp::Category::getRoot(); //by laogu
		root.addAppender(m_syslogAppender); //
		root.addAppender(m_string_appender);

		root.setPriority(log4cpp::Priority::INFO);//INFO/ERROR

	};

	~ARM_Core_log() {
		if (!m_layout)
			delete m_layout;

		if (!m_layout2)
			delete m_layout2;

		if (!m_syslogAppender)
			delete m_syslogAppender;

		if (!m_string_appender)
			delete m_string_appender;
	};

#ifdef _ARM_SIG_
	template<typename CallBack>
	void bindFunction(CallBack f) {
		return m_syslogAppender->bindFunction(f);
	};

	void ClearBindAll() {
		return m_syslogAppender->ClearBindAll();
	};
#endif

public:

	//if type == 1, std out
	//if type == 0 std err
	log4cpp::Category & getLog(std::string name) {
		//
		boost::mutex::scoped_lock lock(m_mutex);

		map<string, boost::shared_ptr<ARM_Log_Type> >::iterator it =
				m_logs.find(name);

		if (it != m_logs.end()) {
			return it->second->getCategory();
		}

		boost::shared_ptr<ARM_Log_Type> log =
				boost::shared_ptr<ARM_Log_Type>(new ARM_Log_Type(
						log4cpp::Category::getInstance(std::string(name))));

		m_logs.insert(
				map<string, boost::shared_ptr<ARM_Log_Type> >::value_type(
						name, log));

		//m_logs.find(name)->second->getCategory().addAppender(appender);		//by laogu
		return m_logs.find(name)->second->getCategory();
	};

	void setPriority(string name, Priority::Value priority) {

		boost::mutex::scoped_lock lock(m_mutex);

		map<string, boost::shared_ptr<ARM_Log_Type> >::iterator it =
				m_logs.find(name);

		if (it != m_logs.end()) {
			it->second->getCategory().setPriority(priority);
		}
	};

	map<string, int> getAllLogInfo() {
		boost::mutex::scoped_lock lock(m_mutex);
		map<string, int> logInfoMap;
		for (map<string, boost::shared_ptr<ARM_Log_Type> >::iterator i =
				m_logs.begin(); i != m_logs.end(); i++) {
			logInfoMap.insert(map<string, int>::value_type(i->first,
					i->second->getCategory().getPriority()));
		}

		return logInfoMap;
	};

	void removeStdLogAll() {
		boost::mutex::scoped_lock lock(m_mutex);
		log4cpp::Category& root = log4cpp::Category::getRoot();
		root.removeAppender(m_syslogAppender);
	};

	void removeStdLog(std::string name, int level = Priority::DEBUG,
			bool remove_net = true) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_syslogAppender->AddDisablePrintName(name, level);

		if (remove_net) {
			m_syslogAppender->AddDisableNetName(name, level);
		}
	};

	void rebackStdLog(std::string name) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_syslogAppender->RemoveDisablePrintName(name);
	};

	void removeNetLog(std::string name, int level = Priority::DEBUG) {
		m_syslogAppender->AddDisableNetName(name, level);
	};

	void rebackNetLog(std::string name) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_syslogAppender->RemoveDisableNetName(name);
	};

	void enableFileLog(string name, bool const_name = false, std::string dir =
			"null", int type = -1) {
		boost::mutex::scoped_lock lock(m_mutex);
		long last_time;
		switch (type) {
		case -1:
			last_time = -1;
			break;
		case 1: //minute
			last_time = 60;
			break;
		case 2: // hour
			last_time = 60 * 60;
			break;
		case 3: // day
			last_time = 24 * 60 * 60;
			break;
		case 4: // week
			last_time = 24 * 60 * 60 * 7;
			break;
		case 5: //month
			last_time = 24 * 60 * 60 * 7 * 30;
			break;
		}

		m_string_appender->addFileLogEnable(name, const_name, dir, last_time);
	};

	void disableFileLog(string name) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_string_appender->removeFileLogEnable(name);
	};

};

#define LOG ARM_Core_log::GetSingleton()

#define LOGS_FATAL(str) LOG.getLog(str).fatalStream()

//#define LOGS_ALERT(str) LOG.getLog(str).alertStream()

//#define LOGS_CRIT(str) LOG.getLog(str).critStream()

#define LOGS_ERROR(str) LOG.getLog(str).errorStream()

#define LOGS_WARN(str) LOG.getLog(str).warnStream()

//#define LOGS_NOTICE(str) LOG.getLog(str).noticeStream()

#define LOGS_INFO(str) LOG.getLog(str).infoStream()

#define LOGS_DEBUG(str) LOG.getLog(str).debugStream()

#define LOGS_PRIORITY_FATAL(str) LOG.getLog(str).setPriority(Priority::FATAL)

//#define LOGS_PRIORITY_ALERT(str)  LOG.getLog(str).setPriority(Priority::ALERT)

//#define LOGS_PRIORITY_CRIT(str) LOG.getLog(str).setPriority(Priority::CRIT)

#define LOGS_PRIORITY_ERROR(str) LOG.getLog(str).setPriority(Priority::ERROR)

#define LOGS_PRIORITY_WARN(str) LOG.getLog(str).setPriority(Priority::WARN)

//#define LOGS_PRIORITY_NOTICE(str) LOG.getLog(str).setPriority(Priority::NOTICE)

#define LOGS_PRIORITY_INFO(str) LOG.getLog(str).setPriority(Priority::INFO)

#define LOGS_PRIORITY_DEBUG(str) LOG.getLog(str).setPriority(Priority::DEBUG)

#endif

/*
 typedef enum {
 EMERG  = 0,
 FATAL  = 0,
 ALERT  = 100,
 CRIT   = 200,
 ERROR  = 300,
 WARN   = 400,
 NOTICE = 500,
 INFO   = 600,
 DEBUG  = 700,
 NOTSET = 800
 } PriorityLevel;
 */
