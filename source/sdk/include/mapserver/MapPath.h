#ifndef _PATH_WANGHONGTAO_20151123_
#define _PATH_WANGHONGTAO_20151123_

#include <list>

#include "MyDefine.h"
#include "RobotStruct.h"

#define PATH_END -2
#define MIN_SPEED_RESOLUTION (50)  // 1m / 50 = 0.02m

class path_package
{
public:
	path_package(){};
	~path_package(){};

	void setPoint(U16 x,U16 y,S8 value = 0);
	void setPoint(U16 x,U16 y,S8 value,S8 dir,S8 ptype,S8 speed);
	S32 getPoint(U16 &x,U16 &y,S8 &value);
	S32 getPoint( SGridXY &value );
	S32 getPoint( std::list<SGridXY> &path_list );

	S32 Coder(U8* &data,U32 &len);
	S32 Decoder(const U8* data,U32 len);

	S32 size();

protected:
private:
	
	std::list<SGridXY> path_list_;

};

#endif//_PATH_WANGHONGTAO_20151123_
