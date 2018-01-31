#ifndef _PROJECT_KYOSHO_20160910_
#define _PROJECT_KYOSHO_20160910_

#include <map>
#include <string>
#include "MyDefine.h"
#include "RobotStruct.h"
#include "Manage.hpp"
#include "project/project_define.h"

#include "project/node_topology.h"
#include "project/edge_line.h"
#include "project/edge_bezier.h"


class Project
{
public:
	Project();
	~Project();

	void Init(std::string name);
	void Load(std::string path);
	void Save(std::string path);

	std::string GetProjectName();

	U32 CreateNodeTopology(const F32& fx,const F32& fy);
	std::vector<U32> DeleteNodeTopology(const U32& id);
	U32 CreateEdgeLine(const U32& p1_id,const U32& p2_id);
	bool DeleteEdgeLine(const U32& id);
	U32 CreateEdgeBezier(const U32& p1_id,const U32& p2_id);
	U32 CreateEdgeBezier(const U32& p1_id,const U32& p2_id,const F32& cp1x,const F32& cp1y,const F32& cp2x,const F32& cp2y);
	bool DeleteEdgeBezier(const U32& id);

	S32 GetAllNode();
	S32 GetAllEdge();

	Node_base* getNode(const U32 &id);
	Edeg_base* getEdge(const U32 &id);

	void update_topology_node(const U32 &id,const F32 &fx ,const F32 &fy);
	void update_edge(const U32& eb_id);
	void update_edge(Edeg_base* edge);
	void update_edge_bz(const U32& id,const F32& ctl1_x,const F32& ctl1_y,const F32& ctl2_x,const F32& ctl2_y);

	void set_map_file(const std::string &path);
	std::string get_map_file();


	Manage<Node_topology> get_node_topology();
	Manage<Edeg_line>     get_edge_line();
	Manage<Edeg_bezier>   get_edge_bezier();


protected:

private:
	std::string str_name_;
	EDITION     edition_;
	
	void create_file_path(std::string str_path);
	std::string path_map_;
	std::string path_topology_node_;
	std::string path_ln_edge_;
	std::string path_bz_edge_;

	void load_topology_node();
	void load_ln_edge();
	void load_bz_edge();

	void save_topology_node();
	void save_ln_edge();
	void save_bz_edge();
	
	Manage<Node_topology> node_topology_list_;
	Manage<Edeg_line> edge_line_list_;
	Manage<Edeg_bezier> edge_bezier_list_;

private:
	void init_ctl_pos( F32& cp1x,F32& cp1y,F32& cp2x,F32& cp2y, const U32& p1_id,const U32& p2_id );



};


#endif//_PROJECT_KYOSHO_20160910_
