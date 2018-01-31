#ifndef _PROJECT_EDGE_BEZIER_KYOSHO_20160910_
#define _PROJECT_EDGE_BEZIER_KYOSHO_20160910_

#include <map>
#include <string>
#include "MyDefine.h"
#include "RobotStruct.h"

#include "project/project_define.h"
#include "project/edge_base.h"

class Edeg_bezier : public Edeg_base
{
public:
	Edeg_bezier();
	~Edeg_bezier();

	enum { Type = Object::EDGE };
	int type() const
	{ return Type;};

	void Code(std::ofstream& of);
	bool DeCode(std::string data);

	void SetCtlPos1(const F32 &x,const F32 &y);
	void SetCtlPos2(const F32 &x,const F32 &y);

	Sxy  GetCtlPos1();
	Sxy  GetCtlPos2(); 

protected:

private:

	Sxy ctl1_;
	Sxy ctl2_;
};


#endif//_PROJECT_EDGE_BASE_KYOSHO_20160910_
