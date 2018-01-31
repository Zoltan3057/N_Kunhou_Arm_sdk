#ifndef _PROJECT_EDGE_BASE_KYOSHO_20160910_
#define _PROJECT_EDGE_BASE_KYOSHO_20160910_

#include <map>
#include <string>
#include "MyDefine.h"
#include "RobotStruct.h"

#include "project/project_define.h"
#include "Object.h"

class Edeg_base : public Object
{
public:
	Edeg_base();
	~Edeg_base();

	void SetNode(const U32 &p1_id,const U32 &p2_id);
	void GetNode( U32 &p1_id, U32 &p2_id);
	void SetDir(const emEdgeDirType& dir_);

	void SetNodePos(const Sxy& node1,const Sxy& node2);
	void GetNodePos(Sxy& node1,Sxy& node2);

protected:

	U32 p1_id_;
	U32 p2_id_;

	emEdgeDirType dir_;

	Sxy node1_;
	Sxy node2_;

};


#endif//_PROJECT_EDGE_BASE_KYOSHO_20160910_
