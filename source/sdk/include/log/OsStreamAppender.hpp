/*
 * OstreamAppender.cpp
 *
 * Copyright 2000, LifeLine Networks BV (www.lifeline.nl). All rights reserved.
 * Copyright 2000, Bastiaan Bakker. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _ARM_OS_OS_STREAM_APPENDER_KYOSHO_3_13_H__
#define _ARM_OS_OS_STREAM_APPENDER_KYOSHO_3_13_H__

#include <log4cpp/Portability.hh>
#ifdef LOG4CPP_HAVE_UNISTD_H
#    include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <log4cpp/Portability.hh>
#include <string>
#include <iostream>
#include <log4cpp/LayoutAppender.hh>
#include <vector>

namespace log4cpp {

/**
 * OstreamAppender appends LoggingEvents to ostreams.
 **/
class LOG4CPP_EXPORT ARMOstreamAppender: public LayoutAppender {
public:
	ARMOstreamAppender(const std::string& name, std::ostream* stream) :
		LayoutAppender(name), _stream(stream) {

	};
	virtual ~ARMOstreamAppender() {
		close();
	};

	virtual bool reopen() {
		return true;
	};
	virtual void close() {

	};

	void AddDisableNetName(std::string name, int level = Priority::DEBUG) {
		m_disable_name.insert(make_pair(name, level));
	};

	void RemoveDisableNetName(std::string name) {
		std::map<std::string, int>::iterator it = m_disable_name.find(name);
		if (it != m_disable_name.end()) {
			m_disable_name.erase(it);
		}
	};

	void AddDisablePrintName(std::string name, int level = Priority::DEBUG) {
		m_disable_print_name.insert(make_pair(name, level));
	};

	void RemoveDisablePrintName(std::string name) {
		std::map<std::string, int>::iterator it = m_disable_print_name.find(
				name);
		if (it != m_disable_print_name.end()) {
			m_disable_print_name.erase(it);
		}
	};

#ifdef _ARM_SIG_
	template<typename CallBack>
	void bindFunction(CallBack f) {
		return m_string_sig.push_back(f);
		//m_func = f;
	};

	void ClearBindAll() {
		m_string_sig.clear();
	};

#endif

protected:
	void UpdatePrint(const LoggingEvent& event) {
		std::map<std::string, int>::iterator it;
		if ((it = m_disable_print_name.find(event.categoryName))
				!= m_disable_print_name.end()) {
			if (event.priority >= it->second)
				return;
		}

		printf(_getLayout().format(event).c_str());
		fflush(stdout);
	};

	virtual void _append(const LoggingEvent& event) {

		UpdatePrint(event);

		std::map<std::string, int>::iterator it;
		if ((it = m_disable_name.find(event.categoryName))
				!= m_disable_name.end()) {
			if (event.priority >= it->second)
				return;
		}

#ifdef _ARM_SIG_
		for(int i = 0; i < (int)m_string_sig.size(); i++) {
			m_string_sig[i](_getLayout().format(event), event.priority,
					event.timeStamp);
		}
#endif

		//(*_stream) << _getLayout().format(event);
		//(*_stream) << _getLayout().format(event) <<std::endl;
		//if (!_stream->good()) {
		// XXX help! help!
		//}

	};

#ifdef _ARM_SIG_
	std::vector<boost::function<void(std::string, Priority::Value, TimeStamp)> > m_string_sig;
#endif

	std::ostream* _stream;

	std::map<std::string, int> m_disable_name;

	std::map<std::string, int> m_disable_print_name;
};
}

#endif
