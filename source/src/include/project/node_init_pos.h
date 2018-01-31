#ifndef _PROJECT_NODE_INIT_POS_KYOSHO_20160910_
#define _PROJECT_NODE_INIT_POS_KYOSHO_20160910_

#include <map>
#include <string>
#include "MyDefine.h"

#include "project/node_base.h"

class Node_init_pos : public Node_base
{
public:
	Node_init_pos();
	~Node_init_pos();

	enum { Type = Object::INIT_NODE };
	int type() const
	{ return Type;};

	void GetPos(F32 &px,F32 & py,F32 & pa);
	void Code(std::ofstream& of);
	bool DeCode(std::string data);

protected:


private:
	F32 node_a_;
};

#endif//_PROJECT_NODE_INIT_POS_KYOSHO_20160910_
