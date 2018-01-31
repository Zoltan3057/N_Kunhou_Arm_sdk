
#ifndef _JSON_FORWARDS_H_
#define _JSON_FORWARDS_H_

#include "config.h"

namespace Json {

   // writer.h
   class FastWriter;
   class StyledWriter;

   // reader.h
   class Reader;

   // features.h
   class Features;

   // value.h
   typedef unsigned int ArrayIndex;
   class StaticString;
   class Path;
   class PathArgument;
   class Value;
   class ValueIteratorBase;
   class ValueIterator;
   class ValueConstIterator;

} // namespace Json


#endif // _JSON_FORWARDS_H_
