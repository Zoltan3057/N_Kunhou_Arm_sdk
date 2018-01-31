#ifndef _PROJECT_EDGE_LINE_KYOSHO_20160910_
#define _PROJECT_EDGE_LINE_KYOSHO_20160910_

#include <map>
#include <string>
#include "MyDefine.h"
#include "RobotStruct.h"

#include "project/project_define.h"
#include "project/edge_base.h"

class Edeg_line : public Edeg_base
{
public:
	Edeg_line();
	~Edeg_line();

	enum { Type = Object::EDGE };
	int type() const
	{ return Type;};

	void Code(std::ofstream& of);
	bool DeCode(std::string data);



};


#endif//_PROJECT_EDGE_LINE_KYOSHO_20160910_
