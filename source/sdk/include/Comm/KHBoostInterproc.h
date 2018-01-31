/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    KHBoostInterproc.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-07-06
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _KH_BOOST_INTERPROC_H_
#define _KH_BOOST_INTERPROC_H_

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

namespace BI = boost::interprocess;

typedef BI::allocator<char, BI::managed_shared_memory::segment_manager> BICharAllocator;

typedef BI::basic_string<char, std::char_traits<char>, BICharAllocator> BIString;


#endif//_KH_BOOST_INTERPROC_H_
