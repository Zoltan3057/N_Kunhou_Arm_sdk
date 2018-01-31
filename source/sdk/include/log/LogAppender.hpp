/*
 * StringQueueAppender.hh
 *
 * Copyright 2000, LifeLine Networks BV (www.lifeline.nl). All rights reserved.
 * Copyright 2000, Bastiaan Bakker. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _LOG4CPP_ARM_LOG_APPENDER_HH
#define _LOG4CPP_ARM_LOG_APPENDER_HH

#include <log4cpp/Portability.hh>
#include <string>
#include <map>
#include <queue>
#include <log4cpp/LayoutAppender.hh>
#include <boost/function.hpp>

#include <iostream>
#include <fstream>
#include <time.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <CountTime.hpp>
#include "LocalIp.hpp"

#define ARM_LOG_DIR "ARMLog/"

#ifndef _ARM_SIG_
#define _ARM_SIG_
#endif

namespace log4cpp {


class LogFileInfo{
public:
	LogFileInfo(std::ofstream *s , std::string name , long last_time){
		stream_ = s;
		last_time_ = last_time;
		name_ = name;
	}

	std::ofstream* stream_;
	CountTime time_;
	long last_time_ ;
	std::string name_;
};

class ARMLogData {
public:
	ARMLogData(std::string d = "", Priority::Value p = Priority::NOTSET) {
		data = d;
		priority = p;
	};

	ARMLogData(std::string d, Priority::Value p, TimeStamp t) {
		data = d;
		priority = p;
		timeStamp = t;
	};

	std::string data;
	Priority::Value priority;
	TimeStamp timeStamp;
};

/**
 * This class puts log messages in an in-memory queue. Its primary use
 * is in test cases, but it may be useful elsewhere as well.
 *
 * @since 0.2.4
 **/
class LOG4CPP_EXPORT ARMLogAppender: public LayoutAppender {
public:

	ARMLogAppender(const std::string& name) :
			LayoutAppender(name) {

	};
	virtual ~ARMLogAppender() {
		close();
	};

	virtual bool reopen() {
		return true;
	};
	virtual void close() {
		// empty
	};

	std::string GetSimpleDateTime() {
		time_t tt;
		struct tm *t;
		tt = time(0);
		t = localtime(&tt);
		char ch[40];
		sprintf(ch, "%04d-%02d-%02d-%02d%02d%02d", t->tm_year + 1900,
				t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		std::string str;
		str.append(ch);
		return str;
	};

	void CreateFloder(const std::string & dir)
	{
		boost::filesystem::path input_path = dir;
		boost::filesystem::path full_path_;
		if (!input_path.has_root_path())
		{
			full_path_ = boost::filesystem::initial_path<boost::filesystem::path>() / dir;
		}
		else
		{
			full_path_ = dir;
		}

		boost::filesystem::path parent_path = full_path_.parent_path();
		if (!exists(parent_path))
		{
			CreateFloder(parent_path.string());
			create_directory(full_path_);
		}
		else
		{
			if (!exists(full_path_))
			{
				create_directory(full_path_);
			}
		}
	};

	void addFileLogEnable(std::string name,bool const_name = true, std::string dir = "null" , long last_time = -1) {
		if (m_file_log_enable.find(name) == m_file_log_enable.end()) {
			std::ofstream * ss = new std::ofstream();
			std::stringstream s2;
			std::string use_dir;

			if (dir != "null")
			{
				use_dir = ARM_LOG_DIR + dir;
			}
			else
			{
				use_dir = ARM_LOG_DIR + LOCAL_IP.m_node_name;
			}

			s2 << use_dir << "/";

			CreateFloder(s2.str());

			std::string dir_name = s2.str();

			if (const_name == true){
				s2 << LOCAL_IP.m_node_name << "." << name << ".oslog";
			}
			else {
				//time name
				s2 << LOCAL_IP.m_node_name << "." << name << "_"<< GetSimpleDateTime() << ".oslog";
			}

			ss->open(s2.str().c_str(),
					std::ios::out | std::ios::binary);

			boost::shared_ptr<LogFileInfo> info = boost::shared_ptr<LogFileInfo>(new LogFileInfo(ss ,dir_name + name , last_time));

			m_file_log_enable.insert(make_pair(name, info));
		}
	};

	void removeFileLogEnable(std::string name) {
		std::map<std::string, boost::shared_ptr<LogFileInfo> >::iterator it;
		if ((it = m_file_log_enable.find(name)) != m_file_log_enable.end()) {
			it->second->stream_->close();
			delete it->second->stream_;
			m_file_log_enable.erase(name);
		}
	};



	/**
	 * Return the current size of the message queue.
	 * Shorthand for getQueue().size().
	 * @returns the queue size
	 **/
	virtual size_t queueSize() const {
		return getQueue().size();
	};

	/**
	 * Return the queue to which the Appends adds messages.
	 * @returns the message queue
	 **/
	virtual std::queue<ARMLogData>& getQueue() {
		return _queue;
	};

	/**
	 * Return the queue to which the Appends adds messages.
	 * @returns the message queue
	 **/
	virtual const std::queue<ARMLogData>& getQueue() const {
		return _queue;
	};

	/**
	 * Pop the oldest log message from the front of the queue.
	 * @returns the oldest log message
	 **/
	virtual ARMLogData popMessage() {
		ARMLogData message;

		if (!_queue.empty()) {
			message = _queue.front();
			_queue.pop();
		}

		return message;
	};

protected:

	/**
	 * Appends the LoggingEvent to the queue.
	 * @param event the LoggingEvent to layout and append to the queue.
	 **/
	virtual void _append(const LoggingEvent& event) {
		//m_func(_getLayout().format(event), event.priority,event.timeStamp);

		//boost::mutex::scoped_lock lock(m_file_mutex);

		//ARMLogData data(_getLayout().format(event), event.priority);
		//_queue.push(data);
		std::map<std::string, boost::shared_ptr<LogFileInfo> >::iterator it;
		if ((it = m_file_log_enable.find(event.categoryName))
				!= m_file_log_enable.end()) {
			*((it->second)->stream_) << _getLayout().format(event);
			(it->second->stream_)->flush();

			if(it->second->last_time_ < 0)
				return;

			//check last time
			(it->second)->time_.end();

			if(it->second->time_.getTime()/1000 > it->second->last_time_){
				it->second->stream_->close();
				delete it->second->stream_;
				std::stringstream s2;
				s2 <<it->second->name_ << "_"<< GetSimpleDateTime() << ".oslog";
				std::ofstream * ss = new std::ofstream();
				ss->open(s2.str().c_str(), std::ios::out | std::ios::binary);
				it->second->time_.begin();
				it->second->stream_ = ss;
			}

		}

	};

	void nullFunc(std::string, Priority::Value, TimeStamp) {

	};



	std::queue<ARMLogData> _queue;

	//boost::function<void(std::string, Priority::Value,TimeStamp)> m_func;



	std::map<std::string, boost::shared_ptr<LogFileInfo> > m_file_log_enable;

	boost::mutex m_file_mutex;
};
}

#endif // _LOG4CPP_STRINGQUEUEAPPENDER_HH
