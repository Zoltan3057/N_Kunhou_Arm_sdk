#ifndef _PROJECT_NODE_BASE_KYOSHO_20160910_
#define _PROJECT_NODE_BASE_KYOSHO_20160910_

#include <map>
#include <string>
#include <vector>
#include "MyDefine.h"
#include "RobotStruct.h"

#include "project/project_define.h"


class Node_base : public Object
{
public:
	Node_base();
	~Node_base();

	void SetPos(const F32& px,const F32& py);
	void GetPos(F32 &px,F32 & py);
	Sxy GetPos();

	void GetRefNode(std::vector<int> & ref_node);


	void gedEdge(std::vector<int> &list_edge);
	void addEdge(U32 id);
	void rmEdge(U32 id);

	void addNode(U32 id);
	void rmNode(U32 id);

	bool check_ref(const U32 &id);

protected:

	F32 node_x_;
	F32 node_y_;

	std::vector<int> ref_node_list_;
	std::vector<int> ref_edge_list_;

private:

	
};


#endif//_PROJECT_NODE_BASE_KYOSHO_20160910_
