/*
 * DataConvert.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: dl
 */
#ifndef DATACONVERT_H_
#define DATACONVERT_H_

#include "MyDefine.h"

class DataConvert {
public:
	DataConvert();
	~DataConvert();

   U16 ToU16(U8 *data,U32 startIndex);
   U32 ToU32(U8 *data,U32 startIndex);
   U64 ToU64(U8 *data,U32 startIndex);

   S16 ToS16(U8 *data,U32 startIndex);
   S32 ToS32(U8 *data,U32 startIndex);
   S64 ToS64(U8 *data,U32 startIndex);
   float Tofloat(U8 *data,U32 startIndex);
   double Todouble(U8 *data,U32 startIndex);

   double Todouble(unsigned short data);

};


#endif
